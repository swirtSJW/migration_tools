<?php

/**
 * @file
 * Defines migration classes for Criminal Division.
 */

/**
 * Migrates .html files from /criminal to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal
 */
class CriminalPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      23 => 'criminal/pin',
      76 => 'criminal/taskforces',
      77 => 'criminal/employment',
      120 => 'criminal/about',
      121 => 'criminal/foia',
      148 => 'criminal',
    );

    $options = array('recurse' => FALSE);

    $arguments['source_parser_class'] = "CriminalPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSelectorAttribute', array('div[style="margin-bottom:20px;"] > img', "alt"));
    $title->addMethod('findSelectorAttribute', array("div.headline > img", "alt"));
    $title->addMethod('pluckSelector', array('div.headline', 1));
    $title->addMethod('pluckSelector', array('div.headline', 2));
    $title->addMethod('pluckSelector', array("title", 1));
    $title->addMethod('pluckSelector', array("h1", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Criminal Division.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/criminal/index.html',
      '/criminal/about/graphic.html',
      '/criminal/about/orgchart.html',
      '/criminal/foia/index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these)  || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /criminal.
 *
 * @package doj_migration
 * @subpackage criminal
 */
class CriminalFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal',
    );

    $dest_dir = 'public:///criminal/docs';
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
    return "criminal";
  }
}

/**
 * SourceParser for /criminal pages.
 *
 * @package doj_migration
 * @subpackage criminal
 */
class CriminalPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal pages.
 *
 * @package doj_migration
 * @subpackage criminal
 * @see Obtainer.api.php
 */
class CriminalObtainTitlePage extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'USDOJ: CRM: ',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}
