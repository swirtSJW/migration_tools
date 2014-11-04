<?php

/**
 * @file
 * Defines migration classes for USAO organization.
 *
 * Migrates US Attorneys District home pages from CSV.
 *
 * @todo Refactor this to pull data from relevant HTML files. This was written
 * as a proof-of-concept for source migration ids, to verify that 1) parent
 * groups could be set for these districts, and 2) staff profiles for each
 * district could be migrated.
 */

/**
 * Class UsaoDistrictMigration.
 *
 * @package doj_migration
 * @subpackage usao
 */
class UsaoDistrictMigration extends JusticeCsvMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    ini_set('auto_detect_line_endings', TRUE);

    $source_file = __DIR__ . '/../sources/usao-districts.csv';
    parent::__construct($arguments, $source_file);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates USAO districts into organizations.');

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'abbreviation' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Title',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );
    $this->destination = new MigrateDestinationNode('organization');

    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('field_org_abbreviation', 'abbreviation');
    $this->addFieldMapping('migrate_redirects', 'legacy_path');
    $this->addFieldMapping('field_org_type')->defaultValue('district');
    $this->addFieldMapping('og_group_ref')->defaultValue('usao')->sourceMigration('Organization');

    $this->addFieldMapping('pathauto')->defaultValue(FALSE);
    $this->addFieldMapping('path', 'path');

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'uid',
      'status',
    ));
  }

  /**
   * {@inheritdoc}
   */
  public function csvcolumns() {
    // Note: Remember to subtract 1 from column number.
    $columns[0] = array('abbreviation', 'Abbreviation');
    $columns[1] = array('title', 'Title');
    $columns[2] = array('attorney', 'Attorney');

    return $columns;
  }

  /**
   * {@inheritdoc}
   */
  public function fields() {
    return array(
      'title' => 'The title',
      'abbreviation' => 'The organization abbreviation',
      'attorney' => 'The District Attorney',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);
    $abbr = $row->abbreviation;
    $row->path = $abbr;
    $short_abbr = str_replace("usao-", "", $abbr);

    $map = array(
      "ndca" => "can",
    );

    if (array_key_exists($short_abbr, $map)) {
      $short_abbr = $map[$short_abbr];
    }

    $row->legacy_path = "usao/districts/{$short_abbr}.html";

    // Set to admin for now.
    $row->uid = 1;
    $row->status = 1;
  }
}

/**
 * Class UsaoStaffProfileMigration.
 *
 * Migrates US Attorneys staff profiles from CSV.
 *
 * @package doj_migration
 * @subpackage usao
 */
class UsaoStaffProfileMigration extends JusticeCsvMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    ini_set('auto_detect_line_endings', TRUE);

    $source_file = __DIR__ . '/../sources/usao-districts.csv';
    parent::__construct($arguments, $source_file);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates USAO District Attorneys.');

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'abbreviation' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Title',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );
    $this->destination = new MigrateDestinationNode('staff_profile');
    $this->addFieldMapping('title', 'attorney');
    $this->addFieldMapping('og_group_ref', 'abbreviation')->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function csvcolumns() {
    // Note: Remember to subtract 1 from column number.
    $columns[0] = array('abbreviation', 'Abbreviation');
    $columns[1] = array('title', 'Title');
    $columns[2] = array('attorney', 'Attorney');

    return $columns;
  }

  /**
   * {@inheritdoc}
   */
  public function fields() {
    return array(
      'title' => 'The title',
      'abbreviation' => 'The organization abbreviation',
      'attorney' => 'The District Attorney',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {}
}


/**
 * Migrates .html files from /usao to page nodes.
 *
 * @package doj_migration
 * @subpackage usao
 */
class UsaoPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/eousa/foia_reading_room/usam/title6',
      1 => 'usao/eousa/foia_reading_room/usam/title5',
      2 => 'usao/eousa/foia_reading_room/usam/title9',
      3 => 'usao/eousa/foia_reading_room/usam/title3',
      4 => 'usao/eousa/foia_reading_room/usam/title2',
      5 => 'usao/eousa/foia_reading_room/usam/title4',
      6 => 'usao/eousa/foia_reading_room/usam/title7',
      7 => 'usao/eousa/foia_reading_room/usam/title1',
      8 => 'usao/eousa/foia_reading_room/usam/title8',
      9 => 'usao/eousa/foia_reading_room/usamndx',
      10 => 'usao/eousa/foia_reading_room/usamndx/archive',
      11 => 'usao/reading_room/reports/asr2005',
      12 => 'usao/reading_room/data',
      13 => 'usao/eousa',
      14 => 'usao/eousa/vr',
      15 => 'usao/eousa/foia_reading_room',
      16 => 'usao/eousa/foia_reading_room/usam',
      17 => 'usao/eousa/palmetto',
      18 => 'usao/contact_form',
      19 => 'usao/justice101',
      20 => 'usao/justice101/steps',
      21 => 'usao/briefing_room/fin',
      22 => 'usao/briefing_room',
      23 => 'usao/briefing_room/dp',
      24 => 'usao/briefing_room/ns',
      25 => 'usao/briefing_room/crt',
      26 => 'usao/briefing_room/ic',
      27 => 'usao/briefing_room/vc',
      28 => 'usao/briefing_room/ab',
      29 => 'usao/briefing_room/cc',
      30 => 'usao/briefing_room/psc',
      31 => 'usao/briefing_room/vw',
      32 => 'usao/resources',
      33 => 'usao/resources/foiarequests',
      34 => 'usao/resources/reports',
      35 => 'usao/resources/foialibrary',
      36 => 'usao/career',
      37 => 'usao/reading_room',
      38 => 'usao/reading_room/reports/asr2003',
      39 => 'usao/reading_room/reports/asr2006',
      40 => 'usao/reading_room/reports/asr2011',
      41 => 'usao/reading_room/reports/asr2007',
      42 => 'usao/reading_room/reports/asr2010',
      43 => 'usao/reading_room/reports/asr2002',
      44 => 'usao/reading_room/reports/asr2004',
      45 => 'usao/reading_room/reports/asr2009',
      46 => 'usao/reading_room/reports/asr2008',
      47 => 'usao/biographies',
      49 => 'usao/about',
      50 => 'usao/training',
      51 => 'usao/training/mii',
      52 => 'usao/training/mii/includes',
      53 => 'usao/training/attendeeinfo',
      54 => 'usao/training/training',
    );

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Executive Office for United States Attorneys.');
    $this->addFieldMapping('og_group_ref')->defaultValue('usao')->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /ola.
 *
 * @package doj_migration
 * @subpackage ola
 */
class UsaoFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the ola subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'usao',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao/docs';

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
    return "usao";
  }
}
