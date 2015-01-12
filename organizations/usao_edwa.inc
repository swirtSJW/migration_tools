<?php

/**
 * @file
 * Defines migration classes for Eastern District of Washington.
 */

/**
 * Migrates .html files from /usao-edwa to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edwa
 */
class UsaoEdwaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/wae',
      1 => 'usao/wae/divisions',
      2 => 'usao/wae/disclaimers',
      4 => 'usao/wae/news',
      9 => 'usao/wae/about',
      10 => 'usao/wae/programs',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Washington.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edwa')
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
 * Migrates files (*.pdf) from /usao-edwa.
 *
 * @package doj_migration
 * @subpackage usao-edwa
 */
class UsaoEdwaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/wae subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/wae',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-edwa/docs';

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
    return "usao-edwa";
  }
}

class UsaoEdwaPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edwa";
    $arguments['component'] = "USAO - Eastern District of Washington";

    $source_dirs = array(
      10 => 'usao/wae/news/2015',
      6 => 'usao/wae/news/2014',
      7 => 'usao/wae/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
