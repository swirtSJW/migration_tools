<?php

/**
 * @file
 * Defines JusticeHtmlToNodeMigration class.
 */

/**
 * Class JusticeHtmlToNodeMigration.
 *
 * Parent class for all migrations from static HTML to any content type.
 *
 * @package doj_migration
 */
abstract class JusticeHtmlToNodeMigration extends JusticeFileSourceMigration {

  /**
   * Define $this->source, $this->map, and simple mappings.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   * @param array $source_fields
   *   An associative aray of fields contained in the source data, in the form
   *   of array('machine_name' => t('Human Readable')).
   * @param array $source_dirs
   *   A flat array of the source directories containing html files.
   * @param array $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   */
  public function __construct($arguments, $source_fields, $source_dirs, $regex = '/.*\.htm(l)?$/', $options = array()) {

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('page');

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
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');

    // Create redirects using relative, legacy path of static file.
    $this->addFieldMapping('migrate_redirects', 'legacy_path')->separator('|');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row, SourceParser $source_parser = NULL) {
    parent::prepareRow($row);

    // Sets $row->legacy_path and $row->url_path.
    $this->generateLegacyPath($row);

    // Check whether a redirect to this page already exists, if so, do not
    // migrate the "duplicate" content.
    if ($this->isDuplicateByRedirect($row)) {
      return FALSE;
    }

    // Set to admin for now.
    $row->uid = 1;

    // Create a new SourceParser to handle HTML content.
    if (!$source_parser) {
      $source_parser = new SourceParser($row->url_path, $row->filedata);
    }

    $row->title = $source_parser->getTitle();
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    doj_migration_prepare_workbench_settings($entity, $row);

    // Sidestep language to get redirects and aliases working for all users.
    parent::languageSideStep($entity);
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    // Change the language back if it was sidestepped in prepare().
    // Must come before workbench settings or revision ends up unpublished.
    parent::languageReturnStep($entity);

    if (isset($entity->nid) && $entity->nid) {
      doj_migration_complete_workbench_settings($entity, $row);
    }

  }

  /**
   * Is this document a duplicate? Determine this by using redirect objects.
   *
   * Legacy paths from justice, should not be pointing to more than one node,
   * If this is happening, it is a good sign that we are bringing in duplicate
   * content.
   *
   * @param object $row
   *   A row object as delivered by migrate.
   *
   * @return bool
   *   Whether this row is a duplicate or not.
   */
  public function isDuplicateByRedirect($row) {
    $parsed = redirect_parse_url($row->legacy_path);
    $source = isset($parsed['path']) ? ltrim($parsed['path'], '/') : '';
    $redirect = redirect_load_by_source($source);

    if ($redirect) {
      $message = "The source path '@source' is already being redirected. The redirect is to '@redirect'. The document was not migrated.";
      watchdog('doj_migration', $message, array('@source' => $source, '@redirect' => $redirect->redirect), WATCHDOG_WARNING);
      return TRUE;
    }
    return FALSE;
  }
}
