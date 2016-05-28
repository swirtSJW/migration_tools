<?php

/**
 * @file
 * Defines Url Source for migrating items from a list of URLs.
 */

namespace MigrationTools\Source;

/**
 * Creates the migration source for a list of Url based migrations.
 *
 * Assign the migration source created by this class to $this->source within
 * the migration class.
 */
class Url {
  // When calling MigrateItemFile need to set flag $this->getContents to TRUE.
  // http://www.drupalcontrib.org/api/drupal/contributions!migrate!plugins!sources!files.inc/function/MigrateItemFile%3A%3AloadFile/7
}
