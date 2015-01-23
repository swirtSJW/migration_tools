<?php

/**
 * @file
 * Defines migration classes for Northern District of Florida.
 */

/**
 * Migrates .html files from /usao-ndfl to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ndfl
 */
class UsaoNdflPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/fln',
      26 => 'usao/fln/divisions',
      27 => 'usao/fln/includes',
      28 => 'usao/fln/programs',
      29 => 'usao/fln/programs/VW',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Northern District of Florida.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ndfl')
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
 * Migrates files (*.pdf) from /usao-ndfl.
 *
 * @package doj_migration
 * @subpackage usao-ndfl
 */
class UsaoNdflFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/fln subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/fln',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-ndfl/docs';

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
    return "usao-ndfl";
  }
}

class UsaoNdflPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ndfl";
    $arguments['component_tid'] = 1766;

    $source_dirs = array(
      1 => 'usao/fln/press/2014/feb',
      2 => 'usao/fln/press/2014/jan',
      3 => 'usao/fln/press/2014/mar',
      4 => 'usao/fln/press/2014/dec',
      5 => 'usao/fln/press/2014/sept',
      6 => 'usao/fln/press/2014/may',
      7 => 'usao/fln/press/2014/oct',
      8 => 'usao/fln/press/2014/jul',
      9 => 'usao/fln/press/2014/nov',
      26 => 'usao/fln/press/2014/dec',
      10 => 'usao/fln/press/2014/jun',
      11 => 'usao/fln/press/2014/apr',
      12 => 'usao/fln/press/2014/aug',
      13 => 'usao/fln/press/2013/feb',
      14 => 'usao/fln/press/2013/july',
      15 => 'usao/fln/press/2013/jan',
      16 => 'usao/fln/press/2013/mar',
      17 => 'usao/fln/press/2013/dec',
      18 => 'usao/fln/press/2013/may',
      19 => 'usao/fln/press/2013/sep',
      20 => 'usao/fln/press/2013/oct',
      21 => 'usao/fln/press/2013/nov',
      22 => 'usao/fln/press/2013',
      23 => 'usao/fln/press/2013/jun',
      24 => 'usao/fln/press/2013/apr',
      25 => 'usao/fln/press/2013/aug',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
