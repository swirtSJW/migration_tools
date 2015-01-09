<?php

/**
 * @file
 * Defines migration classes for the Northern District of West Virginia.
 */

/**
 * Migrates .html files from /usao/wvn to page nodes in /usao-ndwv.
 *
 * @package doj_migration
 * @subpackage usao-ndwv
 */
class UsaoNdwvPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/wvn/images',
      1 => 'usao/wvn',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Northern District of West Virginia.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('usao-ndwv')
      ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    module_load_include("inc", "doj_migration", "includes/doj_migration");

    $skip_these = array(
      '',
      'usao/wvn/images/images.html',
      'usao/wvn/images/skyline1.htm',
      'usao/wvn/images/thrift.html',
      'usao/wvn/images/District%20Map.htm',
      'usao/wvn/images/wvseallarge.html',
      'usao/wvn/images/wvseallarge.html',
      'usao/wvn/images/wv.html',
      'usao/wvn/images/wvmages.html',
      'usao/wvn/images/wv3images.html',
      'usao/wvn/images/skyline2.htm',
      'usao/wvn/images/skyline1.htm ',
      'usao/wvn/images/wv2images.html',
      'usao/wvn/images/Northern District of West Virginia1.htm',
      'usao/wvn/images/Northern District of West Virginia2.htm',
      'usao/wvn/images/Northern District of West Virginia3.htm',
      'usao/wvn/images/Northern District of West Virginia4.htm',
      'usao/wvn/images/images.html',
      'usao/wvn/index-savemetoo.html',
      'Copy (2) of index.html',
      'usao/wvn/Untitled-1.html',
      'usao/wvn/meetmgtteam-x.html',
    );

    return doj_migration_check_skip_this($row->legacy_path, $skip_these);

  }
}


/**
 * Migrates files (*.pdf) from /usao-ndwv.
 *
 * @package doj_migration
 * @subpackage usao-ndwv
 */
class UsaoNdwvFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao-ndwv subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    $source_dirs = array(
      0 => 'usao/wvn/images/IslandCleanup',
      1 => 'usao/wvn/images',
      2 => 'usao/wvn/images/Chambers-Youth-Leadership',
      3 => 'usao/wvn',
      4 => 'usao/wvn/news/2013/february',
      5 => 'usao/wvn/news/2013/january',
      6 => 'usao/wvn/news/2013/october',
      7 => 'usao/wvn/news/2013/november',
      8 => 'usao/wvn/news/2013/june',
      9 => 'usao/wvn/news/2013/may',
      10 => 'usao/wvn/news/2013/april',
      11 => 'usao/wvn/news/2013/august',
      12 => 'usao/wvn/news/2013/september',
      13 => 'usao/wvn/news/2013/march',
      32 => 'usao/wvn/news/2014/january',
      33 => 'usao/wvn/news/2014/june',
      37 => 'usao/wvn/news/2014/july',
      35 => 'usao/wvn/images/ProjectFuture/april2013',
      36 => 'usao/wvn/images/ProjectFuture',
      38 => 'usao/wvn/includes',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-az/docs';

    $options = array('recurse' => FALSE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-ndwv";
  }
}


/**
 * Migrates .html files from /usao/wvn to press release nodes in /usao-ndwv.
 *
 * @package doj_migration
 * @subpackage usao-ndwv
 */
class UsaoNdwvPressReleaseMigration extends DistrictPressReleaseMigration {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ndwv";
    $arguments['component'] = "USAO - Northern District of West Virginia";
    $arguments['source_parser_class'] = "NdwvPressSourceParser";
    $arguments['obtainer_methods'] = array(
      'title' => array(
        'findH1First',
        'findIdLayer4P5UptoEmptyBr',
        'findIdLayer4P5ShortEnough',
        'findIdLayer4P5Firstline',
        'findIdLayer4P6ShortEnough',
        'findIdLayer4P7ShortEnough',
      ),
      'body' => array(
        'findClassContentSub',
      ),
      'subtitle' => array(),
      'date' => array(),
      // There are no ids.
      'id' => array(),
    );

    $source_dirs = array(
      2 => 'usao/wvn/news/2013/april',
      18 => 'usao/wvn/news/2013/february',
      19 => 'usao/wvn/news/2013/january',
      20 => 'usao/wvn/news/2013/october',
      21 => 'usao/wvn/news/2013/november',
      22 => 'usao/wvn/news/2013/june',
      23 => 'usao/wvn/news/2013/july',
      24 => 'usao/wvn/news/2013/may',
      25 => 'usao/wvn/news/2013/december',
      26 => 'usao/wvn/news/2013/august',
      27 => 'usao/wvn/news/2013/september',
      28 => 'usao/wvn/news/2013/march',
      53 => 'usao/wvn/news/2014/february',
      54 => 'usao/wvn/news/2014/january',
      55 => 'usao/wvn/news/2014/october',
      56 => 'usao/wvn/news/2014/november',
      57 => 'usao/wvn/news/2014/june',
      58 => 'usao/wvn/news/2014/july',
      59 => 'usao/wvn/news/2014/may',
      60 => 'usao/wvn/news/2014/december',
      61 => 'usao/wvn/news/2014/april',
      62 => 'usao/wvn/news/2014/august',
      63 => 'usao/wvn/news/2014/september',
      64 => 'usao/wvn/news/2014/march',
      65 => 'usao/wvn/news/2015/january',
      66 => 'usao/wvn/news/2015/february',
    );

    parent::__construct($arguments, $source_dirs);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $skip_these = array(
      'usao/wvn/news/2013/january/january2013.html',
      'usao/wvn/news/2013/february/february2013.html',
      'usao/wvn/news/2013/march/march2013.html',
      'usao/wvn/news/2013/april/april2013.html',
      'usao/wvn/news/2013/may/may2013.html',
      'usao/wvn/news/2013/june/june2013.html',
      'usao/wvn/news/2013/july/july2013.html',
      'usao/wvn/news/2013/august/august2013.html',
      'usao/wvn/news/2013/september/september2013.html',
      'usao/wvn/news/2013/october/october2013.html',
      'usao/wvn/news/2013/november/november2013.html',
      'usao/wvn/news/2013/december/december2013.html',
      'usao/wvn/news/2014/january/january2014.html',
      'usao/wvn/news/2014/february/february2014.html',
      'usao/wvn/news/2014/march/march2014.html',
      'usao/wvn/news/2014/april/april2014.html',
      'usao/wvn/news/2014/may/may2014.html',
      'usao/wvn/news/2014/june/june2014.html',
      'usao/wvn/news/2014/july/july2014.html',
      'usao/wvn/news/2014/august/august2014.html',
      'usao/wvn/news/2014/september/september2014.html',
      'usao/wvn/news/2014/october/october2014.html',
      'usao/wvn/news/2014/november/november2014.html',
      'usao/wvn/news/2014/december/december2014.html',
      'usao/wvn/news/2015/january/january2015.html',
      'usao/wvn/news/2015/february/february2015.html',
    );

    return doj_migration_check_skip_this($row->legacy_path, $skip_these);

  }
}
