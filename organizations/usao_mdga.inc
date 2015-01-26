<?php

/**
 * @file
 * Defines migration classes for Middle District of Georgia.
 */

/**
 * Migrates .html files from /usao-mdga to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-mdga
 */
class UsaoMdgaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/gam',
      1 => 'usao/gam/contact_info',
      2 => 'usao/gam/divisions',
      3 => 'usao/gam/includes',
      5 => 'usao/gam/press_releases',
      13 => 'usao/gam/Old',
      14 => 'usao/gam/employee',
      15 => 'usao/gam/lecc',
      16 => 'usao/gam/victim_witness',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Middle District of Georgia.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-mdga')
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
 * Migrates files (*.pdf) from /usao-mdga.
 *
 * @package doj_migration
 * @subpackage usao-mdga
 */
class UsaoMdgaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/gam subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/gam',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-mdga/docs';

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
    return "usao-mdga";
  }
}

/**
 * Migrates press releases from /usao-mdga.
 *
 * @package doj_migration
 * @subpackage usao-mdga
 */
class UsaoMdgaPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-mdga";
    $arguments['component_tid'] = 1776;

    $source_dirs = array(
      10 => 'usao/gam/press_releases/2014',
      11 => 'usao/gam/press_releases/2013',
      20 => 'usao/gam/press_releases/2015',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
