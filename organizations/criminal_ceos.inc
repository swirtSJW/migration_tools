<?php

/**
 * @file
 * Defines migration classes for Child Exploitation and Obscenity Section.
 */

/**
 * Migrates .html files from /criminal-ceos to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-ceos
 */
class CriminalCeosPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/ceos',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "CriminalCeosPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalCeosObtainTitlePage");
    $title->addMethod('findPCenterStrong');
    $title->addMethod('findDivCenterStrong');
    $title->addMethod('findPStrong');
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findSelector', array("title", 1));
    $title->addMethod('findSelector', array("h1", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Child Exploitation and Obscenity Section.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-ceos')
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
 * Migrates files (*.pdf) from /criminal-ceos.
 *
 * @package doj_migration
 * @subpackage criminal-ceos
 */
class CriminalCeosFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/ceos subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/ceos',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///criminal-ceos/docs';

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
    return "criminal-ceos";
  }
}


/**
 * SourceParser for /criminal-ceos pages.
 *
 * @package doj_migration
 * @subpackage criminal-ceos
 */
class CriminalCeosPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-ceos pages.
 *
 * @package doj_migration
 * @subpackage criminal-ceos
 * @see Obtainer.api.php
 */
class CriminalCeosObtainTitlePage extends ObtainTitle {

  /**
   * Returns first .bodytext p[align="center"] > strong.
   *
   * @return string
   *   The matched text.
   */
  public function findPCenterStrong() {
    $element = $this->queryPath->find('.bodytext p[align="center"] > strong')->first();
    $this->setElementToRemove($element);
    $text = $element->text();

    return $text;
  }

  /**
   * Returns first .bodytext  div[align="center"] > strong.
   *
   * @return string
   *   The matched text.
   */
  public function findDivCenterStrong() {
    $element = $this->queryPath->find('.bodytext div[align="center"] > strong')->first();
    $this->setElementToRemove($element);
    $text = $element->text();

    return $text;
  }
  /**
   * Returns first .bodytext p > strong.
   *
   * @return string
   *   The matched text.
   */
  public function findPStrong() {
    $element = $this->queryPath->find('.bodytext p > strong')->first();
    $this->setElementToRemove($element);
    $text = $element->text();

    return $text;
  }
}
