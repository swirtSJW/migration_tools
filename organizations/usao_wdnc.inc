<?php

/**
 * @file
 * Defines migration classes for Western District of North Carolina.
 */

/**
 * Migrates .html files from /usao-wdnc to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdnc
 */
class UsaoWdncPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ncw',
      1 => 'usao/ncw/pr',
      2 => 'usao/ncw/memorial',
      3 => 'usao/ncw/victimwitness',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Western District of North Carolina.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wdnc')
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
 * Migrates files (*.pdf) from /usao-wdnc.
 *
 * @package doj_migration
 * @subpackage usao-wdnc
 */
class UsaoWdncFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ncw subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ncw',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-wdnc/docs';

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
    return "usao-wdnc";
  }
}

/**
 * Migrates press releases from /usao-wdnc.
 *
 * @package doj_migration
 * @subpackage usao-wdnc
 */
class UsaoWdncPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdnc";
    $arguments['component_tid'] = 1936;

    $source_dirs = array(
      5 => 'usao/ncw/pressreleases/2013',
      8 => 'usao/ncw/pressreleases/2014',
      9 => 'usao/ncw/pressreleases/2015',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
