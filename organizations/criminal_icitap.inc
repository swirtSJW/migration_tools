<?php

/**
 * @file
 * Defines migration classes for ICITAP.
 */

/**
 * Migrates .html files from /criminal-icitap to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-icitap
 */
class CriminalIcitapPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/icitap',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "CriminalIcitapPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalIcitapObtainTitlePage");
    $title->addMethod('findSelector', array('p[style="margin-bottom:-15px;"]', 1));
    $title->addMethod('findSelector', array('p[style="margin-bottom:-10px;"]', 1));
    $title->addMethod('findSelector', array(".subHeader", 1));
    $title->addMethod('findSelector', array(".headline", 1));
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findSelector', array("title", 1));
    $title->addMethod('findSelector', array("h1", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the International Criminal Investigative Training Assistance Program.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-icitap')
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
 * Migrates binary files from /criminal-icitap.
 *
 * @package doj_migration
 * @subpackage criminal-icitap
 */
class CriminalIcitapFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/icitapcd subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/icitap',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///criminal-icitap/docs';

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
    return "criminal-icitap";
  }
}

/**
 * SourceParser for /criminal-icitap pages.
 *
 * @package doj_migration
 * @subpackage criminal-icitap
 */
class CriminalIcitapPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-icitap pages.
 *
 * @package doj_migration
 * @subpackage criminal-icitap
 * @see Obtainer.api.php
 */
class CriminalIcitapObtainTitlePage extends ObtainTitle {

}
