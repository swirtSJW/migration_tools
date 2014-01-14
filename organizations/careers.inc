<?php

/**
 * @file
 * Defines migration classes for the Careers section of justice.gov.
 */

/**
 * Abstract intermediate class holding common settings.
 */
abstract class CareersMigration extends JusticeMigration {
  public $basicFormat;

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    // We group all sub classes of this super class under the 'careers' group,
    // and indicate that it should appear after the 'default' group in UI.
    parent::__construct(MigrateGroup::getInstance('careers', array('default')));
  }
}
