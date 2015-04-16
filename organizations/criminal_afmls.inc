<?php

/**
 * @file
 * Migration classes for Asset Forfeiture and Money Laundering Section Home.
 */

/**
 * Migrates .html files from /criminal-afmls to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-afmls
 */
class CriminalAfmlsPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/afmls',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "CriminalAfmlsPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalAfmlsObtainTitlePage");
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
    $this->description = t('Migrates pages from the Asset Forfeiture and Money Laundering Section Home.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-afmls')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/criminal/afmls/forfeiture-fellowship-bu07252013.html',
      '/criminal/afmls/forfeiture-fellowship.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /criminal-afmls.
 *
 * @package doj_migration
 * @subpackage criminal-afmls
 */
class CriminalAfmlsFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/afmls subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/afmls',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///criminal-afmls/docs';

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
    return "criminal-afmls";
  }
}

/**
 * SourceParser for /criminal-afmls pages.
 *
 * @package doj_migration
 * @subpackage criminal-afmls
 */
class CriminalAfmlsPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-afmls pages.
 *
 * @package doj_migration
 * @subpackage criminal-afmls
 * @see Obtainer.api.php
 */
class CriminalAfmlsObtainTitlePage extends ObtainTitle {
}
