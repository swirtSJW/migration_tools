<?php

/**
 * @file
 * Defines migration classes for Western District of Oklahoma.
 */

/**
 * Migrates .html files from /usao-wdok to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdok
 */
class UsaoWdokPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/okw',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Western District of Oklahoma.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('usao-wdok')
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
 * Migrates files (*.pdf) from /usao-wdok.
 *
 * @package doj_migration
 * @subpackage usao-wdok
 */
class UsaoWdokFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-wdok subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/okw',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-wdok/docs';

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
    return "usao-wdok";
  }
}

class UsaoWdokPressReleaseMigration extends DistrictPressReleaseMigration {
  /**
   * {@inheritdoc} 
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdok";
    $arguments['component'] = "Western District of Oklahoma (USAO)";

    $source_dirs = array(
      1 => 'usao/okw/news/2014',
      2 => 'usao/okw/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
