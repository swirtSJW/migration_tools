<?php

/**
 * @file
 * Defines migration classes for District of Deleware.
 */

/**
 * Migrates .html files from /usao-de to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-de
 */
class UsaoDePageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/de',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoDePageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoDeObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Deleware.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-de')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/de/index.html',
      '/usao/de/news.html',
      '/usao/de/news_2010.html',
      '/usao/de/news_2011.html',
      '/usao/de/news_2012.html',
      '/usao/de/news_2013.html',
      '/usao/de/news_2014.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-de.
 *
 * @package doj_migration
 * @subpackage usao-de
 */
class UsaoDeFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/de subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/de',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-de/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-de";
  }
}

/**
 * Migrates press releases from /usao-de.
 *
 * @package doj_migration
 * @subpackage usao-de
 */
class UsaoDePressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-de";
    $arguments['component_tid'] = 1756;
    $source_dirs = array(
      1 => 'usao/de/news/2014',
      2 => 'usao/de/news/2015',
      3 => 'usao/de/news/2013',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoDePressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoDeObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoDeObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoDeObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    parent::__construct($arguments, $source_dirs, $options, $extension_regex);
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
 * SourceParser for /usao-de pages.
 *
 * @package doj_migration
 * @subpackage usao-de
 */
class UsaoDePageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-de press releases.
 *
 * @package doj_migration
 * @subpackage usao-de
 */
class UsaoDePressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-de pages.
 *
 * @package doj_migration
 * @subpackage usao-de
 * @see Obtainer.api.php
 */
class UsaoDeObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-de press releases.
 *
 * @package doj_migration
 * @subpackage usao-de
 * @see Obtainer.api.php
 */
class UsaoDeObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-de press releases.
 *
 * @package doj_migration
 * @subpackage usao-de
 * @see Obtainer.api.php
 */
class UsaoDeObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-de.
 *
 * @package doj_migration
 * @subpackage usao-de
 * @see Obtainer.api.php
 */
class UsaoDeObtainDate extends ObtainDate {
}
