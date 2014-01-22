<?php

/**
 * @file
 * Defines migration classes for importing organization nodes.
 */

class OrganizationsMigration extends JusticeCsvMigration {
  public function __construct() {
    ini_set('auto_detect_line_endings', TRUE);

    // Add this migration to the 'organizations' group.
    $group = MigrateGroup::getInstance('organizations', array());
    $source_file = __DIR__ . '/../sources/organizations.csv';
    parent::__construct($source_file, $group);
    $this->description = t('Migrates organization nodes.');

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'abbreviation' => array('type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Title',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );
    $this->destination = new MigrateDestinationNode('organization');

    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('field_og_abbreviation', 'abbreviation');
  }

  function csvcolumns() {
    // Note: Remember to subtract 1 from column number.
    $columns[0] = array('title', 'Title');
    $columns[1] = array('abbreviation', 'Abbreviation');

    return $columns;
  }

  function fields() {
    return array(
      'Title' => 'The title',
      'URL Prefix' => 'The organization abbreviation',
    );
  }

  function prepareRow($row) {}
}
