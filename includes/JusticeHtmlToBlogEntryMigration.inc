<?php

/**
 * @file
 * Defines JusticeStaticToPageMigration class.
 */

/**
 * Class JusticeStaticToPageMigration.
 *
 * Parent class for all migrations from static HTML to blog_entry content type.
 *
 * @package doj_migration
 */
abstract class JusticeHtmlToBlogEntryMigration extends JusticeHtmlToNodeMigration {

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
      'field_blog_body' => t('Body'),
      'field_blog_date' => t('Date'),
    );

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

    // The destination is the blog_entry content type.
    $this->destination = new MigrateDestinationNode('blog_entry');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_blog_body', 'field_blog_body');
    $this->addFieldMapping('field_blog_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_blog_date', 'field_blog_date');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row, SourceParser $source_parser = NULL) {
    if (!$source_parser) {
      // Create a new SourceParser to handle HTML content.
      $source_parser = new SourceParser($row->url_path, $row->filedata);
    }
    parent::prepareRow($row, $source_parser);

    $row->field_blog_body = $source_parser->getBody();
  }
}
