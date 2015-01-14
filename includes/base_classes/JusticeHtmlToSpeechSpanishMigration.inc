<?php
/**
 * @file
 * Defines JusticeHtmlToSpeechSpanishMigration class.
 */

/**
 * Class JusticeHtmlToSpeechSpanishMigration.
 *
 * Parent class for all migrations from static HTML to speech content type with
 * spanish specific stuff.
 *
 * @package doj_migration
 */

abstract class JusticeHtmlToSpeechSpanishMigration extends JusticeHtmlToSpeechMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $source_dirs, $regex = NULL, $options = array()) {

    // Use our own source parser, if one has not been defined.
    if (!array_key_exists('source_parser_class', $arguments)) {
      $arguments['source_parser_class'] = "HtmlToSpeechSpanishSourceParser";
    }

    parent::__construct($arguments, $source_dirs, $regex, $options);
    $this->addFieldMapping('language')->defaultValue('es');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $bool = parent::prepareRow($row);
    if (isset($bool) && $bool == FALSE) {
      return $bool;
    }

    $row->field_speech_date = $this->sourceParser->getSpeechDate();
  }
}
