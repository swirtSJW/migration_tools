<?php

/**
 * @file
 * Defines migration classes for Eastern District of Pennsylvania.
 */

/**
 * Migrates .html files from /usao-edpa to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edpa
 */
class UsaoEdpaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/pae/Employment',
      1 => 'usao/pae/Admin Division',
      2 => 'usao/pae/Programs/Weed and Seed',
      3 => 'usao/pae/Programs',
      4 => 'usao/pae/Programs/Outreach_Programs',
      5 => 'usao/pae/Programs/Victim_Witness',
      6 => 'usao/pae',
      7 => 'usao/pae/Images/Documents',
      8 => 'usao/pae/Civil_Division',
      10 => 'usao/pae/News',
      13 => 'usao/pae/News/Newsletters',
      51 => 'usao/pae/images/Documents',
      52 => 'usao/pae/Documents',
      53 => 'usao/pae/Criminal_Division',
      54 => 'usao/pae/nursing',
      55 => 'usao/pae/includes',
      56 => 'usao/pae/Pharma-Device',
      57 => 'usao/pae/Programs/Summer_Internships',
      58 => 'usao/pae/Programs/Project_Safe_Neighborhood',
      59 => 'usao/pae/Programs/Outreach_Programs/Barbershop PSA',
      60 => 'usao/pae/Programs/Victim_Witness/regal',
      61 => 'usao/pae/mortgagefraudtaskforce',
      62 => 'usao/pae/Criminal Division',
      63 => 'usao/pae/Procurement',
      64 => 'usao/pae/Mortgage_Task_Force',
      65 => 'usao/pae/Images/Documents/regal',
      108 => 'usao/pae/News/Pharma-Device',
      174 => 'usao/pae/News/Pr',
      188 => 'usao/pae/images',
      189 => 'usao/pae/images/Documents/regal',
      190 => 'usao/pae/Documents/regal',
    );

    $arguments['source_parser_class'] = "EdpaSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Pennsylvania.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('usao-edpa')
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
 * Migrates files (*.pdf) from /usao-edpa.
 *
 * @package doj_migration
 * @subpackage usao-edpa
 */
class UsaoEdpaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-edpa subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/pae',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-edpa/docs';

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
    return "usao-edpa";
  }
}

class UsaoEdpaPressReleaseMigration extends DistrictPressReleaseMigration {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edpa";
    $arguments['component_tid'] = 2021;

    $source_dirs = array(
      14 => 'usao/pae/News/2014/September',
      15 => 'usao/pae/News/2014/March',
      16 => 'usao/pae/News/2014/February',
      17 => 'usao/pae/News/2014/May',
      18 => 'usao/pae/News/2014/November',
      19 => 'usao/pae/News/2014/October',
      20 => 'usao/pae/News/2014/August',
      21 => 'usao/pae/News/2014/April',
      22 => 'usao/pae/News/2014/January',
      23 => 'usao/pae/News/2014/June',
      24 => 'usao/pae/News/2014/July',
      25 => 'usao/pae/News/2013/September',
      26 => 'usao/pae/News/2013/May',
      27 => 'usao/pae/News/2013/Apr',
      28 => 'usao/pae/News/2013/November',
      29 => 'usao/pae/News/2013/October',
      30 => 'usao/pae/News/2013/August',
      31 => 'usao/pae/News/2013/Jan',
      32 => 'usao/pae/News/2013/Mar',
      33 => 'usao/pae/News/2013/June',
      34 => 'usao/pae/News/2013/Feb',
      35 => 'usao/pae/News/2013/December',
      36 => 'usao/pae/News/2013/July',
      35 => 'usao/pae/News/2014/December',
    );

    $arguments["source_parser_class"] = "EdpaPressSourceParser";

    parent::__construct($arguments, $source_dirs);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      'usao/pae/News/2013/Feb/index-news_2013.htm',
      'usao/pae/News/index-news_2014.htm',
    );

    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->legacy_path, $skip_these)) {
      return FALSE;
    }
  }
}
