<?php

/**
 * @file
 * Defines HtmlFile Sourcer for gathering the Source of items to migrate.
 */

namespace MigrationTools\Source;

/**
 * Creates the migration source for Html file based migrations.
 *
 * Assign the migration source created by this class to $this->source within
 * the migration class.
 */
class HtmlFile {
  /**
   *The base domain destination for this source.
   * @var string
   */
  public $sourceDirectoryBase;
  public $source;
  public $listFiles;
  public $ItemFile;

  /**
   * Prepare the file source.
   *
   * All derived classes should define 'fileid' as the source key in
   * MigrateSQLMap(), as it is used to create redirects.
   *
   * @param array $source_dirs
   *   An array of source directories, relative to $this->baseDir.
   * @param string $regex
   *   (Optional) The file mask. Only filenames matching this regex will be
   *   migrated.  Uses case insensitive .html and .htm if empty.
   * @param array $scan_options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   * @param string $base_directory
   *   (Optional) The full migration source base directory if something other
   *   other than the stored migration_tools_source_directory_base is needed.
   */
  public function __construct($source_dirs, $regex = '', $scan_options = array(), $base_directory = NULL) {
    // Use the base directory defined by migration tools if none is present.
    if (empty($base_directory)) {
      $this->sourceDirectoryBase = variable_get('migration_tools_source_directory_base', NULL);
    }
    else {
      $this->sourceDirectoryBase = $base_directory;
    }

    if (empty($this->sourceDirectoryBase)) {
      throw new \MigrateException("There was no source directory base specified.");
    }

    // Use default regex of .html and .htm if none provided.
    $regex = (empty($regex)) ? '/.*\.htm(l)?$/i' : $regex;

    // Define the directories containing files to be migrated.
    $this->prependLegacyFilepath($source_dirs);

    // $list_files will provide migrate with a list of all files to be migrated.
    $this->listFiles = new \MigrateListFiles($source_dirs, $this->sourceDirectoryBase, $regex, $scan_options);

    // $item_file provides methods for retrieving a file given an identifier.
    $this->ItemFile = new \MigrateItemFile($this->sourceDirectoryBase, TRUE);

    // Defines what will become $this->source, essential data source from which
    // to migrate.
    $this->source = new \MigrateSourceList($this->listFiles, $this->ItemFile);

  }

  /**
   * Getter for the MigrateSourceList.
   *
   * @return object
   *   The MigrateSourceList object.
   */
  public function getSource() {
    return $this->source;
  }


  /**
   * Makes relative filepaths absolute based on $this->baseDomain.
   *
   * @param array $relative_paths
   *   A flat array of relative directory paths altered by reference.
   */
  public function prependLegacyFilepath(array &$relative_paths) {
    if (!empty($this->sourceDirectoryBase)) {
      foreach ($relative_paths as $key => $relative_path) {
        $relative_paths[$key] = $this->sourceDirectoryBase . '/' . $relative_path;
      }
    }
  }
}
