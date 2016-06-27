<?php

/**
 * @file
 * Defines MigrateListUrls Source for migrating items from a list of URLs.
 */

namespace MigrationTools\Source\Url;

/**
 * Extends the MigrateList class to obtain a list of URLs to migrate from array.
 */
class MigrateListUrls  extends \MigrateList {
  public $Urls;

  /**
   * Obtain a list of URLs to migrate from an array or URLs.
   *
   * @param array $urls
   *   Array of urls to migrate.
   */
  public function __construct(array $urls) {
    // Weed out any non URLS.
    $urls = array_map('\MigrationTools\Source\Url\MigrateListUrls::cleanAndValidate', $urls);
    $urls = array_filter($urls);
    natcasesort($urls);
    $this->Urls = $urls;
  }

  /**
   * Clean or remove an invalid URL.
   *
   * @param string $url
   *   An url to clean and make sure it is valid.
   *
   * @return string
   *   A cleaned URL.
   */
  public static function cleanAndValidate($url) {
    $url = trim($url);
    $scheme = parse_url($url, PHP_URL_SCHEME);
    $host = parse_url($url, PHP_URL_HOST);
    if (empty($scheme) && empty($host)) {
      // This is invalid.  Wipe it out.
      $url = '';
    }

    return $url;
  }

  /**
   * Return a string representing where the listing is obtained from.
   *
   * @return string
   *   A text description of where the listing is obtained from.
   */
  public function __toString() {
    return 'URLs list';
  }


  /**
   * Returns an array of unique IDs for passing to the MigrateItem.
   *
   * @return mixed
   *   Iterator or Array
   */
  public function getIdList() {
    return $this->Urls;
  }

  /**
   * Return a count of IDs available to be migrated.
   *
   * @return int
   *   The number of ids available to migrate.
   */
  public function computeCount() {
    return count($this->Urls);
  }
}
