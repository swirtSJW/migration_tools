<?php

/**
 * @file
 * Defines migration classes for Eastern District of Missouri.
 */

/**
 * Migrates .html files from /usao-edmo to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edmo
 */
class UsaoEdmoPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/moe',
      1 => 'usao/moe/includes',
    );

    $arguments['source_parser_class'] = "NGDistrictPageSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Missouri.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edmo')
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
 * Migrates files (*.pdf) from /usao-edmo.
 *
 * @package doj_migration
 * @subpackage usao-edmo
 */
class UsaoEdmoFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/moe subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/moe',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-edmo/docs';

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
    return "usao-edmo";
  }
}

/**
 * Migrates press releases from /usao-edmo.
 *
 * @package doj_migration
 * @subpackage usao-edmo
 */
class UsaoEdmoPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edmo";
    $arguments['component_tid'] = 1901;

    $source_dirs = array(
      2 => 'usao/moe/news/2015/january',
      3 => 'usao/moe/news/2015/february',
      4 => 'usao/moe/news/2015',
      5 => 'usao/moe/news/2014/november',
      6 => 'usao/moe/news/2014/july',
      7 => 'usao/moe/news/2014/may',
      8 => 'usao/moe/news/2014/january',
      9 => 'usao/moe/news/2014/october',
      10 => 'usao/moe/news/2014/august',
      11 => 'usao/moe/news/2014/september',
      12 => 'usao/moe/news/2014/february',
      13 => 'usao/moe/news/2014/march',
      14 => 'usao/moe/news/2014/december',
      15 => 'usao/moe/news/2014/june',
      16 => 'usao/moe/news/2014/april',
      18 => 'usao/moe/news/2013/november',
      19 => 'usao/moe/news/2013/july',
      20 => 'usao/moe/news/2013/may',
      21 => 'usao/moe/news/2013/january',
      22 => 'usao/moe/news/2013/october',
      23 => 'usao/moe/news/2013/august',
      24 => 'usao/moe/news/2013/september',
      25 => 'usao/moe/news/2013/february',
      26 => 'usao/moe/news/2013/march',
      27 => 'usao/moe/news/2013/december',
      28 => 'usao/moe/news/2013/june',
      29 => 'usao/moe/news/2013/april',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
