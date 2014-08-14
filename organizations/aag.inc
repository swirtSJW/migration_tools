<?php

/**
 * @file
 * Office of Associate Attorney General AAG.
 *
 * Defines migration classes for the AAG section of justice.gov.
 */

/**
 * Migrates .html files from /aag to page nodes.
 *
 * @package doj_migration
 * @subpackage aag
 */
class AagPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'asg',
      'asg/testimony/2009',
      'asg/testimony/2010',
      'asg/testimony',
      'asg/speeches/2008',
      'asg/speeches/2009',
      'asg/speeches/2010',
      'asg/readingroom',

    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Office of the Associate Attorney General.');
    $this->addFieldMapping('og_group_ref')->defaultValue('aag')->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /aag.
 *
 * @package doj_migration
 * @subpackage aag
 */
class AagFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the aag subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'asg',
      'asg/readingroom',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///aag/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "aag";
  }
}
