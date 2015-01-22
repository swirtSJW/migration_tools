<?php

/**
 * @file
 * Defines migration classes for Tax Division.
 */

/**
 * Migrates .html files from /tax to page nodes.
 *
 * @package doj_migration
 * @subpackage tax
 */
class TaxPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'tax',
      1 => 'tax/DiverseCommittee',
      5 => 'tax/readingroom/jcmanhtm',
      6 => 'tax/readingroom/JCM2004',
      7 => 'tax/readingroom/settlpdf',
      8 => 'tax/readingroom/2008ctm',
      9 => 'tax/readingroom/jcpdf',
      10 => 'tax/readingroom/foia',
      11 => 'tax/readingroom/foia/criminalorders/2013',
      12 => 'tax/readingroom/foia/criminalorders/2014',
      13 => 'tax/readingroom/foia/foia_publications',
      14 => 'tax/readingroom/2001ctm',
      15 => 'tax/readingroom/settlemn_OLD_01',
      16 => 'tax/readingroom/settlemn',
      28 => 'tax/hr_new_employee',
    );

    $arguments['source_parser_class'] = "SourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Tax Division.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('tax')
    ->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /tax.
 *
 * @package doj_migration
 * @subpackage tax
 */
class TaxFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the tax subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'tax',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///tax/docs';

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
    return "tax";
  }
}

/**
 * Migrates press releases from /tax.
 *
 * @package doj_migration
 * @subpackage tax
 */
class TaxPressReleaseMigration extends JusticeHtmlToPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['component'] = "Tax Division";

    $this->addFieldMapping('og_group_ref')
    ->defaultValue('tax')
    ->sourceMigration('Organization');

    $source_dirs = array(
      // 2 => 'tax/2012',
      3 => 'tax/2013',
      4 => 'tax/2014',
      5 => 'tax/2015',
      // 17 => 'tax/prtax',
      // 18 => 'tax/usaopress',
      // 19 => 'tax/usaopress/2007',
      // 20 => 'tax/usaopress/2006',
      // 21 => 'tax/usaopress/2008',
      // 22 => 'tax/usaopress/2002',
      // 23 => 'tax/usaopress/2003',
      // 24 => 'tax/usaopress/2012',
      // 25 => 'tax/usaopress/2004',
      // 26 => 'tax/usaopress/2005',
      // 27 => 'tax/usaopress/2001',
      // 29 => 'tax/componentpress/2009',
      // 30 => 'tax/componentpress/2002',
      // 31 => 'tax/componentpress',
      // 32 => 'tax/usaopress/2009',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
