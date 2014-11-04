<?php

/**
 * @file
 * Defines migration classes for Northern District of California.
 */

/**
 * Migrates .html files from /usao-ndca to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ndca
 */
class UsaoNdcaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao-ndca',
      1 => 'usao-ndca/divisions',
      2 => 'usao-ndca/notifications',
      3 => 'usao-ndca/news',
      7 => 'usao-ndca/about',
      8 => 'usao-ndca/programs',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Northern District of California.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('usao-ndca')
      ->sourceMigration('UsaoDistrict');
  }
}

/**
 * Migrates files (*.pdf) from /usao-ndca.
 *
 * @package doj_migration
 * @subpackage usao-ndca
 */
class UsaoNdcaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-ndca subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao-ndca/documents',
      1 => 'usao-ndca/notifications/docs',
      2 => 'usao-ndca/news/2014/docs',
      3 => 'usao-ndca/news/2013/docs',
      5 => 'usao-ndca/programs/docs',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-ndca/docs';

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
    return "usao-ndca";
  }
}
