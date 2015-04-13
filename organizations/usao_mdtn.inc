<?php

/**
 * @file
 * Defines migration classes for Middle District of Tennessee.
 */

/**
 * Migrates .html files from /usao-mdtn to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-mdtn
 */
class UsaoMdtnPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/tnm',
      8 => 'usao/tnm/tnm',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Middle District of Tennessee.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-mdtn')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-mdtn.
 *
 * @package doj_migration
 * @subpackage usao-mdtn
 */
class UsaoMdtnFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/tnm subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/tnm',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-mdtn/docs';

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
    return "usao-mdtn";
  }
}

/**
 * Migrates press releases from /usao-mdtn.
 *
 * @package doj_migration
 * @subpackage usao-mdtn
 */
class UsaoMdtnPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-mdtn";
    $arguments['component_tid'] = 2066;

    $source_dirs = array(
      3 => 'usao/tnm/pressReleases/2015',
      4 => 'usao/tnm/pressReleases/2014',
      5 => 'usao/tnm/pressReleases/2013',
      // 7 => 'usao/tnm/tnm/pressReleases/2014',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
