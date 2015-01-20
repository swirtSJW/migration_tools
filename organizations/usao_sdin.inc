<?php

/**
 * @file
 * Defines migration classes for Southern District of Indiana.
 */

/**
 * Migrates .html files from /usao-sdin to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdin
 */
class UsaoSdinPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ins',
      1 => 'usao/ins/press_releases',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Southern District of Indiana.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdin')
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
 * Migrates files (*.pdf) from /usao-sdin.
 *
 * @package doj_migration
 * @subpackage usao-sdin
 */
class UsaoSdinFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ins subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ins',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-sdin/docs';

    $options = array('recurse' => FALSE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-sdin";
  }
}

class UsaoSdinPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdin";
    $arguments['component'] = "USAO - Southern District of Indiana";

    $source_dirs = array(
      1 => 'usao/ins/press_releases/Pressrelease14',
      2 => 'usao/ins/press_releases/Pressrelease15',
      4 => 'usao/ins/press_releases/Pressrelease13',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
