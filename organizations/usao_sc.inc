<?php

/**
 * @file
 * Defines migration classes for District of South Carolina.
 */

/**
 * Migrates .html files from /usao-sc to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sc
 */
class UsaoScPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/sc',
      1 => 'usao/sc/divisions',
      2 => 'usao/sc/meetattorney',
      3 => 'usao/sc/jobs',
      4 => 'usao/sc/contact',
      6 => 'usao/sc/about',
      7 => 'usao/sc/programs',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of South Carolina.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sc')
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
 * Migrates files (*.pdf) from /usao-sc.
 *
 * @package doj_migration
 * @subpackage usao-sc
 */
class UsaoScFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/sc subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/sc',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-sc/docs';

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
    return "usao-sc";
  }
}

class UsaoScPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sc";
    $arguments['component'] = "USAO - District of South Carolina";
    $arguments['source_parser_class'] = "ScPressSourceParser";

    $source_dirs = array(
      5 => 'usao/sc/news',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
