<?php

/**
 * @file
 * Contains classes for migrations from Wordpress.
 */

define('DOJ_LEGACY_WORDPRESS_DIR', DRUPAL_ROOT . '/../../legacy/wordpress_exports');

/**
 * Implementation of WordPressMigration, for blog entries.
 *
 * @package doj_migration
 */
abstract class JusticeWordpressBlogEntry extends WordPressBlogEntry {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['post_type'] = 'blog_entry';
    $arguments['path_action'] = 2;
    $arguments['generate_redirects'] = 1;
    $arguments['tag_vocabulary'] = 'tags';
    $arguments['category_vocabulary'] = 'blog_categories';
    $arguments['text_format'] = 'wysiwyg';
    $arguments['podcast_field'] = '';
    $arguments['attachment_field'] = 'blog_attachments';

    parent::__construct($arguments);

    $this->addFieldMapping('og_group_ref')->defaultValue($arguments['group_name'])->sourceMigration('Organization');
    $this->addFieldMapping('field_blog_body', 'content');
    $this->addFieldMapping('field_blog_body:format')->defaultValue($arguments['text_format']);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      if ($row->post_type == $this->postType) {
        if (isset($row->status) && $row->status == 'trash') {
          $this->queueMessage(t('Item has status "trash." Not migrating.'));
        }
      }
      return FALSE;
    }

    if (trim($row->content) == '') {
      $row->status = NODE_UNPUBLISHED;
    }
    else {
      $row->status = NODE_PUBLISHED;
    }

    // Trim title.
    if (strlen($row->title) > 255) {
      $row->title = substr(trim($row->title), 0, 255);
      $this->queueMessage(t('Title was too long. It has been truncated to @title.', array('@title' => $row->title)));
    }
  }
}

/**
 * Class JusticeWordpressAuthor.
 */
abstract class JusticeWordpressAuthor extends WordPressAuthor {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['import_users'] = 0;
    $arguments['default_author_uid'] = 1;

    parent::__construct($arguments);
  }
}

/**
 * Class JusticeWordpressAttachment.
 */
abstract class JusticeWordpressAttachment extends WordPressAttachment {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Replace https with http for URLs point to blogs.justice.gov.
    if (str_pos($row->attachment_url, 'blogs.justice.gov') !== FALSE) {
      $row->attachment_url = str_replace('https://', 'http://', $row->attachment_url);
    }
  }
}

/**
 * Copies and preprocesses raw Wordpress XML file to wordpress:// directory.
 *
 * @param string $filename
 *   The filename of the Wordpress source XML file.
 *
 * @return string
 *   The filepath of the preprocessed file.
 *
 * @throws Exception
 */
function doj_migrate_wordpress_prepare_file($filename) {
  $source_directory = DOJ_LEGACY_WORDPRESS_DIR;
  $source_filepath = $source_directory . '/' . $filename;
  $dest_directory = 'wordpress://';
  $dest_filepath = $dest_directory . str_replace(' ', '%20', $filename);

  // Preprocess source file and store in sites/default/private/wordpress.
  if (file_exists($source_filepath) && !file_exists($dest_filepath)) {
    if (!variable_get('file_private_path')) {
      throw new Exception('You must specify a private file system path at admin/config/media/file-system');
    }
    WordPressBlog::preprocessFile($source_filepath, $dest_filepath, FALSE);
  }

  // Throw error if the preprocessed file does not exist.
  if (file_exists($dest_filepath)) {
    $arguments['filename'] = $dest_filepath;
  }
  else {
    throw new Exception("Could not find Wordpress source XML file $dest_filepath.");
  }

  return $dest_filepath;
}
