<?php

/**
 * @file
 * Defines migration classes for District of New Jersey.
 */

/**
 * Migrates .html files from /usao-nj to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-nj
 */
class UsaoNjPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      1 => 'usao/nj/images',
      2 => 'usao/nj',
      3 => 'usao/nj/includes',
      4 => 'usao/nj/Press',
      8 => 'usao/nj/nj',
      9 => 'usao/nj/nj/includes',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $arguments['obtainer_methods'] = array(
      'title' => array(
        'findSelector' => array("h1", 1),
        'findH1Any' => array(),
        'findFirstContentSubImageAlt' => array(),
        'findClassBreadcrumbLast' => array(),
        'findClassBreadcrumbMenuContentLast' => array(),
        'findSubBannerAlt' => array(),
      ),
    );

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of New Jersey.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-nj')
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
 * Migrates files (*.pdf) from /usao-nj.
 *
 * @package doj_migration
 * @subpackage usao-nj
 */
class UsaoNjFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/nj subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/nj',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-nj/docs';

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
    return "usao-nj";
  }
}

/**
 * Migrates press releases from /usao-nj.
 *
 * @package doj_migration
 * @subpackage usao-nj
 */
class UsaoNjPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-nj";
    $arguments['component_tid'] = 1956;

    $source_dirs = array(
      5 => 'usao/nj/Press/files/pdffiles/2015',
      6 => 'usao/nj/Press/files/pdffiles/2014',
      7 => 'usao/nj/Press/files/pdffiles/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
