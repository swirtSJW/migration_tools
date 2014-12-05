<?php

/**
 * @file
 * Defines migration classes for District of Kansas.
 */

/**
 * Migrates .html files from /usao-ks to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ks
 */
class UsaoKsPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ks',
      1 => 'usao/ks/includes',
      14 => 'usao/ks/PressReleases',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Kansas.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('usao-ks')
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
 * Migrates files (*.pdf) from /usao-ks.
 *
 * @package doj_migration
 * @subpackage usao-ks
 */
class UsaoKsFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-ks subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/ks',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-ks/docs';

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
    return "usao-ks";
  }
}

class UsaoKsPressReleaseMigration extends DistrictPressReleaseMigration {
  /**
   * {@inheritdoc} 
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ks";
    $arguments['component'] = "USAO - District of Kansas";

    $source_dirs = array(
      27 => 'usao/ks/PressReleases/2014',
      45 => 'usao/ks/PressReleases/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
