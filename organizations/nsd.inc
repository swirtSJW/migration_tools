<?php
/**
 * @file
 * National Security Division NSD.
 *
 * Defines migration classes for the NSD section of justice.gov.
 */

/**
 * Migrates .html files from /nsd to page nodes.
 *
 * @package doj_migration
 * @subpackage nsd
 */
class NsdPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'nsd',
      'nsd/foia',
      'nsd/foia/foia_library',
      'nsd/foia/mis',
      'nsd/opa/pr/speeches/2010',
      'nsd/opa/pr/speeches/2011',
      'nsd/opa/pr/speeches/2012',
      'nsd/opa/pr/speeches/2013',
      'nsd/opa/pr/testimony/2011',
      'nsd/testimony',
      'nsd/press',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the National Security Division section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('nsd')->sourceMigration('Organization');
  }
}

/**
 * Migrates .html files from /nsd/ovt to page nodes.
 *
 * @package doj_migration
 * @subpackage nsd
 */
class NsdOvtPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'nsd/ovt',
      'nsd/ovtuganda',
      'nsd/ovtalgeria',
      'nsd/ovtkenya',
      'nsd/ovtmumbai',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the National Security Division Office of Justice for Victims of Overseas Terrorism section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('nsd-ovt')->sourceMigration('Organization');
  }
}

/**
 * Migrates .html files from /nsd/newemployee to page nodes.
 *
 * @package doj_migration
 * @subpackage nsd
 */
class NsdNewemployeePageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'nsd/newemployee',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the National Security Division New Employee Orientation section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('nsd-new-employee')->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /nsd.
 *
 * @package doj_migration
 * @subpackage nsd
 */
class NsdFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates files (*.pdf) from the nsd subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      "nsd/docs/2007",
      "nsd/docs/2008",
      "nsd/docs",
      "nsd/foia/foia_library",
      "nsd/testimony",
      "nsd/pdf",
      "nsd/ovt/docs",
      "nsd/ovt/ovt-victim-rights",
      "nsd/ovtalgeria/scripts/foresee",
      "nsd/ovtalgeria/styles",
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///oarm/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "nsd";
  }
}
