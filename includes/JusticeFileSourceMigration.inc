<?php

/**
 * @file
 * Defines JusticeBinaryFileMigration class.
 */

/**
 * Class JusticeFileSourceMigration
 *
 * Parent class for all migrations with source data stored in files.
 */
abstract class JusticeFileSourceMigration extends JusticeBaseMigration {

  // Stores base directory for legacy files.
  public $base_dir;

  /**
   * Prepare a file migration.
   *
   * This will setup $this->map, $this->destination, and $this->source.
   * It will also add field mappings for file location and redirects.
   *
   * All derived classes should define 'fileid' as the source key, as it is
   * used to create redirects.
   *
   * @param $group
   *   A migration group returned by  MigrateGroup::getInstance().
   * @param array $source_fields
   *   An associative aray of fields contained in the source data, in the form
   *   of array('machine_name' => t('Human Readable')).
   * @param array $source_dirs
   *   An array of source directories, relative to $this->base_dir.
   * @param string $regex
   *   The file mask. Only filenames matching this regex will be migrated.
   */
  public function __construct($group, $source_fields, $source_dirs, $regex) {
    parent::__construct($group);

    $this->base_dir = variable_get('doj_migration_base_dir', DRUPAL_ROOT . '/../../legacy/www.justice.gov');

    // Define the directories containing files to be migrated.
    $absolute_dirs = $this->prependJusticeFilepath($source_dirs);

    // $list_files will provide migrate with a list of alls file to be migrated.
    // $item_file provides methods for deriving a unique file identifier.
    // $this->source define the essential data source from which to migrate.
    $list_files = new MigrateListFiles($absolute_dirs, $this->base_dir, $regex);
    $item_file = new MigrateItemFile($this->base_dir);
    $this->source = new MigrateSourceList($list_files, $item_file, $source_fields);

    // Create redirects using relative, legacy path of static file.
    $this->addFieldMapping('migrate_redirects', 'legacy_path');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $row->legacy_path = substr($row->fileid, 1);
  }

  /**
   * Makes relative filepaths absolute based on $this->base_dir.
   *
   * @param array $relative_paths
   *   A flat array of relative directory paths.
   *
   * @return array
   *   An array of absolute directory paths, based on $this->base_path.
   */
  public function prependJusticeFilepath(array $relative_paths) {

    $absolute_paths = array();
    foreach ($relative_paths as $key => $relative_path) {
      $absolute_paths[$key] = $this->base_dir . '/' . $relative_path;
    }

    return $absolute_paths;
  }
}
