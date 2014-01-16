<?php

/**
 * @file
 * Defines JusticeStaticToPageMigration class.
 */

/**
 * Class JusticeStaticToPageMigration.
 *
 * Parent class for all migrations from static HTML to page content type.
 */
abstract class JusticeHtmlToPageMigration extends JusticeFileSourceMigration {

  /**
   * Define $this->source, $this->map, $this->destination, and simple mappings.
   *
   * @param $group
   *   A migration group returned by  MigrateGroup::getInstance().
   * @param array $source_dirs
   *   A flat array of the source directories containing html files.
   */
  public function __construct($group, $source_dirs) {

    // Match .htm, .html files only.
    $regex = '/.*\.htm(l)?/';

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'page_body' => t('Body')
    );

    // Calling parent constructor will set $this->source.
    parent::__construct($group, $source_fields, $source_dirs, $regex);

    // A map of source HTML filename -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'fileid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('page');

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'uid',
      'title',
    ));

    // Define non-simple field mappings.
    $this->addFieldMapping('page_body', 'body')
      ->arguments(array('format' => 'wysiwyg'));
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Set to admin for now.
    $row->uid = 1;
  }

}
