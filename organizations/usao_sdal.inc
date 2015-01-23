<?php

/**
 * @file
 * Defines migration classes for Southern District of Alabama.
 */

/**
 * Migrates .html files from /usao-sdal to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdal
 */
class UsaoSdalPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/als',
      1 => 'usao/als/rei',
      2 => 'usao/als/jobs',
      3 => 'usao/als/news',
      6 => 'usao/als/vw',
      7 => 'usao/als/civil',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Southern District of Alabama.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdal')
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
 * Migrates files (*.pdf) from /usao-sdal.
 *
 * @package doj_migration
 * @subpackage usao-sdal
 */
class UsaoSdalFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/als subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/als',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-sdal/docs';

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
    return "usao-sdal";
  }
}

class UsaoSdalPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdal";
    $arguments['component_tid'] = 1701;

    $source_dirs = array(
      4 => 'usao/als/news/2014',
      5 => 'usao/als/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
