<?php
/**
 * @file
 * Defines NGDistrictSpeechMigration class.
 */

/**
 * Class NGDistrictSpeechMigration.
 *
 * Parent class for all migrations from static HTML to speech
 * content type.
 *
 * @package doj_migration
 */
abstract class NGDistrictSpeechMigration extends NGJusticeHtmlToSpeechMigration {

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
   * @param string $regex
   *   Regular expression to guide what is processed by the migration.
   */
  public function __construct($arguments, $source_dirs, $options = array(), $regex = NULL) {

    if (array_key_exists('district', $arguments)) {
      $district = $arguments['district'];
    }
    else {
      throw new Exception("NGDistrictSpeechMigration requires the district argument.");
    }

    if (array_key_exists('component_tid', $arguments)) {
      $component_tid = $arguments['component_tid'];
    }
    else {
      throw new Exception("NGDistrictSpeechMigration requires the component_tid argument.");
    }

    if (empty($arguments['source_parser_class'])) {
      $arguments['source_parser_class'] = "NGSpeechSourceParser";
    }

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_dirs, $options, $regex);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('speech');
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates Speeches from Districts.');
    $this->removeFieldMapping("field_pr_component");
    $this->removeFieldMapping("field_pr_component:source_type");
    $this->addFieldMapping("field_pr_component")->defaultValue(array($component_tid));
    $this->addFieldMapping("field_pr_component:source_type")->defaultValue('tid');
    $this->addFieldMapping('og_group_ref')->defaultValue($district)->sourceMigration("UsaoDistrict");
  }
}
