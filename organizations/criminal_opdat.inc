<?php

/**
 * @file
 * Defines migration classes for 
 * Office of Overseas Prosecutorial Development Assistance and Training.
 */

/**
 * Migrates .html files from /criminal-opdat to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-opdat
 */
class CriminalOpdatPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/opdat',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "CriminalOpdatPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalOpdatObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSelector', array('div.headline', 1));
    $title->addMethod('findSelector', array('div.headline', 2));
    $title->addMethod('findSelectorAttribute', array("div.headline > img", "alt"));
    $title->addMethod('findSelector', array("title", 1));
    $title->addMethod('findSelector', array("h1", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Office of Overseas Prosecutorial Development Assistance and Training.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-opdat')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array();
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /criminal-opdat.
 *
 * @package doj_migration
 * @subpackage criminal-opdat
 */
class CriminalOpdatFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/opdat subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/opdat',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///criminal-opdat/docs';

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
    return "criminal-opdat";
  }
}

/**
 * SourceParser for /criminal-opdat pages.
 *
 * @package doj_migration
 * @subpackage criminal-opdat
 */
class CriminalOpdatPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-opdat pages.
 *
 * @package doj_migration
 * @subpackage criminal-opdat
 * @see Obtainer.api.php
 */
class CriminalOpdatObtainTitlePage extends ObtainTitle {

}
