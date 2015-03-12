<?php

/**
 * @file
 * Defines migration classes for District of Connecticut.
 */

/**
 * Migrates .html files from /usao-ct to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ct
 */
class UsaoCtPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ct',
      1 => 'usao/ct/Case Updates',
      2 => 'usao/ct/Documents/Report to District',
      3 => 'usao/ct/new homepage wslides',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Connecticut.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ct')
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
 * Migrates files (*.pdf) from /usao-ct.
 *
 * @package doj_migration
 * @subpackage usao-ct
 */
class UsaoCtFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ct subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ct',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-ct/docs';

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
    return "usao-ct";
  }
}

/**
 * Migrates press releases from /usao-ct.
 *
 * @package doj_migration
 * @subpackage usao-ct
 */
class UsaoCtPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ct";
    $arguments['component_tid'] = 1746;
    $arguments['obtainer_methods'] = array(
      'date' => array(
        'findSelector' => array('.newsLeft', 1),
      ),
    );

    $source_dirs = array(
      4 => 'usao/ct/Press2013',
      5 => 'usao/ct/Press2015',
      6 => 'usao/ct/Press2014',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
