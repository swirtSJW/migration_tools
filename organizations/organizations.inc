<?php

/**
 * @file
 * Defines migration classes for importing organization nodes.
 */

/**
 * Class OrganizationMigration.
 *
 * @package doj_migration
 */
class OrganizationMigration extends JusticeCsvMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    ini_set('auto_detect_line_endings', TRUE);

    $source_file = __DIR__ . '/../sources/organizations.csv';
    parent::__construct($arguments, $source_file);
    $this->description = t('Migrates organization nodes.');

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'abbreviation' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Title',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );
    $this->destination = new MigrateDestinationNode('organization');

    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('field_org_abbreviation', 'abbreviation');
    $this->addFieldMapping('pathauto')->defaultValue(FALSE);
    $this->addFieldMapping('path', 'abbreviation');
  }

  /**
   * {@inheritdoc}
   */
  public function csvcolumns() {
    // Note: Remember to subtract 1 from column number.
    $columns[0] = array('title', 'Title');
    $columns[1] = array('abbreviation', 'Abbreviation');

    return $columns;
  }

  /**
   * {@inheritdoc}
   */
  public function fields() {
    return array(
      'title' => 'The title',
      'abbreviation' => 'The organization abbreviation',
    );
  }
}
