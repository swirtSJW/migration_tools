<?php

/**
 * @file
 * Office of Small and Disadvantaged Business Utilization OSDBU.
 *
 * Defines migration classes for the OSDBU section of justice.gov.
 */

/**
 * Migrates .html files from /osdbu to page nodes.
 *
 * @package doj_migration
 * @subpackage osdbu
 */
class OsdbuPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'jmd/osdbu',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Small and Disadvantaged Business Utilization');
    $this->addFieldMapping('og_group_ref')->defaultValue('osdbu')->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /osdbu.
 *
 * @package doj_migration
 * @subpackage osdbu
 */
class OsdbuFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the osdbu subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'jmd/osdbu',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///osdbu/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "osdbu";
  }
}
