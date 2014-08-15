<?php

/**
 * @file
 * Environmental Justice EJ.
 *
 * Defines migration classes for the EJ section of justice.gov.
 */

/**
 * Migrates .html files from /ej to page nodes.
 *
 * @package doj_migration
 * @subpackage ej
 */
class EjPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'ej',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Environmental Justice section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('ej')->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /olp.
 *
 * @package doj_migration
 * @subpackage ej
 */
class EjFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the ej subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'ej',
      'ej/docs',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///ej/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "ej";
  }
}
