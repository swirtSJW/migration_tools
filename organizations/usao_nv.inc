<?php

/**
 * @file
 * Defines migration classes for District of Nevada.
 */

/**
 * Migrates .html files from /usao-nv to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-nv
 */
class UsaoNvPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/nv',
      1 => 'usao/nv/includes',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Nevada.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-nv')
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
 * Migrates files (*.pdf) from /usao-nv.
 *
 * @package doj_migration
 * @subpackage usao-nv
 */
class UsaoNvFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/nv subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/nv',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-nv/docs';

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
    return "usao-nv";
  }
}

class UsaoNvPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-nv";
    $arguments['component_tid'] = 1966;
    $arguments['source_parser_class'] = "NvPressSourceParser";

    $source_dirs = array(
      2 => 'usao/nv/Justice/news/2014',
      3 => 'usao/nv/news/2014',
      4 => 'usao/nv/news/2013',
      10 => 'usao/nv/news/2015',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
