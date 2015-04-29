<?php

/**
 * @file
 * Defines migration classes for Office of Enforcement Operations.
 */

/**
 * Migrates .html files from /criminal-oeo to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-oeo
 */
class CriminalOeoPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/oeo',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "CriminalOeoPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalOeoObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array('div.headline', 1));
    $title->addMethod('pluckSelector', array('div.headline', 2));
    $title->addMethod('findSelectorAttribute', array("div.headline > img", "alt"));
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("title", 1));

    $arguments['obtainers_info'][] = $title;

    // $extension_regex = /.*\.(html|htm)$/i;
    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Office of Enforcement Operations.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-oeo')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/criminal/oeo/index.html',
      '/criminal/oeo/links/gambling/index.html',
      '/criminal/oeo/links/intlprisoner/intlprisoner.html',
    );
    if ((parent::prepareRow($row) === FALSE) || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /criminal-oeo.
 *
 * @package doj_migration
 * @subpackage criminal-oeo
 */
class CriminalOeoFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/oeo subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/oeo',
    );

    $dest_dir = 'public:///criminal-oeo/docs';
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
    return "criminal-oeo";
  }
}

/**
 * SourceParser for /criminal-oeo pages.
 *
 * @package doj_migration
 * @subpackage criminal-oeo
 */
class CriminalOeoPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-oeo pages.
 *
 * @package doj_migration
 * @subpackage criminal-oeo
 * @see Obtainer.api.php
 */
class CriminalOeoObtainTitlePage extends ObtainTitle {

}
