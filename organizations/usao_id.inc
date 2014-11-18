<?php

/**
 * @file
 * Defines migration classes for District of Idaho.
 */

/**
 * Migrates .html files from /usao-id to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-id
 */
class UsaoIdPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/id',
      38 => 'usao/id/employment',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Idaho.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('usao-id')
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
 * Migrates files (*.pdf) from /usao-id.
 *
 * @package doj_migration
 * @subpackage usao-id
 */
class UsaoIdFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-id subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/id',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-id/docs';

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
    return "usao-id";
  }
}

class UsaoIdPressReleaseMigration extends DistrictPressReleaseMigration {
  /**
   * {@inheritdoc} 
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-id";
    $arguments['component'] = "District of Idaho (USAO)";

    $source_dirs = array(
      "usao/id/news/2013",
      "usao/id/news/2014",
    );
    parent::__construct($arguments, $source_dirs);
  }
}
