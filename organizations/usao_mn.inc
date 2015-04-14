<?php

/**
 * @file
 * Defines migration classes for District of Minnesota.
 */

/**
 * Migrates .html files from /usao-mn to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-mn
 */
class UsaoMnPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/mn',
      1 => 'usao/mn/econ/',
    );

    $arguments['source_parser_class'] = "MnPageSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Minnesota.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-mn')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/mn/downloads/untitled.html',
      '/usao/mn/images/untitled.html',
      '/usao/mn/photo_test.html',
      '/usao/mn/photo_test_2.html',
      '/usao/mn/index.html',

    );

    if ((doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) || (!self::isType('', $row))) {
      return FALSE;
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function setProperties($row) {
    $row->content_type = $this->sourceParser->getContentType();
    parent::setProperties($row);
  }
}

/**
 * Migrates files (*.pdf) from /usao-mn.
 *
 * @package doj_migration
 * @subpackage usao-mn
 */
class UsaoMnFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/mn subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      1 => 'usao/mn/petters',
      2 => 'usao/mn/petters/pdfs',
      3 => 'usao/mn/major',
      4 => 'usao/mn/econ',
      5 => 'usao/mn/narc',
      6 => 'usao/mn/downloads',
      7 => 'usao/mn/petters/mp3s',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-mn/docs';

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
    return "usao-mn";
  }
}

/**
 * Migrates press releases from /usao-mn.
 *
 * @package doj_migration
 * @subpackage usao-mn
 */
class UsaoMnPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-mn";
    $arguments['component_tid'] = 1896;
    $arguments['source_parser_class'] = "MnPressSourceParser";

    $source_dirs = array(
      0 => 'usao/mn',
      1 => 'usao/mn/econ/',
    );
    $options = array('recurse' => FALSE);
    parent::__construct($arguments, $source_dirs, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/mn/downloads/untitled.html',
      '/usao/mn/images/untitled.html',
      '/usao/mn/photo_test.html',
      '/usao/mn/photo_test_2.html',
      '/usao/mn/index.html',
      '/usao/mn/news_2011.html',
    );

    if ((doj_migration_skip_file($row->fileid, $skip_these)
        || parent::prepareRow($row) === FALSE)
        || (!self::isType('press_release', $row))
        ) {

      return FALSE;
    }

    if (!doj_migration_date_after($row->field_pr_date, '12/31/2012', FALSE)) {
      $message = '@fileid ------> Dated prior to 2013. Skipped: intentionally.';
      $this->migrationMessage($message, array('@fileid' => $row->fileid), WATCHDOG_WARNING);
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function setProperties($row) {
    $row->content_type = $this->sourceParser->getContentType();
    parent::setProperties($row);
  }
}
