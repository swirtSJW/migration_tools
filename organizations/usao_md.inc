<?php

/**
 * @file
 * Defines migration classes for District of Maryland.
 */

/**
 * Migrates .html files from /usao-md to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-md
 */
class UsaoMdPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/md/_mm',
      1 => 'usao/md/_mm/ct3beta/messaging/users/pattysmith2_at_usdoj.gov',
      2 => 'usao/md/_mm/ct3beta/messaging/users',
      3 => 'usao/md/_mm/ct3beta/messaging/users/list',
      4 => 'usao/md/_mm/ct3beta/messaging/users/stephanie.vice_at_usdoj.gov',
      5 => 'usao/md/_mm/ct3beta/messaging/users/stephanie.vice_at_usa.doj.gov',
      6 => 'usao/md/_mm/ct3beta/messaging/users/psmith3_at_usdoj.gov',
      7 => 'usao/md/_mm/ct3beta/messaging/users/patty.smith2_at_usdoj.gov',
      8 => 'usao/md/_mm/ct3beta/WIPMetaData',
      21 => 'usao/md/_baks',
      22 => 'usao/md/MMWIP',
      23 => 'usao/md/MMWIP/601248f2194cb9d8',
      27 => 'usao/md',
      49 => 'usao/md/includes',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Maryland.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-md')
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
 * Migrates files (*.pdf) from /usao-md.
 *
 * @package doj_migration
 * @subpackage usao-md
 */
class UsaoMdFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/md subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/md',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-md/docs';

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
    return "usao-md";
  }
}

class UsaoMdPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-md";
    $arguments['component_tid'] = 1876;

    $arguments['obtainer_methods'] = array(
      'title' => array(
        'findH1Any' => array(),
        'findSelector' => array("#contentstart > div > h2", 2),
        'findSelector' => array("h1", 2),
        'findFirstCenteredAlignPAfterHr' => array(),
        'findSelector' => array(".contentSub > div > p[align='center'] > strong", 1),
        'findSelector' => array(".contentSub > div > div > p > strong", 1),
        'findSelector' => array("#headline", 1),
        'findSelector' => array("p > strong > em", 1),
        'findSelector' => array("#contentstart > div > h2", 1),
      ),
      'date' => array(
        'findTableRow1Col2' => array(),
        'findTableRow1Col1' => array(),
        'findTable2Row2Col2' => array(),
        'findSelector' => array("p[align='center']", 1),
        'findSelector' => array('#contentstart > p', 1),
        'findSelector' => array('.newsRight', 1),
        'findSelector' => array('.BottomLeftContent', 1),
        'findProbableDate' => array(),
        'findStyle1PwithBr' => array(),
      ),
    );

    $source_dirs = array(
      24 => 'usao/md/news/2014/_baks',
      25 => 'usao/md/news/2013',
      26 => 'usao/md/news/2013/_baks',
      50 => 'usao/md/news/2014',
    );

    $arguments['source_parser_class'] = "MdPressSourceParser";

    parent::__construct($arguments, $source_dirs);
  }
}
