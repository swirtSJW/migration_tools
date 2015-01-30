<?php

/**
 * @file
 * Contains base migration class for all justice.gov migrations.
 */

/**
 * The default system path for the DOJ legacy webroot. Can be overriden via
 * doj_migration_base_dir variable.
 */
define('DOJ_LEGACY_WEBROOT', DRUPAL_ROOT . '/../../legacy/www.justice.gov');

/**
 * Abstract intermediate class holding common settings.
 *
 * @package doj_migration
 */
abstract class JusticeBaseMigration extends Migration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->mergeArguments($arguments);

    $this->team = array(
      new MigrateTeamMember('Matthew Grasmick', 'matthew.grasmick@acquia.com', t('Architect')),
    );
    $this->issuePattern = 'https://acquiadojo.atlassian.net/browse/:id:';
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

    // Prepare title property by trimming to 255 chars and fixing encoding.
    if (!empty($row->title)) {
      $row->title = mb_strimwidth($row->title, 0, 255, "...");
      $row->title = html_entity_decode($row->title, ENT_QUOTES, 'UTF-8');
    }
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
   * Converts es date text of the form w m d y to numerical Y-M-D.
   *
   * @param string $date_string
   *   Should look like miércoles, 28 de febrero de 2014
   *
   * @return string
   *   Date in the form of 2014-02-21
   */
  public function dojMigrationESDateConvertWDMY($date_string = '') {
    $processed_date = '';

    // Date_string looks like   miércoles, 28 de febrero de 2014.
    // Clean up the string.
    $date_modified = trim(strtolower($date_string));
    $date_modified = str_ireplace('de', ' ', $date_modified);
    // Replace commas with a space.
    $date_modified = preg_replace('/,/', ' ', $date_modified);
    // Replace multiple spaces with a space.
    $date_modified = preg_replace('!\s+!', ' ', $date_modified);
    // Parse out the date.
    $date_array = explode(' ', $date_modified);

    // Make sure only digits in day $date_array[1] and year $date_array[3].
    $date_array[1] = preg_replace('/[^0-9]/', '', trim($date_array[1]));
    $date_array[3] = preg_replace('/[^0-9]/', '', trim($date_array[3]));

    // Properly formed should have 4 elements of which we need 3.
    if (count($date_array) == 4) {
      $error = TRUE;
      // Convert spanish months to numeric.
      $months = array(
        'enero' => '01',
        'febrero' => '02',
        'marzo' => '03',
        'abril' => '04',
        'mayo' => '05',
        'junio' => '06',
        'julio' => '07',
        'agosto' => '08',
        'septiembre' => '09',
        'octubre' => '10',
        'noviembre' => '11',
        'diciembre' => '12',
      );

      // If the spanish month name is present in the array, use the number.
      $date_array[2] = (array_key_exists($date_array[2], $months)) ? $months[$date_array[2]] : '';
      // Convert date to time and put into $row.
      if ((!empty($date_array[1])) && (!empty($date_array[2])) && (!empty($date_array[3]))) {
        $processed_date = $date_array[3] . '-' . $date_array[2] . '-' . $date_array[1];
        $error = FALSE;
      }

    }
    if ($error) {
      // Date failed, throw a migrate message.
      $msgvars = array(
        '@date_string' => $date_string,
        '@date_modified' => $date_modified,
        '@processed_date' => $date_array[3] . '-' . $date_array[2] . '-' . $date_array[1],
      );
      $message = t("Date import went wrong when: '@date_string'  modified to '@date_modified' became'@processed_date'.", $msgvars);
      watchdog('doj_migration', $message);
    }

    return $processed_date;
  }

  /**
   * Geocode a string.
   *
   * @param string $string
   *   A location string.
   *
   * @return array
   *   An array with location information extracted from the string.
   */
  public function geoCodeString($string) {

    // Geocode the location and parse into structured data for migration.
    // Geocoder module is not an explicit dependency because most migrations
    // do not rely on it. It should be disabled after use.
    if (!empty($string)) {
      if ($string == 'Washington, D.C.') {
        // The most common entry, so skip geocoding.
        $address['locality'] = 'Washington';
        $address['administrative_area_level_1'] = 'DC';
        $address['country'] = "US";
      }
      elseif (module_exists('geocoder')) {
        // Note that calling this too many times (as in very large migrations)
        // may exceed the API request limit for geocoder's source data.
        $point = geocoder('google', $string);
        module_load_include('inc', 'doj_migration', 'includes/doj_migration');
        $address = doj_migrate_convert_geocoded_point_to_address($point);

        if (!$address) {
          $address['locality'] = '';
          $address['administrative_area_level_1'] = '';
          $address['country'] = '';
          $this->queueMessage(t('Could not look up speech location because geocoder returned nothing The API request limit may have been exceeded.'));
        }
      }
      else {
        $this->queueMessage(t('Could not look up speech location because geocoder is not installed.'));
      }

      return $address;
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
      $pdfinfo_bin = variable_get('pdfinfo_binary', '/home/doj/xpdf/bin64/pdfinfo');
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
      watchdog('doj_migration', $message);
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
      $pdftotext_bin = variable_get('pdftotext_binary', '/home/doj/xpdf/bin64/pdftotext');
      $pdf_parser = XPDF\PdfToText::create(array('pdftotext.binaries' => $pdftotext_bin));
      $pdf_contents = $pdf_parser->getText($pdf_file_path);

      return $pdf_contents;
    }
    catch (Exception $e) {
      $message = t('Could not parse attached PDF for @fileid. Exception thrown: @exception', array('@fileid' => $pdf_file_path, '@exception' => $e->getMessage()));
      $this->queueMessage($message);
      watchdog('doj_migration', $message);
    }
  }

  /**
   * Creates redirects for files attached to a given entity's field field.
   *
   * @param object $entity
   *   The fully loaded entity.
   *
   * @param array $source_urls
   *   A flat array of source urls that should redirect to the attachments
   *   on this entity. $source_urls[0] will redirect to the first attachment,
   *   $entity->$field_name[$language][0], and so on.
   *
   * @param string $field_name
   *   The machine name of the attachment field.
   *
   * @param string $language
   *   Optional. Defaults to LANGUAGE_NONE.
   */
  public static function createAttachmentRedirect($entity, $source_urls, $field_name, $language = LANGUAGE_NONE) {
    if (empty($source_urls)) {
      // Nothing to be done here.
      $json_entity = json_encode($entity);
      watchdog("doj_migration", "redirect was not created for attachment in entity {$json_entity}");
      return;
    }

    $field = $entity->$field_name;
    if (!empty($field[$language])) {
      foreach ($field[$language] as $delta => $item) {
        $file = file_load($item['fid']);
        $destination = file_create_url($file->uri);
        $source = $source_urls[$delta];

        // Create redirect.
        $redirect = redirect_load_by_source($source);
        if (!$redirect) {
          $redirect = new stdClass();
          redirect_object_prepare($redirect);
          $redirect->source = $source;
          $redirect->redirect = $destination;
          redirect_save($redirect);
        }
      }
    }
  }

  /**
   * Deletes any redirects associated files attached to an entity's file field.
   *
   * @param object $entity
   *   The fully loaded entity.
   *
   * @param string $field_name
   *   The machine name of the attachment field.
   *
   * @param string $language
   *   Optional. Defaults to LANGUAGE_NONE.
   */
  public static function rollbackAttachmentRedirect($entity, $field_name, $language = LANGUAGE_NONE) {
    $field = $entity->$field_name;
    if (!empty($field[$language])) {
      foreach ($field[$language] as $delta => $item) {
        $file = file_load($item['fid']);
        $destination = file_create_url($file->uri);
        redirect_delete_by_path($destination);
      }
    }
  }
}
