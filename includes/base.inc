<?php

/**
 * @file
 * Contains base migration class for all justice.gov migrations.
 */

/**
 * Abstract intermediate class holding common settings.
 */
abstract class JusticeMigration extends DynamicMigration {
  public $basicFormat;

  public function __construct() {
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
   * @param boolean $warn_on_override
   *  Set to FALSE to prevent warnings when there's an existing mapping.
   */
  public function addFieldMappings(array $mappings, $warn_on_override = TRUE) {
    foreach ($mappings as $source => $destination) {
      $this->addFieldMapping($source, $destination, $warn_on_override);
    }
  }
}
