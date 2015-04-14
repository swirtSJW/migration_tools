<?php

/**
 * @file
 * Defines migration classes for Eastern District of Oklahoma.
 */

/**
 * Migrates .html files from /usao-edok to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edok
 */
class UsaoEdokPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/oke',
      2 => 'usao/oke/caseupdates',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Oklahoma.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edok')
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
 * Migrates files (*.pdf) from /usao-edok.
 *
 * @package doj_migration
 * @subpackage usao-edok
 */
class UsaoEdokFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/oke subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/oke',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-edok/docs';

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
    return "usao-edok";
  }
}

/**
 * Migrates press releases from /usao-edok.
 *
 * @package doj_migration
 * @subpackage usao-edok
 */
class UsaoEdokPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edok";
    $arguments['component_tid'] = 2001;

    $source_dirs = array(
      3 => 'usao/oke/news/2013',
      4 => 'usao/oke/news/2014',
      5 => 'usao/oke/news/2015',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
