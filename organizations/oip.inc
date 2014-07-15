<?php

/**
 * @file
 * OIP.
 *
 * Defines migration classes for the OIP section of justice.gov.
 */

/**
 * Migrates binary files from /oip.
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates binary files from the oip subdirectory.');
    $this->dependencies = array('Organization');

    // Match binary files only.
    $source_dirs = array(
      'oip/csv',
      'oip/xls',
      'oip/foia-guide14',
      'oip/lc-rpt-2012',
      'oip/docs',
      'oip/foiapost',
      'oip/lc-rpt-2013',
      'oip/foia-guide13',
      'oip/readingrooms',
      'oip/readingrooms',
      'oip/foia_guide09',
      'oip/annual_report/2007',
      'oip/annual_report/2011',
      'oip/annual_report/2010',
      'oip/annual_report/2009',
      'oip/annual_report/2008',
      'oip/annual_report/2013',
      'oip/annual_report/2012',
      'oip/foiacontact',
      'oip/forms',
      'oip/lc-rpt-2011',
      'oip/cy09',
      'oip/cy10',
      'oip/foia_guide07',
      'oip/leg-history',
      'oip',
      'oip/niem',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///oip/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "oip";
  }
}
