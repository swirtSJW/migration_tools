<?php

/**
 * @file
 * INTERPOL.
 *
 * Defines migration classes for the INTERPOL section of justice.gov.
 */

/**
 * Migrates .html files from /interpol-washington to page nodes.
 *
 * @package doj_migration
 * @subpackage interpol
 */
class InterpolPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'interpol-washington',
      'interpol-washington/news',
    );
    // Assemble a regular expression of files to ignore.
    $nomask = '|usncb-rightcol\\.html';
    $options = array(
      'recurse' => FALSE,
      'nomask' => '/(\.\.?|CVS' . $nomask . ')$/',
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from INTERPOL section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('interpol-washington')->sourceMigration('Organization');
  }

}
