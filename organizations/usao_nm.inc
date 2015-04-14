<?php

/**
 * @file
 * Defines migration classes for District of New Mexico.
 */

/**
 * Migrates .html files from /usao-nm to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-nm
 */
class UsaoNmPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/nm',
      1 => 'usao/nm/includes',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of New Mexico.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-nm')
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
 * Migrates files (*.pdf) from /usao-nm.
 *
 * @package doj_migration
 * @subpackage usao-nm
 */
class UsaoNmFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/nm subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/nm',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-nm/docs';

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
    return "usao-nm";
  }
}

class UsaoNmPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-nm";
    $arguments['component_tid'] = 1961;

    $source_dirs = array(
      30 => 'usao/nm/press-releases/2014/Dec',
      31 => 'usao/nm/press-releases/2015/Jan',
      32 => 'usao/nm/press-releases/2015/Feb',
      3 => 'usao/nm/press-releases/2014/Nov',
      4 => 'usao/nm/press-releases/2014/May',
      5 => 'usao/nm/press-releases/2014/Apr',
      6 => 'usao/nm/press-releases/2014/Jul',
      7 => 'usao/nm/press-releases/2014/Jan',
      8 => 'usao/nm/press-releases/2014/Jun',
      9 => 'usao/nm/press-releases/2014/Mar',
      10 => 'usao/nm/press-releases/2014/Feb',
      11 => 'usao/nm/press-releases/2014/Oct',
      12 => 'usao/nm/press-releases/2014/Aug',
      13 => 'usao/nm/press-releases/2014/Sep',
      14 => 'usao/nm/press-releases/2013/feb',
      15 => 'usao/nm/press-releases/2013/jan',
      16 => 'usao/nm/press-releases/2013/mar',
      17 => 'usao/nm/press-releases/2013/dec',
      18 => 'usao/nm/press-releases/2013/may',
      19 => 'usao/nm/press-releases/2013/sep',
      20 => 'usao/nm/press-releases/2013/oct',
      21 => 'usao/nm/press-releases/2013/nov',
      22 => 'usao/nm/press-releases/2013/June',
      23 => 'usao/nm/press-releases/2013',
      24 => 'usao/nm/press-releases/2013/July',
      25 => 'usao/nm/press-releases/2013/apr',
      26 => 'usao/nm/press-releases/2013/aug',
      40 => 'usao/nm/press-releases/APD',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
