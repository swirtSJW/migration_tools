<?php

/**
 * @file
 * Defines migration classes for Eastern District of Kentucky.
 */

/**
 * Migrates .html files from /usao-edky to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edky
 */
class UsaoEdkyPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/kye',
      1 => 'usao/kye/divisions',
      2 => 'usao/kye/us_attorney',
      3 => 'usao/kye/ouroffice',
      4 => 'usao/kye/disclaimers',
      5 => 'usao/kye/news',
      10 => 'usao/kye/about',
      11 => 'usao/kye/programs',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Kentucky.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edky')
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
 * Migrates files (*.pdf) from /usao-edky.
 *
 * @package doj_migration
 * @subpackage usao-edky
 */
class UsaoEdkyFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/kye subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/kye',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-edky/docs';

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
    return "usao-edky";
  }
}

/**
 * Migrates press releases from /usao-edky.
 *
 * @package doj_migration
 * @subpackage usao-edky
 */
class UsaoEdkyPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edky";
    $arguments['component_tid'] = 1846;

    $source_dirs = array(
      6 => 'usao/kye/news/2015',
      7 => 'usao/kye/news/2014',
      8 => 'usao/kye/news/2014/disclaimers',
      9 => 'usao/kye/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
