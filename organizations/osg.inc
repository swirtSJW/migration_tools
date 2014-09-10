<?php

/**
 * @file
 * Office of the Solicitor General OSG.
 *
 * Defines migration classes for the OSG section of justice.gov.
 */

/**
 * Migrates .html files from /osg to page nodes.
 *
 * @package doj_migration
 * @subpackage osg
 */
class OsgPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    $dir_json = '["osg","osg\/aboutosg","osg\/opportunities","osg\/test"]';

    // Define source directories.
    $source_dirs = drupal_json_decode($dir_json);

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Legal Policy section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('osg')->sourceMigration('Organization');
  }

}

/**
 * Migrates files (*.pdf) from /osg.
 *
 * @package doj_migration
 * @subpackage osg
 */
class OsgFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the olp subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'osg',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///osg/docs';

    $options = array('recurse' => FALSE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "osg";
  }
}
