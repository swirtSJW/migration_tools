<?php

/**
 * @file
 * Defines migration classes for Northern District of New York.
 */

/**
 * Migrates .html files from /usao-ndny to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ndny
 */
class UsaoNdnyPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/nyn',
      2 => 'usao/nyn/vw',
      3 => 'usao/nyn/vw/cases',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Northern District of New York.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ndny')
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
 * Migrates files (*.pdf) from /usao-ndny.
 *
 * @package doj_migration
 * @subpackage usao-ndny
 */
class UsaoNdnyFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/nyn subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      0 => 'usao/nyn/VictimWitness',
      1 => 'usao/nyn/VictimWitness/Cases/StuioPay',
      2 => 'usao/nyn/VictimWitness/Cases/Kane',
      3 => 'usao/nyn/VictimWitness/Cases/CES',
      4 => 'usao/nyn/Vacancy Announcements',
      6 => 'usao/nyn/vw/cases/StudioPay',
      7 => 'usao/nyn/vw/documents',
      8 => 'usao/nyn/USA Events/Troy Peer Court',
      9 => 'usao/nyn/_notes',
      10 => 'usao/nyn/images/_notes',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-ndny/docs';

    $options = array('recurse' => FALSE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-ndny";
  }
}

class UsaoNdnyPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ndny";
    $arguments['component_tid'] = 1976;

    $source_dirs = array(
      1 => 'usao/nyn/news',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
