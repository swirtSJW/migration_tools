<?php

/**
 * @file
 * Defines migration classes for División Antimonopolios en Español.
 */

/**
 * Migrates .html files from /atr-espanol to page nodes.
 *
 * @package doj_migration
 * @subpackage atr-espanol
 */
class AtrEspanolPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'atr/espanol',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "AtrEspanolPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "AtrEspanolObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("title", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the División Antimonopolios en Español.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('atr-espanol')
    ->sourceMigration('Organization');
    $this->addFieldMapping('language')->defaultValue('es');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/atr/espanol/index.html',
      '/atr/espanol/index1.html',
      '/atr/espanol/index-bk.html',
      '/atr/espanol/index-old2.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these)  || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /atr-espanol.
 *
 * @package doj_migration
 * @subpackage atr-espanol
 */
class AtrEspanolFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the atr/espanol subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'atr/espanol',
    );

    $dest_dir = 'public:///atr-espanol/docs';
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
    return "atr-espanol";
  }
}

/**
 * SourceParser for /atr-espanol pages.
 *
 * @package doj_migration
 * @subpackage atr-espanol
 */
class AtrEspanolPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /atr-espanol pages.
 *
 * @package doj_migration
 * @subpackage atr-espanol
 * @see Obtainer.api.php
 */
class AtrEspanolObtainTitlePage extends ObtainTitle {

}
