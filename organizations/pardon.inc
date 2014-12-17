<?php

/**
 * @file
 * Defines migration classes for Office of the Pardon.
 */

/**
 * Migrates .html files from /pardon to page nodes.
 *
 * @package doj_migration
 * @subpackage pardon
 */
class PardonPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      'pardon',
    );

    $arguments['source_parser_class'] = "SourceParser";

    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Office of the Pardon.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('pardon')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Clean up odd characters.
    $body = $row->body;
    $body = str_replace("Â", "", $body);

    $row->body = $body;
  }
}

/**
 * Migrates files (*.pdf) from /pardon.
 *
 * @package doj_migration
 * @subpackage pardon
 */
class PardonFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the pardon subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'pardon',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///pardon/docs';

    $options = array('recurse' => TRUE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "pardon";
  }
}
