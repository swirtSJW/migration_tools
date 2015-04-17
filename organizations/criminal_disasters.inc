<?php

/**
 * @file
 * Defines migration classes for Disaster Fraud Task Force.
 */

/**
 * Migrates .html files from /criminal-disasters to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-disasters
 */
class CriminalDisastersPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/disasters',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "CriminalDisastersPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalDisastersObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array('div.headline', 1));
    $title->addMethod('pluckSelector', array('div.headline', 2));
    $title->addMethod('findSelectorAttribute', array("div.headline > img", "alt"));
    $title->addMethod('pluckSelector', array("title", 1));
    $title->addMethod('pluckSelector', array("h1", 1));

    $arguments['obtainers_info'][] = $title;

    // $extension_regex = /.*\.(html|htm)$/i;
    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Disaster Fraud Task Force.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-disasters')
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
 * Migrates binary files from /criminal-disasters.
 *
 * @package doj_migration
 * @subpackage criminal-disasters
 */
class CriminalDisastersFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/disasters subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/disasters',
    );

    $dest_dir = 'public:///criminal-disasters/docs';
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
    return "criminal-disasters";
  }
}

/**
 * SourceParser for /criminal-disasters pages.
 *
 * @package doj_migration
 * @subpackage criminal-disasters
 */
class CriminalDisastersPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    $this->queryPath->find('p[align="right"]')->remove();
    $this->queryPath->find('div[class="pressheader"]')->remove();
  }
}

/**
 * Obtainer for title property for /criminal-disasters pages.
 *
 * @package doj_migration
 * @subpackage criminal-disasters
 * @see Obtainer.api.php
 */
class CriminalDisastersObtainTitlePage extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'USDOJ: CRM: DFTF: Press Releases 2006',
      'USDOJ: CRM: DFTF: Press Releases 2007',
      'USDOJ: CRM: DFTF: Press Releases 2008',
      'USDOJ: CRM: DFTF: Press Releases 2009',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}
