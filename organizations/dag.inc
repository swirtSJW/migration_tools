<?php

/**
 * @file
 * Defines migration classes for DAG group.
 *
 * @package doj_migration
 * @subpackage dag
 */

/**
 * Migrates .html files from /dag to page nodes.
 *
 * @package doj_migration
 * @subpackage dag
 */
class DagPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'dag',
    );

    $options = array(
      'recurse' => TRUE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from DAG section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('dag')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $bool = parent::prepareRow($row);
    if (isset($bool)) {
      return $bool;
    }

    $title = $row->title;
    // Some of this titles are inside of the first h2 tag.
    if (empty($title) || strcmp($title, "Justice News") == 0) {
      $query_body = htmlqp($row->body);
      $elements = $query_body->find("div.presscontenttitle");
      foreach ($elements as $element) {
        $title = $element->text();
        break;
      }
    }
    $row->title = $title;
  }
}

/**
 * Migrates binary files from /dag.
 *
 * @package doj_migration
 * @subpackage dag
 */
class DagFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates files (*.pdf) from the dag subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'dag',
    );

    $options = array(
      'recurse' => TRUE,
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///dag/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "dag";
  }
}
