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
      'nsd/ovt',
      'nsd/opa/pr/speeches/2010',
      'nsd/opa/pr/speeches/2011',
      'nsd/opa/pr/speeches/2012',
      'nsd/opa/pr/speeches/2013',
      'nsd/opa/pr/testimony/2011',
      'nsd/ovtuganda',
      'nsd/newemployee',
      'nsd/testimony',
      'nsd/ovtalgeria',
      'nsd/ovtkenya',
      'nsd/ovtmumbai',
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

