<?php

/**
 * @file
 * Defines migration classes for District of Hawaii.
 */

/**
 * Migrates .html files from /usao-hi to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-hi
 */
class UsaoHiPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/hi',
      1 => 'usao/hi/news',
      4 => 'usao/hi/about',
      5 => 'usao/hi/programs',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Hawaii.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-hi')
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
 * Migrates files (*.pdf) from /usao-hi.
 *
 * @package doj_migration
 * @subpackage usao-hi
 */
class UsaoHiFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/hi subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/hi',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-hi/docs';

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
    return "usao-hi";
  }
}

/**
 * Migrates press releases from /usao-hi.
 *
 * @package doj_migration
 * @subpackage usao-hi
 */
class UsaoHiPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-hi";
    $arguments['component_tid'] = 1796;

    $source_dirs = array(
      2 => 'usao/hi/news/2014',
      3 => 'usao/hi/news/2013',
      4 => 'usao/hi/news/2015',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
