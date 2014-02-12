<?php

/**
 * @file
 * Defines migration classes for the Office of the Attorney General.
 */

/**
 * Class AgPressReleaseMigration.
 *
 * @package doj_migration
 */
class AgPressReleaseMigration extends JusticePressReleaseMigration {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $org_abbreviation = 'ag';

    parent::__construct($arguments, $org_abbreviation);
  }
}
