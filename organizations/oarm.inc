<?php

/**
 * @file
 * Office of Attorney Recruitment & Management OARM.
 * 
 * Defines migration classes for the OARM section of justice.gov.
 */

/**
 * Migrates .html files from /oarm to page nodes.
 *
 * @package doj_migration
 * @subpackage oarm
 */
class OARMPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'oarm',
      'oarm/aslrp',
      'oarm/wb',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Attorney Recruitment & Management section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('oarm')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    $title = $row->title;
    // Let's clean up the titles from » and the weird blank char also.
    $count = 1;
    while ($count > 0) {
      $title = str_replace("»", "", $title, $count);
    }
    $count = 1;
    while ($count > 0) {
      $title = str_replace(" ", "", $title, $count);
    }

    $row->title = $title;
  }
}

/**
 * Migrates files (*.pdf) from /oarm.
 *
 * @package doj_migration
 * @subpackage oarm
 */
class OARMFileMigration extends JusticeBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates files (*.pdf) from the oarm subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'oarm',
      'oarm/aslrp',
      'oarm/wb',
    );
    $regex = '/.*\.pdf/';
    $dest_dir = 'public://oarm/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }
}
