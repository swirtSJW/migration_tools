<?php

/**
 * @file
 * Defines migration classes for District of Massachusetts.
 */

/**
 * Migrates .html files from /usao-ma to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ma
 */
class UsaoMaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ma',
      1 => 'usao/ma/reportacrime',
      2 => 'usao/ma/childexploitation',
      3 => 'usao/ma/includes',
      4 => 'usao/ma/outreach',
      5 => 'usao/ma/resources',
      6 => 'usao/ma/complaint',
      7 => 'usao/ma/violentcrime',
      8 => 'usao/ma/antiterrorism',
      9 => 'usao/ma/vwa/marathon',
      10 => 'usao/ma/civilrights',
      59 => 'usao/ma/news',
      106 => 'usao/ma/employment',
      107 => 'usao/ma/vwr',
      108 => 'usao/ma/marathon',
      109 => 'usao/ma/training',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Massachusetts.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ma')
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
 * Migrates files (*.pdf) from /usao-ma.
 *
 * @package doj_migration
 * @subpackage usao-ma
 */
class UsaoMaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ma subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ma',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-ma/docs';

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
    return "usao-ma";
  }
}

class UsaoMaPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ma";
    $arguments['component'] = "USAO - District of Massachusetts";

    $source_dirs = array(
      31 => 'usao/ma/news/events/2014/March',
      32 => 'usao/ma/news/events/2013/September',
      33 => 'usao/ma/news/events/2013/May',
      34 => 'usao/ma/news/events/2013/June',
      63 => 'usao/ma/news/2014/September',
      64 => 'usao/ma/news/2014/March',
      65 => 'usao/ma/news/2014/February',
      66 => 'usao/ma/news/2014/May',
      67 => 'usao/ma/news/2014/November',
      68 => 'usao/ma/news/2014/October',
      69 => 'usao/ma/news/2014/August',
      70 => 'usao/ma/news/2014/April',
      71 => 'usao/ma/news/2014/January',
      72 => 'usao/ma/news/2014/June',
      73 => 'usao/ma/news/2014/July',
      75 => 'usao/ma/news/2013',
      76 => 'usao/ma/news/2013/September',
      77 => 'usao/ma/news/2013/March',
      78 => 'usao/ma/news/2013/February',
      79 => 'usao/ma/news/2013/May',
      80 => 'usao/ma/news/2013/November',
      81 => 'usao/ma/news/2013/October',
      82 => 'usao/ma/news/2013/August',
      83 => 'usao/ma/news/2013/April',
      84 => 'usao/ma/news/2013/January',
      85 => 'usao/ma/news/2013/June',
      86 => 'usao/ma/news/2013/December',
      87 => 'usao/ma/news/2013/July',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
