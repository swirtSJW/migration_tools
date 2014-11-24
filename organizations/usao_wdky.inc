<?php

/**
 * @file
 * Defines migration classes for Western District of Kentucky.
 */

/**
 * Migrates .html files from /usao-wdky to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdky
 */
class UsaoWdkyPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/kyw',
      1 => 'usao/kyw/divisions',
      2 => 'usao/kyw/disclaimers',
      4 => 'usao/kyw/news',
      8 => 'usao/kyw/about',
      9 => 'usao/kyw/programs',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Western District of Kentucky.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wdky')
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
 * Migrates files (*.pdf) from /usao-wdky.
 *
 * @package doj_migration
 * @subpackage usao-wdky
 */
class UsaoWdkyFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/kyw subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/kyw',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-wdky/docs';

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
    return "usao-wdky";
  }
}

class UsaoWdkyPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdky";
    $arguments['component'] = "Western District of Kentucky (USAO)";

    $source_dirs = array(
      5 => 'usao/kyw/news/2014',
      6 => 'usao/kyw/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
