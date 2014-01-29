<?php

/**
 * @file
 * Defines migration classes for the Office of the Attorney General.
 */

class AgPressReleaseMigration extends JusticePressReleaseMigration {
  public function __construct($arguments) {
    $org_abbreviation = 'ag';

    parent::__construct($arguments, $org_abbreviation);
  }
}
