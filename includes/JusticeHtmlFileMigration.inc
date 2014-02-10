<?php

/**
 * @file
 * Defines JusticeBinaryFileMigration class.
 */

/**
 * Class JusticeHtmlFileMigration.
 *

 */
abstract class JusticeHtmlFileMigration extends JusticeFileSourceMigration {

  /**
   * Prepare a file migration.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   * @param array $source_fields
   *   An associative aray of fields contained in the source data, in the form
   *   of array('machine_name' => t('Human Readable')).
   * @param array $source_dirs
   *   An array of source directories, relative to $this->base_dir.
   * @param $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   */
  public function __construct($arguments, $source_fields, $source_dirs, $options) {

    // Match .htm, .html files only.
    $regex = '/.*\.htm(l)?/';

    // Calling parent constructor will set $this->source.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

    // A map of source HTML filename -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'fileid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Create redirects using relative, legacy path of static file.
    $this->addFieldMapping('migrate_redirects', 'legacy_path')->separator('|');
  }


  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    // Check to see if $row->fileid == 'index.html'. If so,
    // add both /path/index.html and /path as redirects.
    $row->legacy_path = substr($row->fileid, 1);
    if (preg_match('/index\.(htm(l?)|php)/', $row->fileid, $matches)) {
      $row->legacy_path .= '|' . rtrim(str_replace($matches[0], $row->fileid, ''), '/');
    }
  }
}
