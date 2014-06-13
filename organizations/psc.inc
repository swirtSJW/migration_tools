<?php

/**
 * @file
 * PSC.
 *
 * Defines migration classes for the PSC section of justice.gov.
 */

/**
 * Migrates .html files from /psc to page nodes.
 *
 * @package doj_migration
 * @subpackage psc
 */
class PscPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'psc',
      'psc/docs',
      'psc/video',
    );
    $options = array(
      'recurse' => FALSE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from PSC section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('psc')->sourceMigration('Organization');
  }
}

/**
 * Migrates binary files from /psc.
 *
 * @package doj_migration
 * @subpackage psc
 */
class PscFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates binary files from the psc subdirectory.');
    $this->dependencies = array('Organization');

    // Match binary files only.
    $source_dirs = array(
      'psc',
      'psc/archive',
      'psc/archive/image-sourcefiles',
      'psc/docs',
      'psc/video',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///psc/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "psc";
  }
}
