<?php

/**
 * @file
 * Defines migration classes for Human Rights and Special Prosecutions Section.
 */

/**
 * Migrates .html files from /criminal-hrsp to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-hrsp
 */
class CriminalHrspPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/hrsp',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "CriminalHrspPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalHrspObtainTitlePage");
    $title->addMethod('findSelector', array(".headline", 1));
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findSelector', array("title", 1));
    $title->addMethod('findSelector', array("h1", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Human Rights and Special Prosecutions Section.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-hrsp')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array();
    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /criminal-hrsp.
 *
 * @package doj_migration
 * @subpackage criminal-hrsp
 */
class CriminalHrspFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/hrsp subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/hrsp',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///criminal-hrsp/docs';

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
    return "criminal-hrsp";
  }
}

/**
 * SourceParser for /criminal-hrsp pages.
 *
 * @package doj_migration
 * @subpackage criminal-hrsp
 */
class CriminalHrspPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-hrsp pages.
 *
 * @package doj_migration
 * @subpackage criminal-hrsp
 * @see Obtainer.api.php
 */
class CriminalHrspObtainTitlePage extends ObtainTitle {

}
