<?php

/**
 * @file
 * Defines MigrateItemUrl  for migrating items from retrieving content from an
 * URL.
 */

namespace MigrationTools\Source\Url;

/**
 * Implementation of MigrateItem, for retrieving a page from an URL
 * based on an URL provided by a MigrateListUrls class.
 */
class MigrateItemUrl extends \MigrateItem {
  /**
   * {@inheritdoc}
   */
  public function __construct() {}

  /**
   * Given an URL return an object representing a source item.
   *
   * @param string $url
   *   The Url which is the unique ID for the item.
   *
   * @return \stdClass
   *   Contains the html retreived from $url as ->filedata.
   */
  public function getItem($url) {
    $html = file_get_contents($url);

    if ($html === FALSE) {
      // There was an error.
      $message = 'Was unable to load !url';
      $variables = array('!url' => $url);
      \MigrationTools\Message::make($message, $variables, WATCHDOG_ERROR);
      $migration = \Migration::currentMigration();
      $message = t('Loading of !objecturi failed:', array('!objecturi' => $url));
      $migration->getMap()->saveMessage(
              array($id), $message, MigrationBase::MESSAGE_ERROR);
      $return = NULL;
    }
    else {
      $return = new \stdClass();
      $return->filedata = $html;
    }

    return $return;
  }
}
