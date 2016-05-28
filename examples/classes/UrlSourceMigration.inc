<?php

/**
 * @file
 * Defines FileSourceMigration class.
 */

/**
 * Class FileSourceMigration.
 *
 * Parent class for all migrations with source data from urls to live pages.
 *
 * @TODO This is an incomplete class and is largely just a placeholder
 *
 * @package migration_tools
 */
abstract class UrlSourceMigration extends \MigrationTools\HtmlFileBase {

  /**
   * Prepare the file source.
   *
   * This will setup $this->source.
   * It will also add field mappings for redirects.
   *
   * All derived classes should define 'fileid' as the source key in
   * MigrateSQLMap(), as it is used to create redirects.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    // $list_files will provide migrate with a list of all files to be migrated.
    // $item_file provides methods for retrieving a file given an identifier.
    // $this->source define the essential data source from which to migrate.
    $list_files = new MigrateListFiles($source_urls, $this->baseDir, $regex, $options, $parser);
    $item_file = new MigrateItemFile($this->baseDir, TRUE, $parser);
    $this->source = new MigrateSourceList($list_files, $item_file, $source_fields);
  }
}
