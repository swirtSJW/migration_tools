<?php

/**
 * @file
 * Defines migration classes for District of North Dakota.
 */

/**
 * Migrates .html files from /usao-nd to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-nd
 */
class UsaoNdPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/nd/divisions',
      1 => 'usao/nd/ic',
      2 => 'usao/nd/about',
      3 => 'usao/nd',
      5 => 'usao/nd/programs',
      6 => 'usao/nd/programs/vw',
      7 => 'usao/nd/programs/vw/brochures',
      8 => 'usao/nd/news',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of North Dakota.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-nd')
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
 * Migrates files (*.pdf) from /usao-nd.
 *
 * @package doj_migration
 * @subpackage usao-nd
 */
class UsaoNdFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/nd subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/nd',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-nd/docs';

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
    return "usao-nd";
  }
}

class UsaoNdPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-nd";
    $arguments['component_tid'] = 1941;

    $source_dirs = array(
      9 => 'usao/nd/news/2013',
      10 => 'usao/nd/news/2014',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
