<?php

/**
 * @file
 * Defines migration classes for Civil Rights Division, Military.
 */

/**
 * Migrates .html files from /crt-military to page nodes.
 *
 * @package doj_migration
 * @subpackage crt-military
 */
class CrtMilitaryPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'crt/military',
    );

    $options = array('recurse' => TRUE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "CrtMilitaryPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CrtMilitaryObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("h2", 1));
    $title->addMethod('pluckSelector', array("title", 1));

    $body = new ObtainerInfo('body', 'UsaoNdilObtainBody');
    $body->addMethod('pluckSelector', array("blockquote", 1));
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $body;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Civil Rights Division, Military.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('crt-military')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/crt/military/cases.htm',
      '/crt/military/cases_spanish.htm',
      '/crt/military/contact_us_spanish.htm',
      '/crt/military/faq_spanish.htm',
      '/crt/military/file_claim_spanish.htm',
      '/crt/military/other_sources_spanish.htm',
      '/crt/military/press_releases_spanish.htm',
      '/crt/military/press_releases_spanish.html',
      '/crt/military/scra_summary_spanish.html',
      '/crt/military/mil_search.html',
      '/crt/military/mil_search2.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /crt-military.
 *
 * @package doj_migration
 * @subpackage crt-military
 */
class CrtMilitaryFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the crt/military subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'crt/military',
    );

    $dest_dir = 'public:///crt-military/docs';
    $regex = NULL;

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
    return "crt-military";
  }
}

/**
 * SourceParser for /crt-military pages.
 *
 * @package doj_migration
 * @subpackage crt-military
 */
class CrtMilitaryPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /crt-military pages.
 *
 * @package doj_migration
 * @subpackage crt-military
 * @see Obtainer.api.php
 */
class CrtMilitaryObtainTitlePage extends ObtainTitle {

}


