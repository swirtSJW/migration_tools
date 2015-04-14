<?php

/**
 * @file
 * Defines migration classes for District of District of Columbia.
 */

/**
 * Migrates .html files from /usao-dc to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-dc
 */
class UsaoDcPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/dc',
      1 => 'usao/dc/divisions',
      2 => 'usao/dc/contact_form',
      3 => 'usao/dc/includes',
      4 => 'usao/dc/resources',
      5 => 'usao/dc/Internet_Files',
      89 => 'usao/dc/about',
      90 => 'usao/dc/programs/cp',
      91 => 'usao/dc/programs/lep',
      92 => 'usao/dc/programs/neighborhood',
      93 => 'usao/dc/programs',
      94 => 'usao/dc/programs/employment',
      95 => 'usao/dc/programs/employment/security_forms',
      96 => 'usao/dc/programs/vw',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of District of Columbia.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-dc')
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
 * Migrates files (*.pdf) from /usao-dc.
 *
 * @package doj_migration
 * @subpackage usao-dc
 */
class UsaoDcFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/dc subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/dc',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-dc/docs';

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
    return "usao-dc";
  }
}

/**
 * Migrates press releases from /usao-dc.
 *
 * @package doj_migration
 * @subpackage usao-dc
 */
class UsaoDcPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-dc";
    $arguments['component_tid'] = 1751;
    $arguments['source_parser_class'] = "DcPressSourceParser";

    $source_dirs = array(
      6 => 'usao/dc/Internet_Files/news/2014/feb',
      7 => 'usao/dc/Internet_Files/news/2014/jan',
      8 => 'usao/dc/Internet_Files/news/2014/mar',
      9 => 'usao/dc/Internet_Files/news/2014/may',
      10 => 'usao/dc/Internet_Files/news/2014/oct',
      11 => 'usao/dc/Internet_Files/news/2014/jul',
      12 => 'usao/dc/Internet_Files/news/2014/nov',
      13 => 'usao/dc/Internet_Files/news/2014/jun',
      14 => 'usao/dc/Internet_Files/news/2014/apr',
      15 => 'usao/dc/Internet_Files/news/2013/nov',
      102 => 'usao/dc/news/2015/apr',
      101 => 'usao/dc/news/2015/mar',
      100 => 'usao/dc/news/2015/feb',
      52 => 'usao/dc/news/2015/jan',
      53 => 'usao/dc/news/2014/feb',
      54 => 'usao/dc/news/2014/jan',
      55 => 'usao/dc/news/2014/mar',
      56 => 'usao/dc/news/2014/dec',
      57 => 'usao/dc/news/2014/may',
      58 => 'usao/dc/news/2014/sep',
      59 => 'usao/dc/news/2014/oct',
      60 => 'usao/dc/news/2014/jul',
      61 => 'usao/dc/news/2014/nov',
      62 => 'usao/dc/news/2014/jun',
      63 => 'usao/dc/news/2014/apr',
      64 => 'usao/dc/news/2014/aug',
      65 => 'usao/dc/news/2013/feb',
      66 => 'usao/dc/news/2013/jan',
      67 => 'usao/dc/news/2013/mar',
      68 => 'usao/dc/news/2013/dec',
      69 => 'usao/dc/news/2013/may',
      70 => 'usao/dc/news/2013/sep',
      71 => 'usao/dc/news/2013/oct',
      72 => 'usao/dc/news/2013/jul',
      73 => 'usao/dc/news/2013/nov',
      74 => 'usao/dc/news/2013/jun',
      75 => 'usao/dc/news/2013/apr',
      76 => 'usao/dc/news/2013/aug',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
