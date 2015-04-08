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
        'findAnySelectorUntilValid' => array('h1'),
        'findFirstContentSubImageAlt' => array(),
        'findBreadcrumbLastAnchor' => array('.breadcrumb'),
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
      5 => 'usao/nj/Press/files',
      6 => 'usao/nj/Press',
    );
    $options = array('recurse' => FALSE);
    parent::__construct($arguments, $source_dirs, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      'usao/nj/Press/news2006.html',
      'usao/nj/Press/news2007.html',
      'usao/nj/Press/news2008.html',
      'usao/nj/Press/news2009.html',
      'usao/nj/Press/news2010.html',
      'usao/nj/Press/news2011.html',
      'usao/nj/Press/news2012.html',
      'usao/nj/Press/news2013.html',
      'usao/nj/Press/news2014.html',
    );

    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->legacy_path, $skip_these)) {
      return FALSE;
    }

    // Check the year to reject any prior to 2013
    if (!doj_migration_date_after($row->field_pr_date, '12/31/2012')) {
      $message = t('- @fileid  -> Skipped: Too old.', array('@fileid' => $row->fileid));
      watchdog('doj_migration', $message, array('@fileid' => $row->fileid), WATCHDOG_WARNING);
      return FALSE;
    }
  }
}
