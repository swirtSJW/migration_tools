<?php

/**
 * @file
 * Defines migration classes for Northern District of Mississippi.
 */

/**
 * Migrates .html files from /usao-ndms to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ndms
 */
class UsaoNdmsPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/msn',
      1 => 'usao/msn/divisions',
      2 => 'usao/msn/includes',
      3 => 'usao/msn/msn',
      4 => 'usao/msn/disclaimers',
      7 => 'usao/msn/about',
      8 => 'usao/msn/programs',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Northern District of Mississippi.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ndms')
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
 * Migrates files (*.pdf) from /usao-ndms.
 *
 * @package doj_migration
 * @subpackage usao-ndms
 */
class UsaoNdmsFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/msn subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/msn',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-ndms/docs';

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
    return "usao-ndms";
  }
}

class UsaoNdmsPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ndms";
    $arguments['component'] = "USAO - Northern District of Mississippi";

    $source_dirs = array(
      5 => 'usao/msn/news/2014',
      6 => 'usao/msn/news/2013',
      10 => 'usao/msn/news/2015',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
