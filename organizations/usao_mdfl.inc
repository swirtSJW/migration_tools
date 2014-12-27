<?php

/**
 * @file
 * Defines migration classes for Middle District of Florida.
 */

/**
 * Migrates .html files from /usao-mdfl to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-mdfl
 */
class UsaoMdflPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/flm',
      2 => 'usao/flm/press',
      28 => 'usao/flm/activity',
      29 => 'usao/flm/includes',
      30 => 'usao/flm/programs',
      31 => 'usao/flm/programs/vw',
      32 => 'usao/flm/programs/vw/EMG',
      33 => 'usao/flm/programs/vw/gunter',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Middle District of Florida.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-mdfl')
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
 * Migrates files (*.pdf) from /usao-mdfl.
 *
 * @package doj_migration
 * @subpackage usao-mdfl
 */
class UsaoMdflFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/flm subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/flm',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-mdfl/docs';

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
    return "usao-mdfl";
  }
}

class UsaoMdflPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-mdfl";
    $arguments['component'] = "USAO - Middle District of Florida";

    $source_dirs = array(
      3 => 'usao/flm/press/2014/Nov',
      4 => 'usao/flm/press/2014/May',
      5 => 'usao/flm/press/2014/Apr',
      6 => 'usao/flm/press/2014/Jul',
      7 => 'usao/flm/press/2014/Dec',
      8 => 'usao/flm/press/2014/Jan',
      9 => 'usao/flm/press/2014/Jun',
      10 => 'usao/flm/press/2014/Mar',
      11 => 'usao/flm/press/2014/Feb',
      12 => 'usao/flm/press/2014/Oct',
      13 => 'usao/flm/press/2014/Aug',
      14 => 'usao/flm/press/2014/Sep',
      15 => 'usao/flm/press/2013/feb',
      16 => 'usao/flm/press/2013/july',
      17 => 'usao/flm/press/2013/jan',
      18 => 'usao/flm/press/2013/mar',
      19 => 'usao/flm/press/2013/dec',
      20 => 'usao/flm/press/2013',
      21 => 'usao/flm/press/2013/may',
      22 => 'usao/flm/press/2013/sep',
      23 => 'usao/flm/press/2013/oct',
      24 => 'usao/flm/press/2013/nov',
      25 => 'usao/flm/press/2013/june',
      26 => 'usao/flm/press/2013/apr',
      27 => 'usao/flm/press/2013/aug',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
