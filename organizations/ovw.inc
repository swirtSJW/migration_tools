<?php

/**
 * @file
 * Defines migration classes for OVW group.
 *
 * @package doj_migration
 * @subpackage ovw
 */

/**
 * This file should reside in wordpress://, which is generally
 * sites/default/private/wordpress.
 */
define('OPA_WORDPRESS_SOURCE', 'ovwblog.wordpress.2014-01-30.xml');

/**
 * Class OvwBlogEntryMigration.
 *
 * @package doj_migration
 * @subpackage ovw
 */
class OvwBlogEntryMigration extends JusticeWordpressBlogEntry {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OVW_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}
