<?php

/**
 * @file
 * Defines JusticeHtmlToGalleryMigration class.
 */

/**
 * Class JusticeHtmlToGalleryMigration.
 *
 * Parent class for all migrations from static HTML to gallery content type.
 *
 * @package migration_tools
 */
abstract class JusticeHtmlToGalleryMigration extends JusticeHtmlToNodeMigration {

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
  public function __construct($arguments, $source_dirs, $options = array()) {
    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'field_gallery_body' => t('Body'),
      'field_gallery_date' => t('Date'),
    );

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, NULL, $options);

    // The destination is the gallery content type.
    $this->destination = new MigrateDestinationNode('gallery');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_gallery_body', 'body');
    $this->addFieldMapping('field_gallery_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_gallery_date', 'field_gallery_date');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $row->field_pr_date = $this->sourceParser->getDate();
  }
}
