<?php

/**
 * @file
 * Defines migration classes for the Eastern District of Louisiana.
 */


/**
 * Migrates files (*.pdf) from /usao-wdny.
 *
 * @package doj_migration
 * @subpackage usao-wdny
 */
class UsaoEdlaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-edla subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/lae/downloads',
      1 => 'usao/lae/news/2013/downloads',
      4 => 'usao/lae/news/2014/downloads',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-wdny/docs';

    $options = array('recurse' => FALSE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-edla";
  }
}
