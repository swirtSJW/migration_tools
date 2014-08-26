<?php

/**
 * @file
 * Professional Responsibility Advisory Office PRAO.
 *
 * Defines migration classes for the PRAO section of justice.gov.
 */

/**
 * Migrates .html files from /olp to page nodes.
 *
 * @package doj_migration
 * @subpackage prao
 */
class PraoPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'prao',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Professional Responsibility Advisory Office section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('prao')->sourceMigration('Organization');
  }
}
