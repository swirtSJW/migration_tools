<?php

/**
 * @file
 * tribal.
 *
 * Defines migration classes for the Tribal section of justice.gov.
 */

/**
 * Migrates .html files from /tribal to page nodes.
 *
 * @package doj_migration
 * @subpackage tribal
 */
class TribalPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'tribal',
      'tribal/images',
      'tribal/doc/fv_tjs/session_1',
      'tribal/doc/fv_tjs/session_2/cd2',
      'tribal/newsletters',
    );
    $options = array(
      'recurse' => FALSE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Tribal section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('tribal')->sourceMigration('Organization');
  }
}

/**
 * Migrates binary files from /tribal.
 *
 * @package doj_migration
 * @subpackage tribal
 */
class TribalFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates binary files from the tribal subdirectory.');
    $this->dependencies = array('Organization');

    // Match binary files only.
    $source_dirs = array(
      'tribal',
      'tribal/ctas2012',
      'tribal/ctas2013',
      'tribal/ctas2014',
      'tribal/docs',
      'tribal/docs/fv_tjs',
      'tribal/docs/fv_tjs/session_9',
      'tribal/docs/fv_tjs/session_10',
      'tribal/docs/fv_tjs/session_11',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///tribal/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "tribal";
  }
}
