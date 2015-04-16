<?php

/**
 * @file
 * Defines migration classes for Eastern District of Virginia.
 */

/**
 * Migrates .html files from /usao-edva to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edva
 */
class UsaoEdvaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      1 => 'usao/vae',
      28 => 'usao/vae/vw',
      29 => 'usao/vae/vw/cases',
      30 => 'usao/vae/victimwitness',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";
    $arguments['obtainer_methods'] = array(
      'title' => array(
        'pluckAnySelectorUntilValid' => array('h1'),
        'pluckSelector' => array("h2", 1),
        'findBreadcrumbLastAnchor' => array('.breadcrumb'),
      ),
    );

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Virginia.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edva')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/vae/vw/cases/cat_v_dog.html',
      '/usao/vae/victimwitness/david_maloy.html',
      '/usao/vae/2013_news_index.html',
      'http://www.justice.gov/usao/vae/vw/caseupdates.html',
    );
    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-edva.
 *
 * @package doj_migration
 * @subpackage usao-edva
 */
class UsaoEdvaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/vae subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/vae',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-edva/docs';

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
    return "usao-edva";
  }
}

/**
 * Migrates press releases from /usao-edva.
 *
 * @package doj_migration
 * @subpackage usao-edva
 */
class UsaoEdvaPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edva";
    $arguments['component_tid'] = 2106;
    $arguments['obtainer_methods'] = array(
      'title' => array(
        'pluckAnySelectorUntilValid' => array('h1'),
        'pluckSelector' => array("p[align='center']", 1),
        'pluckSelector' => array("h2", 1),
        'findBreadcrumbLastAnchor' => array('.breadcrumb'),
      ),
      'date' => array(
        'pluckClassNewsRightLast' => array(),
        'pluckSelector' => array('.newsRight', 1),
        'pluckProbableDate' => array(),
      ),
    );

    $source_dirs = array(
      0 => 'usao/vae/news/2014/12',
      2 => 'usao/vae/news/2015/02',
      3 => 'usao/vae/news/2015/1',
      4 => 'usao/vae/news/2015/01',
      5 => 'usao/vae/news/2014/06',
      6 => 'usao/vae/news/2014/02',
      7 => 'usao/vae/news/2014/08',
      8 => 'usao/vae/news/2014/07',
      9 => 'usao/vae/news/2014/05',
      10 => 'usao/vae/news/2014/03',
      11 => 'usao/vae/news/2014/11',
      12 => 'usao/vae/news/2014/04',
      13 => 'usao/vae/news/2014/10',
      14 => 'usao/vae/news/2014/01',
      15 => 'usao/vae/news/2014/09',
      16 => 'usao/vae/news/2013/06',
      17 => 'usao/vae/news/2013/02',
      18 => 'usao/vae/news/2013/08',
      19 => 'usao/vae/news/2013/12',
      20 => 'usao/vae/news/2013/07',
      21 => 'usao/vae/news/2013/05',
      22 => 'usao/vae/news/2013/03',
      23 => 'usao/vae/news/2013/11',
      24 => 'usao/vae/news/2013/04',
      25 => 'usao/vae/news/2013/10',
      26 => 'usao/vae/news/2013/01',
      27 => 'usao/vae/news/2013/09',
      30 => 'usao/vae/news/2015/2',
      31 => 'usao/vae/news/2015/02',
      32 => 'usao/vae/news/2015/3',
      33 => 'usao/vae/news/2015/03',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
