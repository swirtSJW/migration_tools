<?php

/**
 * @file
 * Defines migration classes for Southern District of West Virginia.
 */

/**
 * Migrates .html files from /usao-sdwv to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdwv
 */
class UsaoSdwvPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/wvs',
      1 => 'usao/wvs/includes',
      3 => 'usao/wvs/_notes',
      6 => 'usao/wvs/press_releases',
      65 => 'usao/wvs/images',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Southern District of West Virginia.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdwv')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      'usao/wvs/includes/usao-header.html',
      'usao/wvs/includes/usao-footer.html',
      'usao/wvs/includes/index.html',
      'usao/wvs/includes/newsfeed.html',
      'usao/wvs/includes/usao-rightCol.html',
    );

    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->legacy_path, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-sdwv.
 *
 * @package doj_migration
 * @subpackage usao-sdwv
 */
class UsaoSdwvFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/wvs subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/wvs',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-sdwv/docs';

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
    return "usao-sdwv";
  }
}

class UsaoSdwvPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdwv";
    $arguments['component_tid'] = 2151;
    $arguments['source_parser_class'] = "DistrictPressReleaseSourceParser";
    $arguments['obtainer_methods'] = array(
      'title' => array(
        'pluckClassMultiU' => array(),
        'pluckClassMultiStrongU' => array(),
        'pluckSelector' => array('p[align="center"] > strong > u', 1),
        'pluckSelector' => array("h1", 1),

      ),
      'body' => array(
        'findClassContentSub' => array(),
      ),
      'subtitle' => array(),
      'date' => array(
        'pluckSelector' => array("p[align='center']", 1),
        'pluckProbableDate' => array(),
      ),
      // There are no ids.
      'id' => array(),
    );

    $source_dirs = array(
      4 => 'usao/wvs/press_releases/October2014/attachments',
      5 => 'usao/wvs/press_releases/March2014/attachments',
      11 => 'usao/wvs/press_releases/September2014/attachments',
      15 => 'usao/wvs/press_releases/Feb2014/attachments',
      16 => 'usao/wvs/press_releases/April2014/attachments',
      17 => 'usao/wvs/press_releases/March2013/attachments',
      19 => 'usao/wvs/press_releases/June2014/attachments',
      20 => 'usao/wvs/press_releases/April2013/attachments',
      23 => 'usao/wvs/press_releases/Aug2013/attachments',
      25 => 'usao/wvs/press_releases/Jan2013/attachments',
      26 => 'usao/wvs/press_releases/Oct2013/attachments',
      40 => 'usao/wvs/press_releases/August2014/attachments',
      43 => 'usao/wvs/press_releases/July2013/attachments',
      44 => 'usao/wvs/press_releases/May2013/attachments',
      45 => 'usao/wvs/press_releases/Dec2013/attachments',
      46 => 'usao/wvs/press_releases/Nov2013/attachments',
      50 => 'usao/wvs/press_releases/Feb2013/attachments',
      51 => 'usao/wvs/press_releases/Jan2014/attachments',
      52 => 'usao/wvs/press_releases/Jan2014/attachments/_notes',
      57 => 'usao/wvs/press_releases/July2014/attachments',
      58 => 'usao/wvs/press_releases/Sept2013/attachments',
      59 => 'usao/wvs/press_releases/June2013/attachments',
      60 => 'usao/wvs/press_releases/May2014/attachments',
      63 => 'usao/wvs/press_releases/2014-11-Nov/attachments',
      64 => 'usao/wvs/press_releases/2014-12-Dec/attachments',
      65 => 'usao/wvs/press_releases/2015-01-Jan/attachments',
      66 => 'usao/wvs/press_releases/2015-02-Feb/attachments',
    );
    parent::__construct($arguments, $source_dirs);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      'usao/wvs/press_releases/Jan2013/attachments/Jan2013releases.html',
      'usao/wvs/press_releases/Feb2013/attachments/Feb2013releases.html',
      'usao/wvs/press_releases/March2013/attachments/March2013releases.html',
      'usao/wvs/press_releases/April2013/attachments/April2013releases.html',
      'usao/wvs/press_releases/June2013/attachments/June2013releases.html',
      'usao/wvs/press_releases/July2013/attachments/July2013releases.html',
      'usao/wvs/press_releases/Aug2013/attachments/Aug2013releases.html',
      'usao/wvs/press_releases/Sept2013/attachments/Sept2013releases.html',
      'usao/wvs/press_releases/Oct2013/attachments/Oct2013releases.html',
      'usao/wvs/press_releases/Nov2013/attachments/Nov2013releases.html',
      'usao/wvs/press_releases/Dec2013/attachments/Dec2013releases.html',
      'usao/wvs/press_releases/Jan2014/attachments/_notes/Jan2014releases.html',
      'usao/wvs/press_releases/Jan2014/attachments/Jan2014releases.html',
      'usao/wvs/press_releases/Feb2014/attachments/Feb2014releases.html',
      'usao/wvs/press_releases/March2014/attachments/March2014releases.html',
      'usao/wvs/press_releases/April2014/attachments/April2014releases.html',
      'usao/wvs/press_releases/May2014/attachments/May2014releases.html',
      'usao/wvs/press_releases/June2014/attachments/June2014releases.html',
      'usao/wvs/press_releases/July2014/attachments/July2014releases.html',
      'usao/wvs/press_releases/August2014/attachments/Aug2014releases.html',
      'usao/wvs/press_releases/September2014/attachments/Sept2014releases.html',
      'usao/wvs/press_releases/October2014/attachments/OctoberReleases.html',
      'usao/wvs/press_releases/2014-11-Nov/attachments/NovemberReleases.html',
      'usao/wvs/press_releases/2014-12-Dec/attachments/DecemberReleases.html',
      'usao/wvs/press_releases/2015-01-Jan/attachments/JanuaryReleases.html',
      'usao/wvs/press_releases/2015-02-Feb/attachments/FebruaryReleases.html',
      'usao/wvs/press_releases/Jan2013/attachments/newsfeed.html',
    );

    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->legacy_path, $skip_these)) {
      return FALSE;
    }
  }
}
