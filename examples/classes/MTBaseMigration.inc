<?php

/**
 * @file
 * Contains base migration class for all migrations.
 */

/**
 * The default system path for the MT legacy webroot. Can be overriden via
 * migration_tools_base_dir variable.
 */
define('MT_LEGACY_WEBROOT', DRUPAL_ROOT . '/../../legacy/www.somesite.com');

/**
 * Abstract intermediate class holding common settings.
 *
 * @package migration_tools
 */
abstract class MTBaseMigration extends Migration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->mergeArguments($arguments);

  }

  /**
   * Add multiple field mappings at once.
   *
   * @param array $mappings
   *   An array of field mappings in the form of source_key => dest_key.
   *
   * @param bool $warn_on_override
   *   Set to FALSE to prevent warnings when there's an existing mapping.
   */
  public function addFieldMappings(array $mappings, $warn_on_override = TRUE) {
    foreach ($mappings as $source => $destination) {
      $this->addFieldMapping($source, $destination, $warn_on_override);
    }
  }

  /**
   * Sets the field mapping for an organic group parent using uuid.
   *
   * @param string $uuid
   *   The unique identifier for the Organic Group.
   *
   * @return string
   *   The entity id of the Organic Group.
   */
  public function addGroupReferenceMapping($uuid) {
    // Set the Organic Group to which this content belongs. Get the data
    // here so that it isn't fetched per $row in prepare().
    list($og_eid) = array_values(entity_get_id_by_uuid('node', array($uuid)));
    $this->addFieldMapping('og_group_ref')->defaultValue($og_eid);

    return $og_eid;
  }

  /**
   * Arguements in keyed array passed among migration classes and parsers.
   *
   * @var array $arguments
   */
  protected $arguments = array();
  /**
   * Basic cetter for $arguments.
   *
   * @return array
   *   Whatever has been stored in $this->arguments.
   */
  public function getArguments() {
    return $this->arguments;
  }
  /**
   * Merges an array into the existing arguments array.
   *
   * @param array $new_args
   *   An array matching the format of the arguments array, to be merged.
   *
   * @return array
   *   Array of the complete arguments array with the new material.
   */
  protected function mergeArguments($new_args = '') {
    if (!empty($new_args) && is_array($new_args)) {
      $this->arguments = array_merge($this->arguments, $new_args);
    }
  }

  /**
   * Gets a single argument from the arguments array.
   *
   * @param string $arg_key
   *   The key of the item to return from the Arguments array.
   *
   * @return mixed
   *   Whatever is stored in the $keys's value, or NULL if not in the arguments.
   */
  protected function getArgument($arg_key = '') {
    if (!empty($arg_key)) {
      $args = $this->getArguments();
      if (array_key_exists($arg_key, $args)) {
        return $args[$arg_key];
      }
    }
    return NULL;
  }


  /**
   * Unsets NULL properties on a single dimensional object.
   *
   * @param obj $row
   *   The object to iterate over.
   */
  public function removeEmptyProperties(&$row) {
    foreach ($row as $key => $property) {
      if (is_null($property)) {
        unset($row->$key);
      }
      elseif (is_string($property) && !$property) {
        unset($row->$key);
      }
    }
  }

  /**
   * Adds path and path auto field mapping to 'Do not migrate' issue group.
   */
  public function addPathDnmMapping() {
    // Map path and pathauto fields to NULL, assigning to DNM.
    if (module_exists('path')) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (module_exists('pathauto')) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

  }

  /**
   * Alters language to LANGUAGE_NONE if used in prepare().
   *
   * This is to correct issues with redirects and aliases having languages.
   *
   * @param object $entity
   *   Fully loaded entity.
   */
  public function languageSideStep($entity) {
    // Check language, if other than LANGUAGE_NONE, sidestep it.
    if ($entity->language !== LANGUAGE_NONE) {
      // Save the original langage for processing in the complete().
      $entity->original_language = $entity->language;
      $entity->language = LANGUAGE_NONE;
    }
  }

  /**
   * Alters language on node back to originally specified if used in complete().
   *
   * This is to correct issues with redirects and aliases having languages.
   *
   * @param object $entity
   *   Fully loaded entity.
   */
  public function languageReturnStep($entity) {
    // Check to see if language was sidestepped in prepare().
    if (!empty($entity->original_language)) {
      // Language was sidestepped, so put it back and resave.
      $entity->language = $entity->original_language;
      unset($entity->original_language);
      entity_save('node', $entity);
    }
  }


  /**
   * Extracts metadata from pdf file.
   *
   * @param string $pdf_file_path
   *   The absolute file path of the pdf on the local system.
   *
   * @return array
   *   An associative array of pdf metadata.
   */
  public function getPdfMetadata($pdf_file_path) {
    try {
      $pdfinfo_bin = variable_get('pdfinfo_binary', '/home/mt/xpdf/bin64/pdfinfo');
      $file_arg = escapeshellarg($pdf_file_path);
      $command = "$pdfinfo_bin $file_arg";
      $output = shell_exec($command);
      $pdf_info_rows = explode("\n", $output);
      $pdf_metadata = array();
      foreach ($pdf_info_rows as $row) {
        $columns = explode(':', $row);
        $pdf_metadata[$columns[0]] = $columns[1];
      }

      return $pdf_metadata;
    }
    catch (Exception $e) {
      $message = t('Could not parse attached PDF for @fileid. Exception thrown: @exception', array('@fileid' => $pdf_file_path, '@exception' => $e->getMessage()));
      $this->queueMessage($message);
      watchdog('migration_tools', $message);
    }
  }

  /**
   * Extracts the contents of a pdf file.
   *
   * @param string $pdf_file_path
   *   The absolute file path of the pdf on the local system.
   *
   * @return string
   *   The text content of the pdf.
   *
   * @throws \XPDF\Exception\BinaryNotFoundException
   */
  public function getPdfContents($pdf_file_path) {
    try {
      // Get PDF contents.
      $pdftotext_bin = variable_get('pdftotext_binary', '/home/mt/xpdf/bin64/pdftotext');
      $pdf_parser = XPDF\PdfToText::create(array('pdftotext.binaries' => $pdftotext_bin));
      $pdf_contents = $pdf_parser->getText($pdf_file_path);

      return $pdf_contents;
    }
    catch (Exception $e) {
      $message = t('Could not parse attached PDF for @fileid. Exception thrown: @exception', array('@fileid' => $pdf_file_path, '@exception' => $e->getMessage()));
      $this->queueMessage($message);
      watchdog('migration_tools', $message);
    }
  }

}
