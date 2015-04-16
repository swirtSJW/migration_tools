<?php

/**
 * @file
 * Defines migration classes for Middle District of Louisiana.
 */

/**
 * Migrates .html files from /usao-mdla to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-mdla
 */
class UsaoMdlaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/lam',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Middle District of Louisiana.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-mdla')
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
 * Migrates files (*.pdf) from /usao-mdla.
 *
 * @package doj_migration
 * @subpackage usao-mdla
 */
class UsaoMdlaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/lam subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/lam',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-mdla/docs';

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
    return "usao-mdla";
  }
}

class UsaoMdlaPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-mdla";
    $arguments['component_tid'] = 1861;

    $arguments['obtainer_methods'] = array(
      'subtitle' => array(
        'pluckSelector' => array("h2[align='center']", 1),
      ),
    );

    $source_dirs = array(
      3 => 'usao/lam/news/2014',
      4 => 'usao/lam/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
