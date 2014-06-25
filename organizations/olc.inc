<?php

/**
 * @file
 * OLC.
 *
 * Defines migration classes for the OLC section of justice.gov.
 */

/**
 * Migrates .html files from /olc to page nodes.
 *
 * @package doj_migration
 * @subpackage olc
 */
class OlcPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'olc',
      'olc/docs',
      'olc/images',
      'olc/opniondocs',
      'olc/opinions',
      'olc/pdf',
    );

    // Fetch a list of Opinions.
    global $databases;
    $iso_db_name = $databases['iso']['default']['database'];
    $query = db_select("$iso_db_name.olc", 'olc')
      ->fields('olc')
      ->execute()
      ->fetchAllAssoc('id');
    // Assemble a regular expression of files to ignore.
    $excludes = array();
    foreach ($query as $id => $result) {
      // Strip the filename from the link.
      $regex = '/[^*\/]*\.*\z/';
      $filename = array();
      preg_match($regex, $result->link, $filename);
      // Add the filename to the list of files to exclude from the migration.
      $excludes[$id] = preg_replace('/\./', '\\.', $filename[0]);
    }
    // Convert the array of excluded files to a string.
    $exclusions = implode('|', $excludes);

    $nomask = '|' . $exclusions;
    $options = array(
      'recurse' => FALSE,
      'nomask' => '/(\.\.?|CVS' . $nomask . ')$/',
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from OLC section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('olc')->sourceMigration('Organization');
  }
}

/**
 * Migrates .html files from /olc to opinion nodes.
 *
 * @package doj_migration
 * @subpackage olc
 */
class OlcOpinionMigration extends JusticeBaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    global $databases;
    $iso_db_name = $databases['iso']['default']['database'];
    $query = db_select("$iso_db_name.olc", 'olc')
      ->fields('olc', array(
        'id',
        'articledate',
        'title',
        'postdate',
        'link',
        'overview',
      ));

    // A map of source HTML filename -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'int',
          'length' => 11,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceSQL($query);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates Opinion pages from OLC section.');
    $this->destination = new MigrateDestinationNode('opinion');
    // Field Mappings.
    $this->addFieldMapping('field_opinion_date', 'articledate');
    $this->addFieldMapping('title', 'title')
      ->callbacks(array($this, 'sanitizeTitle'), array($this, 'trimTitle'));
    $this->addFieldMapping('created', 'postdate');
    $this->addFieldMapping('field_opinion_overview', 'overview');
    $this->addFieldMapping('field_opinion_overview:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_opinion_body', 'link')
      ->callbacks(array($this, 'attachHtml'));
    $this->addFieldMapping('field_opinion_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_opinion_attachment', 'link');
    $this->addFieldMapping('field_opinion_attachment:file_class')
      ->defaultValue('MigrateFileUri');
    $this->addFieldMapping('field_opinion_attachment:source_dir')
      ->defaultValue(variable_get('doj_migration_base_dir', DOJ_LEGACY_WEBROOT) . '/olc/opiniondocs');
    $this->addFieldMapping('field_opinion_attachment:destination_dir')
      ->defaultValue('public://olc/opinions');
    $this->addFieldMapping('og_group_ref')->defaultValue('olc')->sourceMigration('Organization');
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');
    $this->addFieldMapping('status')->defaultValue(NODE_PUBLISHED);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Set to admin for now.
    $row->uid = 1;
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
    if (isset($entity->field_opinion_attachment['und'][0]['fid']) && $entity->field_opinion_attachment['und'][0]['fid']) {
      if (module_exists('redirect')) {
        // Strip the filename from the link.
        $regex = '/[^*\/]*\.*\z/';
        $filename = array();
        preg_match($regex, $row->link, $filename);
        $filepath = 'olc/opiniondocs/';

        // Construct the source link and check for an existing redirect.
        $src = $filepath . $filename[0];
        $redirect = redirect_load_by_source($src);
        if (!$redirect) {
          // Load the migrated file entity.
          $file = file_load($entity->field_opinion_attachment['und'][0]['fid']);
          $uri = $file->uri;
          // Strip leading public://.
          $file_public_path = variable_get('file_public_path', conf_path() . '/files/');
          $dest = str_replace('public:/', $file_public_path, $uri);
          // Create redirect.
          $redirect = new stdClass();
          redirect_object_prepare($redirect);
          $redirect->source = $src;
          $redirect->redirect = $dest;

          redirect_save($redirect);
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function sanitizeTitle($title) {
    // Create a new SourceParser to handle HTML content.
    $source_parser = new SourceParser(md5($title), $title, TRUE);
    if ($source_parser->queryPath->find('p')->text() != '') {
      return $source_parser->queryPath->find('p')->text();
    }
    else {
      return $title;
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function trimTitle($title) {
    $trimmed_title = trim($title);
    return mb_strimwidth($trimmed_title, 0, 255, "...");
  }

  /**
   * {@inheritdoc}
   */
  protected function attachHtml($source_link) {
    if (preg_match('/.*\.htm(l)?$/', $source_link)) {
      $source_link = ltrim($source_link, ' /');
      $base = variable_get('doj_migration_base_dir', DOJ_LEGACY_WEBROOT);
      if (file_exists($base . '/' . $source_link)) {
        $source_content = file_get_contents($base . '/' . $source_link);
        // Create a new SourceParser to handle HTML content.
        $source_parser = new SourceParser($source_link, $source_content);
        return $source_parser->getBody();
      }
    }
  }
}

/**
 * Migrates binary files from /olc.
 *
 * @package doj_migration
 * @subpackage olc
 */
class OlcFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates binary files from the olc subdirectory.');
    $this->dependencies = array('Organization');

    // Match binary files only.
    $source_dirs = array(
      'olc',
      'olc/docs',
      'olc/images',
      'olc/opniondocs',
      'olc/opinions',
      'olc/pdf',
    );

    // Fetch a list of Opinions.
    global $databases;
    $iso_db_name = $databases['iso']['default']['database'];
    $query = db_select("$iso_db_name.olc", 'olc')
      ->fields('olc')
      ->execute()
      ->fetchAllAssoc('id');
    // Assemble a regular expression of files to ignore.
    $excludes = array();
    foreach ($query as $id => $result) {
      // Strip the filename from the link.
      $regex = '/[^*\/]*\.*\z/';
      $filename = array();
      preg_match($regex, $result->link, $filename);
      // Add the filename to the list of files to exclude from the migration.
      $excludes[$id] = preg_replace('/\./', '\\.', $filename[0]);
    }
    // Convert the array of excluded files to a string.
    $exclusions = implode('|', $excludes);

    $nomask = '|' . $exclusions;
    $options = array(
      'nomask' => '/(\.\.?|CVS' . $nomask . ')$/',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///olc/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "olc";
  }
}
