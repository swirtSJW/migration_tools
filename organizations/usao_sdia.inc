<?php

/**
 * @file
 * Defines migration classes for Southern District of Iowa.
 */

/**
 * Migrates .html files from /usao-sdia to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdia
 */
class UsaoSdiaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ias',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Southern District of Iowa.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdia')
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
 * Migrates files (*.pdf) from /usao-sdia.
 *
 * @package doj_migration
 * @subpackage usao-sdia
 */
class UsaoSdiaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ias subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ias',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-sdia/docs';

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
    return "usao-sdia";
  }
}

/**
 * Migrates press releases from /usao-sdia.
 *
 * @package doj_migration
 * @subpackage usao-sdia
 */
class UsaoSdiaPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdia";
    $arguments['component_tid'] = 1806;

    $source_dirs = array(
      1 => 'usao/ias/news/2015',
      2 => 'usao/ias/news/2014',
      3 => 'usao/ias/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
