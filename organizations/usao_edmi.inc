<?php

/**
 * @file
 * Defines migration classes for Eastern District of Michigan.
 */

/**
 * Migrates .html files from /usao-edmi to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edmi
 */
class UsaoEdmiPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/mie',
      1 => 'usao/mie/slideshow',
      2 => 'usao/mie/includes',
      3 => 'usao/mie/meet_us_att/district',
      4 => 'usao/mie/meet_us_att',
      5 => 'usao/mie/meet_us_att/senior_mgt',
      6 => 'usao/mie/jobs',
      7 => 'usao/mie/mie',
      8 => 'usao/mie/mie/includes',
      9 => 'usao/mie/mie/jobs',
      10 => 'usao/mie/mie/news',
      11 => 'usao/mie/manage',
      12 => 'usao/mie/contact',
      13 => 'usao/mie/video',
      14 => 'usao/mie/disclaimers',
      15 => 'usao/mie/usao/mie/meet_us_att/district',
      16 => 'usao/mie/news',
      20 => 'usao/mie/about',
      21 => 'usao/mie/programs',
      22 => 'usao/mie/programs/victim_witness',
    );

    $arguments['source_parser_class'] = "NGDistrictPageSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Michigan.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edmi')
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
 * Migrates files (*.pdf) from /usao-edmi.
 *
 * @package doj_migration
 * @subpackage usao-edmi
 */
class UsaoEdmiFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/mie subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/mie',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-edmi/docs';

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
    return "usao-edmi";
  }
}

/**
 * Migrates press releases from /usao-edmi.
 *
 * @package doj_migration
 * @subpackage usao-edmi
 */
class UsaoEdmiPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edmi";
    $arguments['component_tid'] = 1886;

    $source_dirs = array(
      17 => 'usao/mie/news/2015',
      18 => 'usao/mie/news/2014',
      19 => 'usao/mie/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
