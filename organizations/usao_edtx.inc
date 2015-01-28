<?php

/**
 * @file
 * Defines migration classes for Eastern District of Texas.
 */

/**
 * Migrates .html files from /usao-edtx to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edtx
 */
class UsaoEdtxPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/txe',
      1 => 'usao/txe/Districts',
      6 => 'usao/txe/Programs',
      8 => 'usao/txe/Divisions/criminal',
      9 => 'usao/txe/Divisions/civil',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Texas.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edtx')
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
 * Migrates files (*.pdf) from /usao-edtx.
 *
 * @package doj_migration
 * @subpackage usao-edtx
 */
class UsaoEdtxFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/txe subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/txe',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-edtx/docs';

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
    return "usao-edtx";
  }
}

/**
 * Migrates press releases from /usao-edtx.
 *
 * @package doj_migration
 * @subpackage usao-edtx
 */
class UsaoEdtxPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edtx";
    $arguments['component_tid'] = 2076;

    $source_dirs = array(
      2 => 'usao/txe/News/2013',
      4 => 'usao/txe/News/2014',
      5 => 'usao/txe/News/2015',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
