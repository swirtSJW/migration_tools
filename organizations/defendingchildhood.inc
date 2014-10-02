<?php
/**
 * @file
 * Defending Childhood defendingchildhood.
 *
 * Defines migration classes for the Defending Childhood section of justice.gov.
 */

/**
 * Migrates .html files from /defendingchildhood to page nodes.
 *
 * @package doj_migration
 * @subpackage defendingchildhood
 */
class DefendingChildhoodPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      'defendingchildhood',
    );

    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Defending Childhood section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('defendingchildhood')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $bool = parent::prepareRow($row);
    if (isset($bool) && $bool == FALSE) {
      return $bool;
    }

    $row->body = StringCleanUp::stripCmsLegacyMarkup($row->body);
  }
}

/**
 * Migrates files (*.pdf) from /defendingchildhood.
 *
 * @package doj_migration
 * @subpackage defendingchildhood
 */
class DefendingChildhoodFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the ola subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'defendingchildhood',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx|eps)/';
    $dest_dir = 'public:///defendingchildhood/docs';

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
    return "defendingchildhood";
  }
}
