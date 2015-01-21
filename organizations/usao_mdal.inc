<?php

/**
 * @file
 * Defines migration classes for Middle District of Alabama.
 */

/**
 * Migrates .html files from /usao-mdal to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-mdal
 */
class UsaoMdalPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/alm',
      11 => 'usao/alm/divisions',
      12 => 'usao/alm/includes',
      13 => 'usao/alm/about',
      14 => 'usao/alm/about/employment',
      15 => 'usao/alm/programs',
      16 => 'usao/alm/programs/vwa',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Middle District of Alabama.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-mdal')
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
 * Migrates files (*.pdf) from /usao-mdal.
 *
 * @package doj_migration
 * @subpackage usao-mdal
 */
class UsaoMdalFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/alm subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/alm',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-mdal/docs';

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
    return "usao-mdal";
  }
}

/**
 * Migrates press releases from /usao-mdal.
 *
 * @package doj_migration
 * @subpackage usao-mdal
 */
class UsaoMdalPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-mdal";
    $arguments['component'] = "USAO - Middle District of Alabama";

    $source_dirs = array(
      7 => 'usao/alm/press/2015',
      8 => 'usao/alm/press/2014',
      9 => 'usao/alm/press/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
