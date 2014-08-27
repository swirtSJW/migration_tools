<?php

/**
 * @file
 * Department of Justice.
 *
 * Defines migration classes for the DOJ section of justice.gov.
 */

/**
 * Migrates .html files from /crs to page nodes.
 *
 * @package doj_migration
 * @subpackage doj
 */
class DojPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(

    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from main Department of Justice section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('doj')->sourceMigration('Organization');
  }
}


/**
 * Migrates non-image files from the doj content.
 *
 * @package doj_migration
 * @subpackage doj
 */
class DojFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the doj (remaining sections of version4 tarball).');
    $this->dependencies = array('Organization');

    // Match non-image files.
    $source_dirs = array(

    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx|swf|flv)/';
    $dest_dir = 'public:///doj/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return 'doj';
  }
}
