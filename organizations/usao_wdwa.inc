<?php

/**
 * @file
 * Defines migration classes for Western District of Washington.
 */

/**
 * Migrates .html files from /usao-wdwa to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdwa
 */
class UsaoWdwaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/waw',
      1 => 'usao/waw/About',
      2 => 'usao/waw/About/how to get here',
      3 => 'usao/waw/Programs',
      4 => 'usao/waw/Programs/PSC',
      5 => 'usao/waw/Programs/Victim Witness/miracle',
      6 => 'usao/waw/Programs/Victim Witness',
      7 => 'usao/waw/Programs/Victim Witness/parking',
      8 => 'usao/waw/Programs/Victim Witness/berg',
      9 => 'usao/waw/Programs/Victim Witness/victim info',
      10 => 'usao/waw/Programs/Victim Witness/witness info',
      11 => 'usao/waw/Programs/Victim Witness/witt-lowe-griffin',
      13 => 'usao/waw/Jobs',
      14 => 'usao/waw/Jobs/law clerks',
      28 => 'usao/waw/press',
      50 => 'usao/waw/press/categories',
      44 => 'usao/waw/Divisions',
      45 => 'usao/waw/Divisions/flu',
      46 => 'usao/waw/footer links',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Western District of Washington.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wdwa')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      'usao/waw/footer links/legal.html',
      'usao/waw/footer links/privacy.html',
      'usao/waw/footer links/FOIA.html',
      'usao/waw/includes/usao-header.html',
      'usao/waw/includes/newsfeed.html',
      'usao/waw/includes/usao-footer.html',
      'usao/waw/includes/index.html',
      'usao/waw/includes/usao-rightCol.html',
      'usao/waw/indexoriginal.html',
    );

    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->legacy_path, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-wdwa.
 *
 * @package doj_migration
 * @subpackage usao-wdwa
 */
class UsaoWdwaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/waw subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/waw',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-wdwa/docs';

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
    return "usao-wdwa";
  }
}

class UsaoWdwaPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdwa";
    $arguments['component_tid'] = 2131;
    $arguments['obtainer_methods'] = array(
      'title' => array(
        'findH1Second' => array(),
        'findH1FirstCentered' => array(),
        'findH1First' => array(),
        'findH1Any' => array(),
      ),
      'subtitle' => array(
        'findH2FirstCentered' => array(),
        'findH2First' => array(),
      ),
    );

    $source_dirs = array(
      15 => 'usao/waw/press/2013/May',
      16 => 'usao/waw/press/2013',
      17 => 'usao/waw/press/2013/December',
      18 => 'usao/waw/press/2013/Jan',
      19 => 'usao/waw/press/2013/July',
      20 => 'usao/waw/press/2013/March',
      21 => 'usao/waw/press/2013/August',
      22 => 'usao/waw/press/2013/April',
      23 => 'usao/waw/press/2013/Feb',
      24 => 'usao/waw/press/2013/September',
      25 => 'usao/waw/press/2013/June',
      26 => 'usao/waw/press/2013/November',
      27 => 'usao/waw/press/2013/October',
      29 => 'usao/waw/press/2014',
      30 => 'usao/waw/press/2014/May',
      31 => 'usao/waw/press/2014/December',
      32 => 'usao/waw/press/2014/July',
      33 => 'usao/waw/press/2014/March',
      34 => 'usao/waw/press/2014/August',
      35 => 'usao/waw/press/2014/April',
      36 => 'usao/waw/press/2014/September',
      37 => 'usao/waw/press/2014/June',
      38 => 'usao/waw/press/2014/February',
      39 => 'usao/waw/press/2014/November',
      40 => 'usao/waw/press/2014/October',
      41 => 'usao/waw/press/2014/January',
      42 => 'usao/waw/press/2015/February',
      43 => 'usao/waw/press/2015/March',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
