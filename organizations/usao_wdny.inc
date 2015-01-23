<?php

/**
 * @file
 * Defines migration classes for the Western District of New York.
 */

/**
 * Migrates .html files from /usao/nyw to page nodes in /usao-wdny.
 *
 * @package doj_migration
 * @subpackage usao-wdny
 */
class UsaoWdnyPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/nyw',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Western District of New York.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('usao-wdny')
      ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      'usao/nyw/usao-header.html',
      'usao/nyw/usao-rightCol.html',
      'usao/nyw/usao-footer.html',
    );

    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->legacy_path, $skip_these)) {
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
class UsaoWdnyFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-wdny subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/nyw',
      1 => 'usao/nyw/nyw_document',
      2 => 'usao/nyw/lecc',
      3 => 'usao/nyw/press/press_releases',
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
    return "usao-wdny";
  }
}

class UsaoWdnyPressReleaseMigration extends DistrictPressReleaseMigration {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['source_parser_class'] = "DistrictPressReleaseSourceParser";
    $arguments['district'] = "usao-wdny";
    $arguments['component_tid'] = 1986;
    $arguments['obtainer_methods'] = array(
      'title' => array(
        'findH1First',
        'findSubBannerAlt',
        'findLastBreadcrumb',
      ),
      'subtitle' => array(),
      'date' => array(),
      'id' => array(),
    );

    $source_dirs = array(
      "usao/nyw/press/press_releases/2014",
      "usao/nyw/press/press_releases/2013",
    );
    parent::__construct($arguments, $source_dirs);
  }
}
