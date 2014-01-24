<?php

/**
 * @file
 * Defines JusticeCsvMigration class.
 */

/**
 * Class JusticeCsveMigration
 *
 * Parent class for all migrations with CSV data source.
 */
abstract class JusticeCsvMigration extends JusticeBaseMigration {

  public function __construct($arguments, $source_file) {
    parent::__construct($arguments);

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceCSV($source_file, $this->csvcolumns(), array(), $this->fields());
  }

  abstract function csvcolumns();
  abstract function fields();
}
