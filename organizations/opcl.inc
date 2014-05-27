<?php

/**
 * @file
 * Office of Privacy and Civil Liberties OPCL.
 *
 * Defines migration classes for the OPCL section of justice.gov.
 */

/**
 * Migrates .html files from /opcl to page nodes.
 *
 * @package doj_migration
 * @subpackage opcl
 */
class OpclPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'opcl',
      'opcl/privacyactoverview2012',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Privacy and Civil Liberties section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('opcl')->sourceMigration('Organization');
  }
}
