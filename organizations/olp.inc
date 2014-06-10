<?php

/**
 * @file
 * Office of Legal Policy OLP.
 *
 * Defines migration classes for the OLP section of justice.gov.
 */

/**
 * Migrates .html files from /olp to page nodes.
 *
 * @package doj_migration
 * @subpackage olp
 */
class OlpPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'olp',
      'olp/adr',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Legal Policy section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('olp')->sourceMigration('Organization');
  }
}
