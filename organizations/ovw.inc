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
define('OVW_WORDPRESS_SOURCE', 'ovwblog.wordpress.2014-07-29.xml');

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

    $this->addSoftDependencies(array("OvwCategory"));

    $this->addFieldMapping("field_blog_topic", 'category_value');
    $this->addFieldMapping("field_blog_date", 'pubDate');

    // Let's get the right taxonomy_term to set as default.
    $ovw = "Office on Violence Against Women";
    $tts = taxonomy_get_term_by_name($ovw, "component");
    $tt = reset($tts);
    if ($tt) {
      $this->addFieldMapping("field_blog_component")->defaultValue($tt->name);
    }
    else {
      throw new Exception("Taxonomy Term '{$ovw}' does not exist. Create it and re-run the migration");
    }
  }
}

/**
 * Class OvwAuthorMigration
 *
 * @package doj_migration
 * @subpackage ovw
 */
class OvwAuthorMigration extends JusticeWordpressAuthor {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OVW_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Class OvwAttachmentMigration.
 *
 * @package doj_migration
 * @subpackage ovw
 */
class OvwAttachmentMigration extends JusticeWordpressAttachment {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OVW_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Class OvwAttachmentMigration.
 *
 * @package doj_migration
 * @subpackage ovw
 */
class OvwCategoryMigration extends JusticeWordpressCategory {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OVW_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    // If the category that is being migrated from wordpress, already exists
    // in the component vocabulary, then we do not need to save it as a
    // topic.
    $taxonomy_term_name = $row->cat_name;
    $tts = taxonomy_get_term_by_name($taxonomy_term_name, "component");
    $tt = reset($tts);
    if ($tt) {
      return FALSE;
    }
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
    parent::__construct($arguments, $source_dirs, NULL, $options);
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
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
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
