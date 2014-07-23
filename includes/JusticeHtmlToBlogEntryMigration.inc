<?php

/**
 * @file
 * Defines JusticeStaticToPageMigration class.
 */

/**
 * Class JusticeStaticToPageMigration.
 *
 * Parent class for all migrations from static HTML to blog_entry content type.
 *
 * @package doj_migration
 */
abstract class JusticeHtmlToBlogEntryMigration extends JusticeFileSourceMigration {

  /**
   * Define $this->source, $this->map, $this->destination, and simple mappings.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   * @param array $source_dirs
   *   A flat array of the source directories containing html files.
   * @param array $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   */
  public function __construct($arguments, $source_dirs, $options = array()) {

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'field_page_body' => t('Body'),
      'date' => t('Date'),
    );

    // Match .htm, .html files only.
    $regex = '/.*\.htm(l)?$/';

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

    // The destination is the blog_entry content type.
    $this->destination = new MigrateDestinationNode('blog_entry');

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'uid',
      'title',
    ));

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

    // Define non-simple field mappings.
    $this->addFieldMapping('field_blog_body', 'field_page_body');
    $this->addFieldMapping('field_blog_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');

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

    $row->title = $source_parser->getTitle();
    $row->field_page_body = $source_parser->getBody();
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    $this->prepareWorkbenchSettings($entity, $row);
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    if (isset($entity->nid) && $entity->nid) {
      $this->completeWorkbenchSettings($entity, $row);
    }
  }
}
