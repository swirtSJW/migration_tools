<?php

/**
 * @file
 * Office of Attorney Recruitment & Management OARM.
 * 
 * Defines migration classes for the OARM section of justice.gov.
 */

/**
 * Migrates .html files from /oarm to page nodes.
 *
 * @package doj_migration
 * @subpackage oarm
 */
class OARMPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'oarm',
      'oarm/aslrp',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Attorney Recruitment & Management section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('oarm')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Let's clean up the titles from ».
    $title = str_replace("»   » ", "", $row->title);
    $title = trim($title);

    $row->title = $title;
  }
}
