<?php

/**
 * @file
 * Open Government - Open.
 *
 * Defines migration classes for the Open section of justice.gov.
 */

/**
 * Migrates .html files from /open to page nodes.
 *
 * @package doj_migration
 * @subpackage open
 */
class OpenPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'open',
    );

    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Open Government section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('open')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {

    $bool = parent::prepareRow($row);
    if (isset($bool)) {
      return $bool;
    }

    $images = $this->source_parser->queryPath->find('img');

    foreach ($images as $image) {
      $src = $image->attr('src');
      if (substr_count($src, "banner") > 0) {
        $image->remove();
      }
      break;
    }

    $row->body = $this->source_parser->getBody();

  }
}

/**
 * Migrates files (*.pdf) from /open.
 *
 * @package doj_migration
 * @subpackage open
 */
class OpenFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Match .pdf files only.
    $source_dirs = array(
      'open',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///open/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "open";
  }
}
