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

/**
 * Migrates .html files from /ovw to page nodes.
 * 
 * @package doj_migration
 * @subpackage nsd
 */
class OvwPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'ovw',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Office on Violence Against Women section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('ovw')->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /nsd.
 *
 * @package doj_migration
 * @subpackage nsd
 */
class OvwFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates files (*.pdf) from the ovw subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      "ovw",
      "ovw/docs",
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///oarm/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "ovw";
  }
}
