<?php

/**
 * @file
 * Defines migration classes for the District of Arizona.
 */

/**
 * Migrates .html files from /usao/az to page nodes in /usao-az.
 *
 * @package doj_migration
 * @subpackage usao-az
 */
class UsaoAzPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/az',
      1 => 'usao/az/victimw',
      2 => 'usao/az/press_releases',
      9 => 'usao/az/reports',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the district of Arizona.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('usao-az')
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
 * Migrates files (*.pdf) from /usao-az.
 *
 * @package doj_migration
 * @subpackage usao-az
 */
class UsaoAzFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-az subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/az/IndianCountry',
      1 => 'usao/az/victimw',
      2 => 'usao/az/press_releases/2013',
      3 => 'usao/az/press_releases/2014',
      8 => 'usao/az/press_releases/2010',
      5 => 'usao/az/reports',
      6 => 'usao/az/reports/Newsletter',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-az/docs';

    $options = array('recurse' => FALSE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-az";
  }
}


/**
 * Migrates .html files from /usao/az to press release nodes in /usao-az.
 *
 * @package doj_migration
 * @subpackage usao-az
 */
class UsaoAzPressReleaseMigration extends DistrictPressReleaseMigration {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-az";
    $arguments['component_tid'] = 1716;
    $arguments['obtainer_methods'] = array(
      'date' => array(
        'findSpanFontSize8',
      ),
    );

    $source_dirs = array(
      3 => 'usao/az/press_releases/2013',
      8 => 'usao/az/press_releases/2014',
      4 => 'usao/az/press_releases/2012',
      5 => 'usao/az/press_releases/2011',
    );

    $arguments['source_parser_class'] = "AzPressSourceParser";

    parent::__construct($arguments, $source_dirs);
  }
}
