<?php

/**
 * @file
 * Defines migration classes for USAO organization.
 *
 * Migrates US Attorneys District home pages from CSV.
 *
 * @todo Refactor this to pull data from relevant HTML files. This was written
 * as a proof-of-concept for source migration ids, to verify that 1) parent
 * groups could be set for these districts, and 2) staff profiles for each
 * district could be migrated.
 */

/**
 * Migrates .html files from /usao-espanol to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-espanol
 */
class UsaoEspanolPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['source_parser_class'] = "DistrictsSourceParser";

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/espanol',
    );

    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Executive Office for United States Attorneys in spanish.');
    $this->addFieldMapping('og_group_ref')->defaultValue('usao-espanol')->sourceMigration('UsaoDistrict');
  }

  /**
   * Processes additional logic and alterations row by row.
   *
   * @param object $row
   *   The migration row.
   *
   * @return bool
   *   FALSE only if the row should be skipped.
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao/espanol.
 *
 * @package doj_migration
 * @subpackage usao-espanol
 */
class UsaoEspanolFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/espanol subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/espanol',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao/espanol';

    $options = array('recurse' => TRUE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-espanol";
  }
}
