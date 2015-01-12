<?php

/**
 * @file
 * Defines migration classes for District of Nebraska.
 */

/**
 * Migrates .html files from /usao-ne to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ne
 */
class UsaoNePageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ne',
      13 => 'usao/ne/vw/cases',
      14 => 'usao/ne/vw',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Nebraska.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ne')
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
 * Migrates files (*.pdf) from /usao-ne.
 *
 * @package doj_migration
 * @subpackage usao-ne
 */
class UsaoNeFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ne subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ne',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-ne/docs';

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
    return "usao-ne";
  }
}

class UsaoNePressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ne";
    $arguments['component'] = "USAO - District of Nebraska";

    $source_dirs = array(
      1 => 'usao/ne/press_releases/feb',
      2 => 'usao/ne/press_releases/jan',
      3 => 'usao/ne/press_releases/mar',
      4 => 'usao/ne/press_releases/dec',
      5 => 'usao/ne/press_releases/may',
      6 => 'usao/ne/press_releases/sep',
      7 => 'usao/ne/press_releases/oct',
      8 => 'usao/ne/press_releases/jul',
      9 => 'usao/ne/press_releases/nov',
      10 => 'usao/ne/press_releases/jun',
      11 => 'usao/ne/press_releases/apr',
      12 => 'usao/ne/press_releases/aug',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
