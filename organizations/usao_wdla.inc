<?php

/**
 * @file
 * Defines migration classes for Western District of Louisiana.
 */

/**
 * Migrates .html files from /usao-wdla to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdla
 */
class UsaoWdlaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/law/law_old/programs/archive',
      1 => 'usao/law',
      2 => 'usao/law/includes',
      3 => 'usao/law/vicwit',
      4 => 'usao/law/law_old',
      5 => 'usao/law/law_old/divisions',
      6 => 'usao/law/law_old/reportacrime',
      7 => 'usao/law/law_old/faq',
      8 => 'usao/law/law_old/lep',
      9 => 'usao/law/law_old/mission',
      10 => 'usao/law/law_old/aboutus',
      11 => 'usao/law/law_old/vicwit',
      12 => 'usao/law/law_old/vicwit/caq',
      13 => 'usao/law/law_old/contact',
      14 => 'usao/law/law_old/courtcalendar',
      15 => 'usao/law/law_old/lecc',
      16 => 'usao/law/law_old/links',
      18 => 'usao/law/law_old/employment',
      19 => 'usao/law/law_old/usattorney',
      20 => 'usao/law/law_old/programs',
      21 => 'usao/law/law_old/programs/grants',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Western District of Louisiana.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wdla')
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
 * Migrates files (*.pdf) from /usao-wdla.
 *
 * @package doj_migration
 * @subpackage usao-wdla
 */
class UsaoWdlaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/law subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/law',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-wdla/docs';

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
    return "usao-wdla";
  }
}

class UsaoWdlaPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdla";
    $arguments['component_tid'] = 1866;

    $arguments['obtainer_methods'] = array(
      'title' => array(
        'findSelector' => array("h1", 1),
        'findAnySelectorUntilValid' => array('h1'),
        'findBreadcrumbLastAnchor' => array('.breadcrumb'),
        'findClassBreadcrumbMenuContentLast' => array(),
        'findSubBannerAlt' => array(),
        'findSelector' => array('table > td center', 1),
      ),
    );

    $source_dirs = array(
      22 => 'usao/law/news',
    );
    parent::__construct($arguments, $source_dirs);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $id = $row->fileid;
    $years = array("2013", "2014");
    $keep = FALSE;

    $pre = "wdla";
    foreach ($years as $year) {
      if (substr_count($id, "{$pre}{$year}") > 0) {
        $keep = TRUE;
        break;
      }
    }

    if (!$keep) {
      return FALSE;
    }

    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}
