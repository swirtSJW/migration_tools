<?php

/**
 * @file
 * Defines JusticeCsvMigration class.
 */

/**
 * Class JusticeCsveMigration.
 *
 * Parent class for all migrations with CSV data source.
 *
 * @package doj_migration
 */
abstract class JusticeCsvMigration extends JusticeBaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $source_file) {
    parent::__construct($arguments);
    ini_set('auto_detect_line_endings', TRUE);

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceCSV($source_file, $this->csvcolumns(), array(), $this->fields());
  }

  /**
   * Returns an array of columns.
   *
   * @return array
   *   A multi dimensional array keyed by number,
   *   e.g., $col[0] = array('machine_name' => 'Title');
   */
  abstract public function csvcolumns();

  /**
   * Returns an array of fields.
   *
   * @return array
   *   An array of field titles, keyed by field machine name.
   */
  abstract public function fields();
}