<?php
/**
 * @file
 * Defines class PageMigration.
 */

namespace CustomMigrate\Html;

/**
 * Migrates HTML files to Basic Page nodes.
 */
abstract class PageMigration extends NodeMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct(array $arguments) {
    $this->mergeArguments($arguments);
    // Declare default arguments specific to the page content type.
    $arguments = array(
      'description' => t('Migration of html files to D7 Page content.'),
      'destination_type' => 'page',
      'default_language' => 'en',
      'source_parser' => '\CustomMigrate\SourceParser\Page',
      'uid' => 1,
    );
    $this->mergeArguments($arguments);
    parent::__construct($this->getArguments());

    $this->description = t('Migrates into Page from Html sources.');

    $this->addFieldMapping('og_group_ref')->defaultValue($this->getArgument('og_nid'));

    $this->addFieldMapping('field_basic_page_type')
      ->defaultValue($this->getArgument('field_page_type_default'));

    $fields = array(
      // Destination field => Source field, or just destination field.
      0 => 'created',
      'field_date_updated_reviewed' => 'field_date_updated_reviewed',
    );
    $this->addFieldMappings($fields);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    parent::prepare($entity, $row);

    \MigrationTools\OrganicGroups::setOgToAdmin($entity);
  }
}
