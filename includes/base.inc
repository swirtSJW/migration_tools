<?php

/**
 * @file
 * Contains base migration class for all justice.gov migrations.
 */


/**
 * Abstract intermediate class holding common settings.
 */
abstract class JusticeMigration extends DynamicMigration {

  public $base_dir;

  public function __construct() {
    parent::__construct();

    $this->team = array(
      new MigrateTeamMember('Matthew Grasmick', 'matthew.grasmick@acquia.com', t('Architect')),
    );
    $this->issuePattern = 'https://acquiadojo.atlassian.net/browse/:id:';

    // Since the base directory of the HTML files can change depending on the
    // environment, we keep it in a variable. There is no interface for this,
    // set it using drush vset.
    $this->base_dir = variable_get('doj_migration_base_dir', DRUPAL_ROOT . '/../../legacy/www.justice.gov');

    // Map path and pathauto fields to NULL, assigning to DNM.
    if (module_exists('path')) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (module_exists('pathauto')) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
  }

  /**
   * Add multiple field mappings at once.
   *
   * @param array $mappings
   *   An array of field mappings in the form of source_key => dest_key.
   *
   * @param boolean $warn_on_override
   *  Set to FALSE to prevent warnings when there's an existing mapping.
   */
  public function addFieldMappings(array $mappings, $warn_on_override = TRUE) {
    foreach ($mappings as $source => $destination) {
      $this->addFieldMapping($source, $destination, $warn_on_override);
    }
  }

  /**
   * @param array $relative_paths
   * @return array
   */
  public function prependJusticeFilepath(array $relative_paths) {

    $absolute_paths = array();
    foreach ($relative_paths as $key => $relative_path) {
      $absolute_paths[$key] = $this->base_dir . '/' . $relative_path;
    }

    return $absolute_paths;
  }

  /**
   * Prepare a file migration.
   *
   * This will setup $this->map, $this->destination, and $this->source.
   * It will also add field mappings for file location and redirects, which
   * relies on prepareRow() defining $row->filepath and $row->legacy_path
   * via $this->prepareFileRow().
   *
   * @param array $source_dirs
   *   An array of source directories, relative to $this->base_dir.
   *
   * @param string $dest_dir
   *   The destination directory uri, with public:// or private::/ prefix.
   *
   * @param string $regex
   *   The file mask. Only filenames matching this regex will be migrated.
   */
  public function prepareFileMigration($source_dirs, $dest_dir, $regex) {

    $this->map = new MigrateSQLMap($this->machineName,
      array('fileid' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      ),
      MigrateDestinationFile::getKeySchema()
    );

    // Define the directories containing files to be migrated.
    $absolute_dirs = $this->prependJusticeFilepath($source_dirs);

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'fileid' => t('File Location'),
    );

    // $list_files will provide migrate with a list of alls file to be migrated.
    // $item_file provides methods for deriving a unique file identifier.
    // $this->source define the essential data source from which to migrate.
    $list_files = new MigrateListFiles($absolute_dirs, $this->base_dir, $regex);
    $item_file = new MigrateItemFile($this->base_dir);
    $this->source = new MigrateSourceList($list_files, $item_file, $source_fields);
    $this->destination = new MigrateDestinationFile();

    $this->addFieldMapping('value', 'filepath');
    $this->addFieldMapping('destination_dir')->defaultValue($dest_dir);

    // Create redirects using relative, legacy path of static file.
    $this->addFieldMapping('migrate_redirects', 'legacy_path');

    $this->addUnmigratedDestinations(array('fid', 'uid', 'timestamp', 'destination_file', 'source_dir', 'preserve_files',
      'file_replace'));
    $this->removeFieldMapping('path');
    $this->removeFieldMapping('pathauto');
  }

  /**
   * Prepares a $row to be migrated.
   *
   * This is meant to be used in combination with $this->prepareFileMigration().
   *
   * @param object $row
   *   The row to be migrated.
   */
  protected function prepareFileRow(&$row) {
    $row->filepath = $this->base_dir . $row->fileid;
    $row->legacy_path = substr($row->fileid, 1);
  }
}


