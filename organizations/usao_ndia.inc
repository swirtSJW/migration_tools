<?php

/**
 * @file
 * Defines migration classes for Northern District of Iowa.
 */

/**
 * Migrates .html files from /usao-ndia to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ndia
 */
class UsaoNdiaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ian',
      1 => 'usao/ian/includes',
      69 => 'usao/ian/vw/form/vw',
      70 => 'usao/ian/vw',
      71 => 'usao/ian/vw/cases',
      72 => 'usao/ian/htrt',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Northern District of Iowa.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ndia')
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
 * Migrates files (*.pdf) from /usao-ndia.
 *
 * @package doj_migration
 * @subpackage usao-ndia
 */
class UsaoNdiaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ian subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ian',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-ndia/docs';

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
    return "usao-ndia";
  }
}

/**
 * Migrates press releases from /usao-ndia.
 *
 * @package doj_migration
 * @subpackage usao-ndia
 */
class UsaoNdiaPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ndia";
    $arguments['component_tid'] = 1801;
    $arguments['source_parser_class'] = "NdiaPressSourceParser";

    $source_dirs = array(
      60 => 'usao/ian/news/2015/feb_15',
      61 => 'usao/ian/news/2015/mar_15',
      62 => 'usao/ian/news/2015/apr_15',
      28 => 'usao/ian/news/2015/jan_15',
      29 => 'usao/ian/news/2014',
      30 => 'usao/ian/news/2014/jul_14',
      31 => 'usao/ian/news/2014/aug_14',
      32 => 'usao/ian/news/2014/feb_14',
      33 => 'usao/ian/news/2014/jun_14',
      34 => 'usao/ian/news/2014/dec_14',
      35 => 'usao/ian/news/2014/oct_14',
      36 => 'usao/ian/news/2014/sept_14',
      37 => 'usao/ian/news/2014/mar_14',
      38 => 'usao/ian/news/2014/jan_14',
      39 => 'usao/ian/news/2014/apr_14',
      40 => 'usao/ian/news/2014/nov_14',
      41 => 'usao/ian/news/2014/may_14',
      42 => 'usao/ian/news/2013/sep_13',
      43 => 'usao/ian/news/2013',
      44 => 'usao/ian/news/2013/dec_13',
      45 => 'usao/ian/news/2013/apr_13',
      46 => 'usao/ian/news/2013/feb_13',
      47 => 'usao/ian/news/2013/mar_13',
      48 => 'usao/ian/news/2013/may13',
      49 => 'usao/ian/news/2013/nov_13',
      50 => 'usao/ian/news/2013/jul_13',
      51 => 'usao/ian/news/2013/jan_13',
      52 => 'usao/ian/news/2013/jun_13',
      53 => 'usao/ian/news/2013/aug_13',
      54 => 'usao/ian/news/2013/oct_13',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
