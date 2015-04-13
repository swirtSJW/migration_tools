<?php

/**
 * @file
 * ncfs.
 *
 * Defines migration classes for the ncfs section of justice.gov.
 */

/**
 * Migrates .html files from /ncfs to page nodes.
 *
 * @package doj_migration
 * @subpackage ncfs
 */
class NcfsPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'ncfs',
    );
    $options = array(
      'recurse' => FALSE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the NCFS section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('ncfs')->sourceMigration('Organization');
  }
}

/**
 * Migrates binary files from /ncfs.
 *
 * @package doj_migration
 * @subpackage ncfs
 */
class NcfsFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates binary files from the ncfs subdirectory.');
    $this->dependencies = array('Organization');

    // Match binary files only.
    $source_dirs = array(
      'ncfs/docs',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///ncfs/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "ncfs";
  }
}
