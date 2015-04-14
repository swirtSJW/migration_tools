<?php

/**
 * @file
 * Office of Privacy and Civil Liberties OPCL.
 *
 * Defines migration classes for the OPCL section of justice.gov.
 */

/**
 * Migrates .html files from /opcl to page nodes.
 *
 * @package doj_migration
 * @subpackage opcl
 */
class OpclPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'opcl',
      'opcl/privacyactoverview2012',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Privacy and Civil Liberties section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('opcl')->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /oarm.
 *
 * @package doj_migration
 * @subpackage opcl
 */
class OpclFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates files (*.pdf) from the opcl subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'opcl',
      'opcl/docs',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///opcl/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "opcl";
  }
}
