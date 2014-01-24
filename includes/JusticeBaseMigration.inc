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
 */
abstract class JusticeBaseMigration extends Migration {

  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->team = array(
      new MigrateTeamMember('Matthew Grasmick', 'matthew.grasmick@acquia.com', t('Architect')),
    );
    $this->issuePattern = 'https://acquiadojo.atlassian.net/browse/:id:';

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
   * Add multiple field mappings at once.
   *
   * @param array $mappings
   *   An array of field mappings in the form of source_key => dest_key.
   *
   * @param boolean $warn_on_override
   *  Set to FALSE to prevent warnings when there's an existing mapping.
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
   * @param $row
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
}
