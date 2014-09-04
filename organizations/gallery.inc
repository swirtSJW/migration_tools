<?php

/**
 * @file
 * Defines migration classes for importing gallery nodes.
 */

/**
 * Class GalleryMigration.
 *
 * @package doj_migration
 */
class GalleryMigration extends JusticeFileSourceMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'field_gallery_body' => t('Body'),
      'field_gallery_date' => t('Date'),
    );

    // Define source directories.
    $source_dirs = array(
      'css-gallery',
    );
    $options = array(
      'recurse' => FALSE,
    );

    // Match .htm, .html files only.
    $regex = '/gallery.*\.htm(l)?$/';

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

    $this->description = t('Migrates gallery nodes.');

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('gallery');

    // A map of source HTML filename -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'fileid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'uid',
      'title',
      'field_gallery_date',
      'field_gallery_body',
    ));

    // Define non-simple field mappings.
    $this->addFieldMapping('field_gallery_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('og_group_ref')->defaultValue('opa')->sourceMigration('Organization');

    // Create redirects using relative, legacy path of static file.
    $this->addFieldMapping('migrate_redirects', 'legacy_path')->separator('|');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Sets $row->legacy_path and $row->url_path.
    $this->generateLegacyPath($row);

    // Set to admin for now.
    $row->uid = 1;

    // Create a new SourceParser to handle HTML content.
    $source_parser = new SourceParser($row->url_path, $row->filedata);

    // Set title field and then remove markup.
    $title_element = $source_parser->queryPath->find('h2');
    if (!$title_element) {
      $title_element = $source_parser->queryPath->find('.gallerytitletext');
    }
    $row->title = $title_element->text();
    $title_element->remove();

    // Set date field and then remove date markup.
    $date_element = $source_parser->queryPath->find('.gallerydatetext')->first();
    $date = html_entity_decode($date_element->text());
    // Do not migrate dates like July 28 - Aug. 2, 2012 and April 25 & 26, 2012.
    if (strstr($date, '-') !== FALSE ||
        strstr($date, '&') !== FALSE) {
      $this->queueMessage(t('Gallery had multiple dates. Did not migrate date field'));
    }
    elseif (empty($date)) {
      $this->queueMessage(t('Could not find gallery date.'));
    }
    else {
      $row->field_gallery_date = $date;

    }
    $date_element->remove();

    // Set body field as all remaining markup.
    $row->field_gallery_body = $source_parser->queryPath->find('.gallerydescriptiontext')->parent('div')->innerHTML();
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    doj_migration_prepare_workbench_settings($entity, $row);
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    if (isset($entity->nid) && $entity->nid) {
      doj_migration_complete_workbench_settings($entity, $row);
    }
  }
}

/**
 * Class GalleryImagesMigration
 *
 * Migrates field collection items for Gallery nodes.
 */
class GalleryImagesMigration extends JusticeFileSourceMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->dependencies = array('Gallery');

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'field_gallery_image_title' => t('Title'),
      'field_gallery_image_body' => t('Body'),
      'field_gallery_image_image' => t('Image'),
      'field_gallery_image_credits' => t('Credits'),
    );

    // Define source directories.
    $source_dirs = array(
      'css-gallery',
    );
    $options = array(
      'recurse' => FALSE,
    );

    // Match .htm, .html files only.
    $regex = '/gallery.*\.htm(l)?$/';

    // Instantiate the parser object that will parse a single HTML file into
    // chunks that will be translated into internship nodes.
    $parser = new GalleryChunkParser();

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options, $parser);

    $this->description = t('Migrates gallery images (field collections).');

    // A map of source HTML filename -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationFieldCollection::getKeySchema()
    );

    $this->destination = new MigrateDestinationFieldCollection(
      'field_gallery_images',
      array('host_entity_type' => 'node')
    );

    // The gallery_id is the file name of the source gallery.
    $this->addFieldMapping('host_entity_id', 'gallery_id')
      ->sourceMigration('Gallery');

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'field_gallery_image_title',
      'field_gallery_image_body',
      'field_gallery_image_credits',
    ));

    $this->addFieldMapping('field_gallery_image_image:file_class')
      ->defaultValue('MigrateFileUri');
    // Map the file name of the image to the image field.
    $this->addFieldMapping('field_gallery_image_image', 'image_filename');
    // Define the source directory for the images.
    $this->addFieldMapping('field_gallery_image_image:source_dir')
      ->defaultValue($this->baseDir . '/css-gallery/images');

    // Map title and alt tags.
    $this->addFieldMapping('field_gallery_image_image:alt', 'image_alt');
    $this->addFieldMapping('field_gallery_image_image:title', 'image_alt');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Row ids are in the form of /file_name.html-?MIGRATECHUNK?-chunk-id.
    // We determine the original gallery id by stripping out only the file name
    // portion of this string.
    $row->gallery_id = substr($row->id, 0, strpos($row->id, '-?MIGRATECHUNK?-'));

    // Create a new SourceParser to handle HTML content.
    try {
      $source_parser = new SourceParser($row->url_path, $row->filedata, TRUE);

      $image_url = $source_parser->queryPath->find('.download a')->attr('href');
      $row->image_filename = basename($image_url);

      $row->image_alt = $source_parser->queryPath->find('img')->attr('alt');
      $row->image_alt = mb_strimwidth($row->image_alt, 0, 512, "...");

      $row->field_gallery_image_title = $source_parser->queryPath->find('.image-title')->text();
      $row->field_gallery_image_credits = $source_parser->queryPath->find('.image-credit')->text();
      $row->field_gallery_image_body = $source_parser->queryPath->find('.image-desc')->text();
    }
    catch (Exception $e) {
      watchdog('doj_migration', 'Gallery Image migration failed for Gallery Image @row_id. File will be skipped. Error message: @error', array('@row_id' => $row->id, '@error' => $e->getMessage()), WATCHDOG_ERROR);
      return FALSE;
    }
  }
}


/**
 * Parses a single gallery into a list gallery images.
 *
 * @package doj_migration
 * @subpackage core
 * @see GalleryImagesMigration
 */
class GalleryChunkParser extends JusticeChunkParser {

  /**
   * Sets $this->chunks.
   */
  public function setChunks() {
    // Each <li> in ul#thumbs is an image.
    $this->chunks = $this->sourceParser->queryPath->find('#thumbs li');
  }

  /**
   * {@inheritdoc}
   */
  public function getChunkIDs() {
    $chunks = $this->getChunks();

    $ids = array();

    foreach ($chunks as $chunk) {
      // The ID of each gallery image will be the href of the image file.
      $id = $chunk->find('.download a')->attr('href');
      // Filter out empty strings.
      if ($id) {
        $ids[] = $id;
      }
    }

    return $ids;
  }

  /**
   * {@inheritdoc}
   */
  public function getChunk($chunk_id) {
    $chunks = $this->getChunks();

    $chunk = $chunks->find('a[href="' . $chunk_id . '"]')->parent('li')->html();
    if (!$chunk) {
      $chunk = $chunks->find('a[name="' . $chunk_id . '"]')->parent()->html();
    }
    if (!$chunk) {
      $chunk = 'Data could not be migrated';
    }

    return $chunk;
  }
}
