<?php

/**
 * @file
 * Defines Url Source for migrating items from a list of URLs.
 */

namespace MigrationTools\Source;

/**
 * Creates the migration source for a list of URL based migrations.
 *
 * Assign the migration source created by this class to $this->source within
 * the migration class.
 */
class Url {
  public $source;
  public $listUrls;
  public $ItemFile;

  /**
   * Prepare the URL source.
   *
   * All derived classes should define 'fileid' as the source key in
   * MigrateSQLMap(), as it is used to create redirects.
   *
   * @param array $urls
   *   An array of Urls of pages to migrate.  Must be fully formed with scheme
   *   host and domain.
   */
  public function __construct(array $urls) {

    // Provide migrate with a list of all URLs to be migrated.
    $this->listUrls = new \MigrationTools\Source\Url\MigrateListUrls($urls);

    // Provide methods for retrieving an URL's contents given an id.
    $this->ItemFile = new \MigrationTools\Source\Url\MigrateItemUrl();

    // Defines what will become $this->source, essential data source from which
    // to migrate.
    $this->source = new \MigrateSourceList($this->listUrls, $this->ItemFile);
  }

  /**
   * Getter for the MigrateSourceList.
   *
   * @return object
   *   The MigrateSourceList object.
   */
  public function getSource() {
    return $this->source;
  }
}
