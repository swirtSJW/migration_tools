<?php

/**
 * @file
 * Defines migration classes for Eastern District of California.
 */

/**
 * Migrates .html files from /usao-edca to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edca
 */
class UsaoEdcaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/cae/employment/docs',
      1 => 'usao/cae',
      2 => 'usao/cae/hate_crimes',
      3 => 'usao/cae/_changed',
      4 => 'usao/cae/us_attorney',
      5 => 'usao/cae/BackupMain/divisions/civil',
      6 => 'usao/cae/links',
      7 => 'usao/cae/vicitm_witness',
      15 => 'usao/cae/Programs/psn',
      16 => 'usao/cae/Programs/law_enforcement',
      17 => 'usao/cae/Programs',
      18 => 'usao/cae/Programs/Outreach',
      19 => 'usao/cae/Programs/psc',
      20 => 'usao/cae/about_us',
      21 => 'usao/cae/offices/fresno',
      22 => 'usao/cae/offices/sacramento',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of California.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edca')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/cae/google66201ba8283a65e2.html',
      '/usao/cae/index_SP.html',
      '/usao/cae/Archive_sp.html',
      '/usao/cae/district_SP.html',
      '/usao/cae/news_sp.html',
      '/usao/cae/reports_SP.html',
      '/usao/cae/_changed/usao-rightCol.html',
      '/usao/cae/BackupMain/divisions/civil/divisionscivil_SP.htmlusao/cae/_changed/usao-footer.html',
      '/usao/cae/_changed/usao-header.html',
      '/usao/cae/us_attorney/index_SP.html',
      '/usao/cae/BackupMain/divisions/civil/index_SP.html',
      '/usao/cae/links/index_SP.html',
      '/usao/cae/vicitm_witness/VictimsRightsAct_SP.html',
      '/usao/cae/vicitm_witness/index_SP.html',
      '/usao/cae/about_us/index_SP.html',
      '/usao/cae/about_us/about_SP.html',
      '/usao/cae/offices/fresno/index_SP.html',
      '/usao/cae/offices/sacramento/index_SP.html',
    );

    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-edca.
 *
 * @package doj_migration
 * @subpackage usao-edca
 */
class UsaoEdcaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/cae subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/cae',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-edca/docs';

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
    return "usao-edca";
  }
}

/**
 * Migrates press releases from /usao-edca.
 *
 * @package doj_migration
 * @subpackage usao-edca
 */
class UsaoEdcaPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edca";
    $arguments['component_tid'] = 1726;
    $arguments['obtainer_methods'] = array(
      'title' => array(
        'findH1Second',
      ),
    );

    $source_dirs = array(
      27 => 'usao/cae/news/docs/2014/2014_07',
      28 => 'usao/cae/news/docs/2014/2014_03',
      29 => 'usao/cae/news/docs/2014/2014_09',
      30 => 'usao/cae/news/docs/2014/2014_02',
      31 => 'usao/cae/news/docs/2014/2014_12',
      32 => 'usao/cae/news/docs/2014/2014_01',
      33 => 'usao/cae/news/docs/2014/2014_08',
      34 => 'usao/cae/news/docs/2014/2014_11',
      35 => 'usao/cae/news/docs/2014/2014_10',
      36 => 'usao/cae/news/docs/2014/2014_06',
      37 => 'usao/cae/news/docs/2014/2014_05',
      38 => 'usao/cae/news/docs/2014/2014_04',
      39 => 'usao/cae/news/docs/2015/2015_01',
      40 => 'usao/cae/news/docs/2015/2015_02',
      41 => 'usao/cae/news/docs/2015/2015_03',
      42 => 'usao/cae/news/docs/2015/2015_04',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
