<?php

/**
 * @file
 * Defines migration classes for District of Puerto Rico.
 */

/**
 * Migrates .html files from /usao-pr to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-pr
 */
class UsaoPrPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/pr',
    );

    $arguments['source_parser_class'] = "UsaoPrPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoPrObtainTitlePage");
    $title->addMethod('findSelector', array('selector' => 'h1', 1));
    $title->addMethod('findAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Puerto Rico.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-pr')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {

    $skip_these = array(
      '/usao/pr/divisions_appellate.html',
      '/usao/pr/news2014.html',
      '/usao/pr/news2013.html',
      '/usao/pr/news.html',
    );

    if ((parent::prepareRow($row) === FALSE) || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-pr.
 *
 * @package doj_migration
 * @subpackage usao-pr
 */
class UsaoPrFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/pr subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/pr',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-pr/docs';

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
    return "usao-pr";
  }
}

/**
 * Migrates press releases from /usao-pr.
 *
 * @package doj_migration
 * @subpackage usao-pr
 */
class UsaoPrPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-pr";
    $arguments['component_tid'] = 2041;
    $arguments['source_parser_class'] = "UsaoPrPressSourceParser";

    $date = new ObtainerInfo('date');
    $date->addMethod('findSelector', array(".newsRight", 1));
    $date->addMethod("findProbableDate");
    $title = new ObtainerInfo('title');
    $title->addMethod('findAnySelectorUntilValid', array('h1'));
    $subtitle = new ObtainerInfo('subtitle');
    $subtitle->addMethod('findSelector', array("h2", 1));

    $pr_number = new ObtainerInfo('prNumber', "UsaoPrObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $subtitle;
    $arguments['obtainers_info'][] = $pr_number;

    $source_dirs = array(
      2 => 'usao/pr/news/2014',
      3 => 'usao/pr/news/2015',
      4 => 'usao/pr/news/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}

/**
 * SourceParser for /usao-pr pages.
 *
 * @package doj_migration
 * @subpackage usao-pr
 */
class UsaoPrPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-pr press releases.
 *
 * @package doj_migration
 * @subpackage usao-pr
 */
class UsaoPrPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-pr pages.
 *
 * @package doj_migration
 * @subpackage usao-pr
 * @see Obtainer.api.php
 */
class UsaoPrObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-pr press releases.
 *
 * @package doj_migration
 * @subpackage usao-pr
 * @see Obtainer.api.php
 */
class UsaoPrObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-pr press releases.
 *
 * @package doj_migration
 * @subpackage usao-pr
 * @see Obtainer.api.php
 */
class UsaoPrObtainPrNumber extends ObtainId {
  /**
   * Finds the press release number from markup.
   */
  protected function findPrNumber() {
    $text = '';
    // $element = $this->queryPath->find('p')->last();
    // $this->setElementToRemove($element);
    // $text = $element->text();

    return $text;
  }
}

/**
 * Custom obtainer for date property for /usao-pr.
 *
 * @package doj_migration
 * @subpackage usao-pr
 * @see Obtainer.api.php
 */
class UsaoPrObtainDate extends ObtainDate {
}
