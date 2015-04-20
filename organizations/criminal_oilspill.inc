<?php

/**
 * @file
 * Defines migration classes for Deepwater Horizon (BP) Oil-Spill Fraud.
 */

/**
 * Migrates .html files from /criminal-oilspill to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-oilspill
 */
class CriminalOilspillPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/oilspill',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "CriminalOilspillPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalOilspillObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array('div.headline', 1));
    $title->addMethod('pluckSelector', array('div.headline', 2));
    $title->addMethod('findSelectorAttribute', array("div.headline > img", "alt"));
    $title->addMethod('pluckSelector', array("title", 1));
    $title->addMethod('pluckSelector', array("h1", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Deepwater Horizon (BP) Oil-Spill Fraud.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-oilspill')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/criminal/oilspill/index.html',
    );
    if ((parent::prepareRow($row) === FALSE) || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /criminal-oilspill.
 *
 * @package doj_migration
 * @subpackage criminal-oilspill
 */
class CriminalOilspillFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/oilspill subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/oilspill',
    );

    $dest_dir = 'public:///criminal-oilspill/docs';
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
    return "criminal-oilspill";
  }
}

/**
 * SourceParser for /criminal-oilspill pages.
 *
 * @package doj_migration
 * @subpackage criminal-oilspill
 */
class CriminalOilspillPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-oilspill pages.
 *
 * @package doj_migration
 * @subpackage criminal-oilspill
 * @see Obtainer.api.php
 */
class CriminalOilspillObtainTitlePage extends ObtainTitle {
}
