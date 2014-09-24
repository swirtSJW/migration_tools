<?php

/**
 * @file
 * Civil Division civil.
 *
 * Defines migration classes for the civil section of justice.gov.
 */

/**
 * Migrates .html files from /civil to page nodes.
 *
 * @package doj_migration
 * @subpackage civil
 */
class CivilPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $json_dir = '["civil\/ocpl","civil\/ocpl\/cases\/Closed","civil\/ocpl\/cases\/Open","civil\/common\/elecread\/2004","civil\/common\/elecread\/2003","civil\/cases\/tobacco2","civil\/cases\/tobacco2\/_vti_cnf","civil\/cases\/cobell","civil\/cases\/cobell\/docs","civil\/cases\/cobell\/docs\/pdf","civil\/cases\/cobell\/docs\/pdf\/_vti_cnf","civil\/cases\/cobell\/_vti_cnf","civil\/docs_forms","civil\/torts\/reca","civil\/torts\/const","civil\/torts\/const\/reca","civil","civil\/ocl","civil\/oil","civil\/BusSchedule","civil\/ocpl\/cases\/cases\/Fidelity_ATM","civil\/ocpl\/cases\/cases\/AmericanVending","civil\/ocpl\/cases\/cases\/Putnam","civil\/orientation","civil\/www.justice.gov\/opcl","civil\/appellate","civil\/common","civil\/common\/press","civil\/common\/oija","civil\/commercial","civil\/commercial\/oija","civil\/commercial\/financial","civil\/commercial\/fraud","civil\/commercial\/intellectual","civil\/commercial\/national-courts","civil\/commercial\/foreign","civil\/omp","civil\/css","civil\/torts\/cstls","civil\/torts\/aa","civil\/torts","civil\/torts\/etls","civil\/torts\/ftcals","civil\/employment","civil\/fedprog","civil\/oil\/dcs","civil\/oil\/as","civil\/pr"]';

    // Define source directories.
    $source_dirs = drupal_json_decode($json_dir);

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Civil Division section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('civil')->sourceMigration('Organization');
  }
}

/**
 * Migrates .html files from /civil to page nodes.
 *
 * @package doj_migration
 * @subpackage civil
 */
class CivilCaseMigration extends JusticeHtmlToCaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'civil/cpb',
    );
    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates cases from the Civil Division section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('civil')->sourceMigration('Organization');
  }

}

/**
 * Migrates files (*.pdf) from /civil.
 *
 * @package doj_migration
 * @subpackage civil
 */
class CivilFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the civil subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'civil',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///olp/docs';

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
    return "civil";
  }
}
