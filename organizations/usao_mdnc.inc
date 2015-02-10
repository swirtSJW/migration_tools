<?php

/**
 * @file
 * Defines migration classes for Middle District of North Carolina.
 */

/**
 * Migrates .html files from /usao-mdnc to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-mdnc
 */
class UsaoMdncPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ncm',
      1 => 'usao/ncm/slideshow',
      2 => 'usao/ncm/Templates',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Middle District of North Carolina.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-mdnc')
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
 * Migrates files (*.pdf) from /usao-mdnc.
 *
 * @package doj_migration
 * @subpackage usao-mdnc
 */
class UsaoMdncFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ncm subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ncm',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-mdnc/docs';

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
    return "usao-mdnc";
  }
}

/**
 * Migrates press releases from /usao-mdnc.
 *
 * @package doj_migration
 * @subpackage usao-mdnc
 */
class UsaoMdncPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-mdnc";
    $arguments['component_tid'] = 1931;

    $source_dirs = array(
      3 => 'usao/ncm/news/2015',
      4 => 'usao/ncm/news/2014',
      5 => 'usao/ncm/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
