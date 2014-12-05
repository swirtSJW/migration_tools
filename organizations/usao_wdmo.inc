<?php

/**
 * @file
 * Defines migration classes for Western District of Missouri.
 */

/**
 * Migrates .html files from /usao-wdmo to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdmo
 */
class UsaoWdmoPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      3 => 'usao/mow',
      8 => 'usao/mow/divisions',
      10 => 'usao/mow/includes',
      11 => 'usao/mow/video/ratigan',
      12 => 'usao/mow/video/bifford',
      14 => 'usao/mow/links',
      15 => 'usao/mow/employment',
      16 => 'usao/mow/about',
      17 => 'usao/mow/programs',
      18 => 'usao/mow/pr',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Western District of Missouri.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wdmo')
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
 * Migrates files (*.pdf) from /usao-wdmo.
 *
 * @package doj_migration
 * @subpackage usao-wdmo
 */
class UsaoWdmoFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/mow subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      "usao/mow",
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-wdmo/docs';

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
    return "usao-wdmo";
  }
}

class UsaoWdmoPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdmo";
    $arguments['component'] = "USAO - Western District of Missouri";

    $source_dirs = array(
      7 => 'usao/mow/news2014',
      13 => 'usao/mow/news2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
