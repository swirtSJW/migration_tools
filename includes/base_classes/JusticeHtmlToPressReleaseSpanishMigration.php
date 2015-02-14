<?php
/**
 * @file
 * Defines JusticeHtmlToPressReleaseSpanishMigration class.
 */

/**
 * Class JusticeHtmlToPressReleaseSpanishMigration.
 *
 * Static HTML to PressRelease content type with spanish specific treatment.
 *
 * @package doj_migration
 */

abstract class JusticeHtmlToPressReleaseSpanishMigration extends JusticeHtmlToPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $source_dirs, $options = array()) {

    // Use our own source parser, if one has not been defined.
    if (!array_key_exists('source_parser_class', $arguments)) {
      $arguments['source_parser_class'] = "DistrictPressReleaseSpanishSourceParser";
    }

    parent::__construct($arguments, $source_dirs, $options);
    $this->addFieldMapping('language')->defaultValue('es');
  }
}
