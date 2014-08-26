<?php

/**
 * @file
 * Defines JusticeHtmlToPageMigration class.
 */

/**
 * Class JusticeHtmlToPageMigration.
 *
 * Parent class for all migrations from static HTML to page content type.
 *
 * @package doj_migration
 */
abstract class JusticeHtmlToPageMigration extends JusticeHtmlToNodeMigration {

  /**
   * Define $this->source, $this->map, $this->destination, and simple mappings.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   * @param array $source_dirs
   *   A flat array of the source directories containing html files.
   * @param array $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   */
  public function __construct($arguments, $source_dirs, $regex = NULL, $options = array()) {

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'field_page_body' => t('Body'),
    );

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('page');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_page_body', 'field_page_body');
    $this->addFieldMapping('field_page_body:format')->defaultValue('wysiwyg');

  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row, SourceParser $source_parser = NULL) {
    // Create a new SourceParser to handle HTML content.
    if (!$source_parser) {
      $source_parser = new SourceParser($row->url_path, $row->filedata);
    }
    parent::prepareRow($row, $source_parser);

    $row->field_page_body = $source_parser->getBody();
  }
}
