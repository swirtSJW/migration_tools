<?php

/**
 * @file
 * Access to Justice Initiative ATJ.
 *
 * Defines migration classes for the ATJ section of justice.gov.
 */

/**
 * Migrates .html files from /atj to page nodes.
 *
 * @package doj_migration
 * @subpackage atj
 */
class AtjPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'atj',
    );
    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Access to Justice Initiative ATJ section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('atj')->sourceMigration('Organization');
  }
}


/**
 * Migrates non-image files from the /atj.
 *
 * @package doj_migration
 * @subpackage atj
 */
class AtjFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the atj subdirectory.');
    $this->dependencies = array('Organization');

    // Match non-image files.
    $source_dirs = array(
      'atj',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx|swf|flv)/';
    $dest_dir = 'public:///atj/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return 'atj';
  }
}
