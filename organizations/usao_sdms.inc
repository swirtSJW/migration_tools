<?php

/**
 * @file
 * Defines migration classes for Southern District of Mississippi.
 */

/**
 * Migrates .html files from /usao-sdms to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdms
 */
class UsaoSdmsPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/mss',
      1 => 'usao/mss/press',
      43 => 'usao/mss/includes',
      44 => 'usao/mss/usao/mss',
      45 => 'usao/mss/usao/mss/press',
      58 => 'usao/mss/usao/mss/includes',
    );

    $arguments['source_parser_class'] = "SdmsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Southern District of Mississippi.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdms')
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
 * Migrates files (*.pdf) from /usao-sdms.
 *
 * @package doj_migration
 * @subpackage usao-sdms
 */
class UsaoSdmsFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/mss subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/mss',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-sdms/docs';

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
    return "usao-sdms";
  }
}

class UsaoSdmsPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdms";
    $arguments['component_tid'] = 1916;
    $arguments['source_parser_class'] = "SdmsPressSourceParser";

    $source_dirs = array(
      2 => 'usao/mss/press/January2013',
      4 => 'usao/mss/press/September2014',
      5 => 'usao/mss/press/JUNE 2014',
      6 => 'usao/mss/press/February 2013',
      8 => 'usao/mss/press/April2014',
      13 => 'usao/mss/press/April2013',
      14 => 'usao/mss/press/JULY2013',
      16 => 'usao/mss/press/November2013',
      18 => 'usao/mss/press/July 2014',
      19 => 'usao/mss/press/MARCH 2014',
      22 => 'usao/mss/press/January 2014',
      23 => 'usao/mss/press/MAY2013',
      26 => 'usao/mss/press/SEPTEMBER2013',
      29 => 'usao/mss/press/August2014',
      30 => 'usao/mss/press/October2013',
      32 => 'usao/mss/press/August2013',
      35 => 'usao/mss/press/December 2013',
      36 => 'usao/mss/press/March 2013',
      37 => 'usao/mss/press/OCTOBER2014',
      39 => 'usao/mss/press/NOVEMBER2014',
      40 => 'usao/mss/press/June2013',
      41 => 'usao/mss/press/May2014',
      42 => 'usao/mss/press/FEBRUARY 2014',
      43 => 'usao/mss/press/DECEMBER 2014',
      44 => 'usao/mss/press/JANUARY 2015',
      45 => 'usao/mss/press/FEBRUARY 2015',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
