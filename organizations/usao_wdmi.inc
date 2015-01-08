<?php

/**
 * @file
 * Defines migration classes for Western District of Michigan.
 */

/**
 * Migrates .html files from /usao-wdmi to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdmi
 */
class UsaoWdmiPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/miw/district',
      1 => 'usao/miw',
      2 => 'usao/miw/divisions/appellate',
      3 => 'usao/miw/divisions/criminal',
      4 => 'usao/miw/divisions/admin',
      5 => 'usao/miw/divisions/civil',
      6 => 'usao/miw/contacts',
      7 => 'usao/miw/joas',
      10 => 'usao/miw/about',
      11 => 'usao/miw/about/formerusa',
      12 => 'usao/miw/programs',
      13 => 'usao/miw/programs/idTheft',
      14 => 'usao/miw/programs/atac',
      15 => 'usao/miw/programs/victimwitness',
      16 => 'usao/miw/programs/victimwitness/courtmaps',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Western District of Michigan.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wdmi')
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
 * Migrates files (*.pdf) from /usao-wdmi.
 *
 * @package doj_migration
 * @subpackage usao-wdmi
 */
class UsaoWdmiFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/miw subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/miw',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-wdmi/docs';

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
    return "usao-wdmi";
  }
}

class UsaoWdmiPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdmi";
    $arguments['component'] = "USAO - Western District of Michigan";

    $source_dirs = array(
      8 => 'usao/miw/news/2014',
      9 => 'usao/miw/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
