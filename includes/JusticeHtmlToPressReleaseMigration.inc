<?php

/**
 * @file
 * Defines JusticeHtmlToPressReleaseMigration class.
 */

/**
 * Class JusticeHtmlToPressReleaseMigration.
 *
 * Parent class for all migrations from static HTML to page content type.
 *
 * @package doj_migration
 */
abstract class JusticeHtmlToPressReleaseMigration extends JusticeHtmlToNodeMigration {

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
      'field_pr_body' => t('Body'),
    );

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, NULL, $options);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('press_release');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_pr_body', 'body');
    $this->addFieldMapping('field_pr_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_pr_number', 'field_pr_number');
    $this->addFieldMapping('field_pr_component', 'field_pr_component');
    $this->addFieldMapping('field_pr_subtitle', 'field_pr_subtitle');
    $this->addFieldMapping('field_pr_date', 'field_pr_date');

  }
}
