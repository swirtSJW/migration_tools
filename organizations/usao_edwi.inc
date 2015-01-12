<?php

/**
 * @file
 * Defines migration classes for Eastern District of Wisconsin.
 */

/**
 * Migrates .html files from /usao-edwi to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edwi
 */
class UsaoEdwiPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      1 => 'usao/wie',
      2 => 'usao/wie/events',
      3 => 'usao/wie/includes',
      4 => 'usao/wie/Programs',
      5 => 'usao/wie/vicwit',
      10 => 'usao/wie/Interns',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Wisconsin.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edwi')
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
 * Migrates files (*.pdf) from /usao-edwi.
 *
 * @package doj_migration
 * @subpackage usao-edwi
 */
class UsaoEdwiFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/wie subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/wie',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-edwi/docs';

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
    return "usao-edwi";
  }
}

class UsaoEdwiPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edwi";
    $arguments['component'] = "USAO - Eastern District of Wisconsin";

    $source_dirs = array(
      7 => 'usao/wie/news/2015',
      8 => 'usao/wie/news/2014',
      9 => 'usao/wie/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
