<?php

/**
 * @file
 * Defines migration classes for OPA group.
 *
 * @package doj_migration
 * @subpackage opa
 */

/**
 * This file should reside in wordpress://, which is generally
 * sites/default/private/wordpress.
 */
define('OPA_WORDPRESS_SOURCE', 'officeofpublicaffairs.wordpress.2014-01-30.xml');

/**
 * Class OpaBlogEntryMigration.
 *
 * @package doj_migration
 * @subpackage opa
 */
class OpaBlogEntryMigration extends JusticeWordpressBlogEntry {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OPA_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Class OpaAuthorMigration
 *
 * @package doj_migration
 * @subpackage opa
 */
class OpaAuthorMigration extends JusticeWordpressAuthor {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OPA_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Class OpaAttachmentMigration.
 *
 * @package doj_migration
 * @subpackage opa
 */
class OpaAttachmentMigration extends JusticeWordpressAttachment {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OPA_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}
