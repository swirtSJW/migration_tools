<?php

/**
 * @file
 * Defines migration classes for Eastern District of Tennessee.
 */

/**
 * Migrates .html files from /usao-edtn to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edtn
 */
class UsaoEdtnPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/tne',
      31 => 'usao/tne/vw',
      32 => 'usao/tne/vw/cases',
    );

    $arguments['source_parser_class'] = "EdtnPageSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Tennessee.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edtn')
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
 * Migrates files (*.pdf) from /usao-edtn.
 *
 * @package doj_migration
 * @subpackage usao-edtn
 */
class UsaoEdtnFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/tne subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/tne',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-edtn/docs';

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
    return "usao-edtn";
  }
}

/**
 * Migrates press releases from /usao-edtn.
 *
 * @package doj_migration
 * @subpackage usao-edtn
 */
class UsaoEdtnPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edtn";
    $arguments['component_tid'] = 2061;

    $title = new ObtainerInfo('title', "ObtainTitlePressRelease");
    $title->addMethod("findH2Third");

    $date = new ObtainerInfo('date');
    $date->addMethod("findIdContentstartFirst");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;

    $arguments['source_parser_class'] = "EdtnPressSourceParser";

    $source_dirs = array(
      40 => 'usao/tne/news/2015/March',
      41 => 'usao/tne/news/2015/April',
      42 => 'usao/tne/news/ACE',
      2 => 'usao/tne/news/2015/February',
      3 => 'usao/tne/news/2015',
      4 => 'usao/tne/news/2015/January',
      5 => 'usao/tne/news/2014/September',
      6 => 'usao/tne/news/2014/March',
      7 => 'usao/tne/news/2014/February',
      8 => 'usao/tne/news/2014/May',
      9 => 'usao/tne/news/2014/November',
      10 => 'usao/tne/news/2014/October',
      11 => 'usao/tne/news/2014/August',
      12 => 'usao/tne/news/2014/April',
      13 => 'usao/tne/news/2014',
      14 => 'usao/tne/news/2014/January',
      15 => 'usao/tne/news/2014/June',
      16 => 'usao/tne/news/2014/December',
      17 => 'usao/tne/news/2014/July',
      18 => 'usao/tne/news/2013/September',
      19 => 'usao/tne/news/2013/March',
      20 => 'usao/tne/news/2013/February',
      21 => 'usao/tne/news/2013/May',
      22 => 'usao/tne/news/2013/November',
      23 => 'usao/tne/news/2013',
      24 => 'usao/tne/news/2013/October',
      25 => 'usao/tne/news/2013/August',
      26 => 'usao/tne/news/2013/April',
      27 => 'usao/tne/news/2013/January',
      28 => 'usao/tne/news/2013/June',
      29 => 'usao/tne/news/2013/December',
      30 => 'usao/tne/news/2013/July',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
