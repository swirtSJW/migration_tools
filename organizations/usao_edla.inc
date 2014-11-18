<?php

/**
 * @file
 * Defines migration classes for the Eastern District of Louisiana.
 */


/**
 * Migrates .html files from /usao/lae to page nodes in /usao-edla.
 *
 * @package doj_migration
 * @subpackage usao-edla
 */
class UsaoEdlaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/lae',
      1 => 'usao/lae/images',
      2 => 'usao/lae/news/2013/index.html',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Louisiana.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('usao-edla')
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
 * Migrates files (*.pdf) from /usao-wdny.
 *
 * @package doj_migration
 * @subpackage usao-wdny
 */
class UsaoEdlaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-edla subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/lae/downloads',
      1 => 'usao/lae/news/2013/downloads',
      4 => 'usao/lae/news/2014/downloads',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-wdny/docs';

    $options = array('recurse' => FALSE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-edla";
  }
}

class UsaoEdlaPressReleaseMigration extends DistrictPressReleaseMigration {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edla";
    $arguments['component'] = "Eastern District of Louisiana (USAO)";

    $source_dirs = array(
      "usao/lae/news/2014",
      "usao/lae/news/2013",
    );
    parent::__construct($arguments, $source_dirs);
  }
}
