<?php

/**
 * @file
 * Defines migrations for District of Guam & the Northern Mariana Islands.
 */

/**
 * Migrates .html files from /usao-gu to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-gu
 */
class UsaoGuPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/gu',
      1 => 'usao/gu/Community Outreach',
      2 => 'usao/gu/USDOJ US Attorney\'s Office - Districts of Guam & the Northern Mariana Islands_files',
      3 => 'usao/gu/includes',
      4 => 'usao/gu/Programs',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Guam & the Northern Mariana Islands.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('usao-gu')
      ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

  }
}

/**
 * Migrates files (*.pdf) from /usao-gu.
 *
 * @package doj_migration
 * @subpackage usao-gu
 */
class UsaoGuFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-gu subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/gu',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-gu/docs';

    $options = array('recurse' => TRUE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-gu";
  }
}

class UsaoGuPressReleaseMigration extends DistrictPressReleaseMigration {
  /**
   * {@inheritdoc} 
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-gu";
    $arguments['component_tid'] = 1791;

    $source_dirs = array(
      5 => 'usao/gu/news/2014',
      6 => 'usao/gu/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
