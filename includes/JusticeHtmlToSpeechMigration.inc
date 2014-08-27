<?php

/**
 * @file
 * Defines JusticeHtmlToSpeechMigration class.
 */

/**
 * Class JusticeHtmlToSpeechMigration.
 *
 * Parent class for all migrations from static HTML to speech content type.
 *
 * @package doj_migration
 */
abstract class JusticeHtmlToSpeechMigration extends JusticeHtmlToNodeMigration {

  /**
   * Define $this->source, $this->map, $this->destination, and simple mappings.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   * @param array $source_dirs
   *   A flat array of the source directories containing html files.
   * @param string $regex
   *   The file mask. Only filenames matching this regex will be migrated.
   *   This will default to '/.*\.htm(l)?$/' in JusticeHtmlToNodeMigration
   *   if NULL is passed in.
   * @param array $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   */
  public function __construct($arguments, $source_dirs, $regex = NULL, $options = array()) {

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'field_speech_body' => t('Body'),
    );

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('speech');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_speech_date', 'field_speech_date');
    $this->addFieldMapping('field_speech_body', 'body');
    $this->addFieldMapping('field_speech_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_speech_location', 'country')->defaultValue('US');
    $this->addFieldMapping('field_speech_location:locality', 'city');
    $this->addFieldMapping('field_speech_location:administrative_area', 'state');
    $this->addFieldMapping('field_speech_image', 'field_speech_image');
    $this->addFieldMapping('field_speech_attachment', 'field_speech_attachment');
    $this->addFieldMapping('field_speech_topic', 'field_speech_topic');
    $this->addFieldMapping('field_speech_component', 'field_speech_component');
  }
}
