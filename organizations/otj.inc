<?php

/**
 * @file
 * OTJ.
 *
 * Defines migration classes for the OTJ section of justice.gov.
 */

/**
 * Migrates .html files from /otj to page nodes.
 *
 * @package doj_migration
 * @subpackage otj
 */
class OtjPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'otj',
    );
    $options = array(
      'recurse' => FALSE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from OTJ section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('otj')->sourceMigration('Organization');
  }
}

/**
 * Migrates binary files from /otj.
 *
 * @package doj_migration
 * @subpackage otj
 */
class OtjFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates binary files from the otj subdirectory.');
    $this->dependencies = array('Organization');

    // Match binary files only.
    $source_dirs = array(
      'otj',
      'otj/pdf',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///otj/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "otj";
  }
}
