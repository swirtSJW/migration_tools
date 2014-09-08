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
    parent::__construct($arguments, $source_dirs, $regex, $options);
    $this->addFieldMapping('language')->defaultValue('es');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Process release_date from spanish, into numeric.
    try {
      $date_string = $this->sourceParser->queryPath->find('.speechdate')->first()->text();
      // Date_string looks like   miércoles, 28 de febrero de 2014.
      $row->field_speech_date = parent::dojMigrationESDateConvertWDMY($date_string);
    }
    catch (Exception $e){
      $this->queueMessage("{$row->fileId} failed to acquire a date.");
    }
  }

}
