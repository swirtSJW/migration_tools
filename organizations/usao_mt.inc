<?php

/**
 * @file
 * Defines migration classes for District of Montana.
 */

/**
 * Migrates .html files from /usao-mt to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-mt
 */
class UsaoMtPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/mt',
      1 => 'usao/mt/includes',
      14 => 'usao/mt/PressReleases',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Montana.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('usao-mt')
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
 * Migrates files (*.pdf) from /usao-mt.
 *
 * @package doj_migration
 * @subpackage usao-mt
 */
class UsaoMtFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-mt subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/mt',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-mt/docs';

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
    return "usao-mt";
  }
}

class UsaoMtPressReleaseMigration extends DistrictPressReleaseMigration {
  /**
   * {@inheritdoc} 
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-mt";
    $arguments['component'] = "District of Montana (USAO)";

    $source_dirs = array(
      2 => 'usao/mt/pressreleases',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
