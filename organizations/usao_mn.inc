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
    );

    $arguments['source_parser_class'] = "MnPageSourceParser";

    $options = array('recurse' => TRUE);

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
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    if ($row->content_type != "page") {
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

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
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
    );
    parent::__construct($arguments, $source_dirs);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    return FALSE;

    /*if (parent::prepareRow($row) === FALSE) {
    return FALSE;
    }
    if ($row->content_type != "press_release") {
    return FALSE;
    }*/
  }

  /**
   * {@inheritdoc}
   */
  protected function setProperties($row) {
    $row->content_type = $this->sourceParser->getContentType();
    parent::setProperties($row);
  }
}
