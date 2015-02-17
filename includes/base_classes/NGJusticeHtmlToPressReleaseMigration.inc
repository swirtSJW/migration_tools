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
abstract class NGJusticeHtmlToPressReleaseMigration extends NGJusticeHtmlToNodeMigration {

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

    if (empty($arguments['source_parser_class'])) {
      $arguments['source_parser_class'] = "NGPressReleaseSourceParser";
    }

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, NULL, $options);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('press_release');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_pr_body', 'body');
    $this->addFieldMapping('field_pr_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_pr_number', 'field_pr_number');
    $this->addFieldMapping('field_pr_component', 'field_pr_component');
    $this->addFieldMapping('field_pr_component:source_type')->defaultValue("tid");
    $this->addFieldMapping('field_pr_subtitle', 'field_pr_subtitle');
    $this->addFieldMapping('field_pr_date', 'field_pr_date');
  }

  /**
   * {@inheritdoc}
   */
  protected function setProperties($row) {
    $row->title = $this->sourceParser->getTitle();

    $row->field_pr_date = $this->sourceParser->getDate();

    // Validate that the date is not in the future.
    $date = $row->field_pr_date;
    $time = strtotime($date);
    if ($time > time()) {
      drush_doj_migration_debug_output("IGNORING: The press release date for {$row->fileid} was in the future");
      return FALSE;
    }

    $row->field_pr_subtitle = $this->sourceParser->getSubtitle();
    $row->field_pr_number = $this->sourceParser->getID();
    $row->body = $this->sourceParser->getBody();
  }
}
