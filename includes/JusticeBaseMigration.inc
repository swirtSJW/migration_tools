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
  public static function geoCodeString($string) {

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
}
