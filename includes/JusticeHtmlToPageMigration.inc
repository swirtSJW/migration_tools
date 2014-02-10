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
abstract class JusticeHtmlToPageMigration extends JusticeHtmlFileMigration {

  /**
   * Define $this->source, $this->map, $this->destination, and simple mappings.
   *
   * @param $group
   *   A migration group returned by  MigrateGroup::getInstance().
   * @param array $source_dirs
   *   A flat array of the source directories containing html files.
   * @param $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   */
  public function __construct($arguments, $source_dirs, $options = array()) {

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'page_body' => t('Body')
    );

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $options);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('page');

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'uid',
      'title',
    ));

    // Define non-simple field mappings.
    $this->addFieldMapping('field_page_body', 'field_page_body');
    $this->addFieldMapping('field_page_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Set to admin for now.
    $row->uid = 1;

    // Create a new SourceParser to handle HTML content.
    $source_parser = new SourceParser($row->url_path, $row->filedata);

    $row->title = $source_parser->getTitle();
    $row->field_page_body = $source_parser->getBody();
  }

}
