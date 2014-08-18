<?php

/**
 * @file
 * tribal.
 *
 * Defines migration classes for the Espanol section of justice.gov.
 */

/**
 * Migrates .html files from /spanish to page nodes.
 *
 * @package doj_migration
 * @subpackage espanol
 */
class EspanolPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'spanish',
      'spanish/topics',
      'spanish/testimony/2011',
      // Press release: The root of pr contains navigation pages.
      'spanish/pr',
      // Speeches: The root of speeches contain navigation pages.
      'spanish/speeches',
    );
    $options = array(
      'recurse' => FALSE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Spanish section into Espanol.');
    $this->addFieldMapping('og_group_ref')->defaultValue('espanol')->sourceMigration('Organization');
    $this->addFieldMapping('language')->defaultValue('es');
  }
}


/**
 * Migrates binary files from /spanish.
 *
 * @package doj_migration
 * @subpackage espanol
 */
class EspanolFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates binary files from the spanish subdirectory.');
    $this->dependencies = array('Organization');

    // Match binary files only.
    $source_dirs = array(
      'spanish',
      'spanish/pr/2012/December',
      'spanish/pr/2014',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    // Sending them to spanish, the old location, instead of espanol so
    // redirects have a better chance of working.
    $dest_dir = 'public:///espanol/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "espanol";
  }
}
