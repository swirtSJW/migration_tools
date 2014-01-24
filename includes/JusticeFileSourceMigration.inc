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
   * Prepare the file source.
   *
   * This will setup $this->source.
   * It will also add field mappings for redirects.
   *
   * All derived classes should define 'fileid' as the source key in
   * MigrateSQLMap(), as it is used to create redirects.
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
   * @param $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   * @param MigrateContentParser $parser
   *   A parser class that will provide file chunk ids. Defaults to
   *   MigrateSimpleContentParser in parent constructor.
   */
  public function __construct($arguments, $source_fields, $source_dirs, $regex, $options = array(), MigrateContentParser $parser = NULL) {
    parent::__construct($arguments);

    $this->base_dir = variable_get('doj_migration_base_dir', DOJ_LEGACY_WEBROOT);

    // Define the directories containing files to be migrated.
    $absolute_dirs = $this->prependJusticeFilepath($source_dirs);

    // $list_files will provide migrate with a list of alls file to be migrated.
    // $item_file provides methods for retrieving a file given an identifier.
    // $this->source define the essential data source from which to migrate.
    $list_files = new MigrateListFiles($absolute_dirs, $this->base_dir, $regex, $options, $parser);
    $item_file = new MigrateItemFile($this->base_dir);
    $this->source = new MigrateSourceList($list_files, $item_file, $source_fields);
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
