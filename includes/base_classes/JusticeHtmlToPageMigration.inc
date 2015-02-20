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
      'field_page_body' => t('Body'),
    );

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('page');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_page_body', 'body');
    $this->addFieldMapping('field_page_body:format')->defaultValue('wysiwyg');
  }

  /**
   * {@inheritdoc}
   */
  public function postImport() {
    $report_rows = $this->generateReportData();
    parent::postImport($report_rows);
  }

  /**
   * Generates migration class-specific report data.
   *
   * @return array
   *   Rows of data to be passed to drush_print_table for the migration report.
   */
  public function generateReportData() {
    // Count migrated nodes with empty bodies.
    $map_table = $this->map->getMapTable();
    $query = db_query("SELECT COUNT(n.nid) FROM {node} n RIGHT JOIN {$map_table} m ON m.destid1 = n.nid LEFT JOIN {field_data_field_page_body} fdfpb on fdfpb.entity_id = n.nid WHERE TRIM(fdfpb.field_page_body_value) = ''");
    $empty_titles = $query->fetchField();

    $report_rows = array(
      array(t('Missing body'), $empty_titles),
    );

    return $report_rows;
  }
}
