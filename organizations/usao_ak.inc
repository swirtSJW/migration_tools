<?php

/**
 * @file
 * Defines migration classes for District of Alaska.
 */

/**
 * Migrates .html files from /usao-ak to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ak
 */
class UsaoAkPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      1 => 'usao/ak',
      14 => 'usao/ak/news/2013',
      49 => 'usao/ak/news/2014',

    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Alaska.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ak')
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
 * Migrates files (*.pdf) from /usao-ak.
 *
 * @package doj_migration
 * @subpackage usao-ak
 */
class UsaoAkFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ak subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ak',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-ak/docs';

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
    return "usao-ak";
  }
}

class UsaoAkPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ak";
    $arguments['component_tid'] = 1686;

    $source_dirs = array(
      0 => 'usao/ak/news/2013/June_2013',
      3 => 'usao/ak/news/2013/December_2013',
      4 => 'usao/ak/news/2013/March_2013',
      5 => 'usao/ak/news/2013/April_2013',
      6 => 'usao/ak/news/2013/May_2013',
      7 => 'usao/ak/news/2013/January_2013',
      8 => 'usao/ak/news/2013/September_2013',
      9 => 'usao/ak/news/2013/August_2013',
      10 => 'usao/ak/news/2013/July_2013',
      11 => 'usao/ak/news/2013/October _2013',
      12 => 'usao/ak/news/2013/November_2013',
      13 => 'usao/ak/news/2013/February_2013',
      41 => 'usao/ak/news/2014/August_2014',
      42 => 'usao/ak/news/2014/March_2014',
      43 => 'usao/ak/news/2014/July_2014',
      44 => 'usao/ak/news/2014/October _2014',
      45 => 'usao/ak/news/2014/May_2014',
      46 => 'usao/ak/news/2014/April_2014',
      47 => 'usao/ak/news/2014/September_2014',
      48 => 'usao/ak/news/2014/February_2014',
      50 => 'usao/ak/news/2014/December_2014',
      51 => 'usao/ak/news/2014/November_2014',
      52 => 'usao/ak/news/2014/June_2014',
      53 => 'usao/ak/news/2014/January_2014',
      54 => 'usao/ak/news/2015/January_2015',
      55 => 'usao/ak/news/2015/February_2015',
      56 => 'usao/ak/news/2015/March_2015',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
