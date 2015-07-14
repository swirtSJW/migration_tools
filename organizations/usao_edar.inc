<?php

/**
 * @file
 * Defines migration classes for Eastern District of Arkansas.
 */

/**
 * Migrates .html files from /usao-edar to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edar
 */
class UsaoEdarPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/are',
      30 => 'usao/are/vw',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoEdarPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoEdarObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of Arkansas.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edar')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/are/index.html',
      '/usao/are/sitemap.html',
      '/usao/are/index.html',
      '/usao/are/news.html',
      '/usao/are/2005news_index.html',
      '/usao/are/2006news_index.html',
      '/usao/are/2007news_index.html',
      '/usao/are/2008news_index.html',
      '/usao/are/2009news_index.html',
      '/usao/are/2010news_index.html',
      '/usao/are/2011news_index.html',
      '/usao/are/2012news_index.html',
      '/usao/are/2013news_index.html',
      '/usao/are/2014news_index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-edar.
 *
 * @package doj_migration
 * @subpackage usao-edar
 */
class UsaoEdarFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/are subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/are',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-edar/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-edar";
  }
}

/**
 * Migrates press releases from /usao-edar.
 *
 * @package doj_migration
 * @subpackage usao-edar
 */
class UsaoEdarPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edar";
    $arguments['component_tid'] = 1706;
    $source_dirs = array(
      1 => 'usao/are/news/2014/August',
      2 => 'usao/are/news/2014/September',
      3 => 'usao/are/news/2014/November',
      4 => 'usao/are/news/2014/April',
      5 => 'usao/are/news/2014/January',
      6 => 'usao/are/news/2014/July',
      7 => 'usao/are/news/2014/October',
      8 => 'usao/are/news/2014/March',
      9 => 'usao/are/news/2014/May',
      10 => 'usao/are/news/2014/February',
      11 => 'usao/are/news/2014/December',
      12 => 'usao/are/news/2015/June',
      13 => 'usao/are/news/2015/April',
      14 => 'usao/are/news/2015/January',
      15 => 'usao/are/news/2015/March',
      16 => 'usao/are/news/2015/May',
      17 => 'usao/are/news/2015/February',
      18 => 'usao/are/news/2013/August',
      19 => 'usao/are/news/2013/June',
      20 => 'usao/are/news/2013/September',
      21 => 'usao/are/news/2013/November',
      22 => 'usao/are/news/2013/April',
      23 => 'usao/are/news/2013/January',
      24 => 'usao/are/news/2013/July',
      25 => 'usao/are/news/2013/October',
      26 => 'usao/are/news/2013/March',
      27 => 'usao/are/news/2013/May',
      28 => 'usao/are/news/2013/February',
      29 => 'usao/are/news/2013/December',
      30 => 'usao/are/news/2015/July',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoEdarPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoEdarObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoEdarObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoEdarObtainPrNumber");

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
 * SourceParser for /usao-edar pages.
 *
 * @package doj_migration
 * @subpackage usao-edar
 */
class UsaoEdarPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-edar press releases.
 *
 * @package doj_migration
 * @subpackage usao-edar
 */
class UsaoEdarPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-edar pages.
 *
 * @package doj_migration
 * @subpackage usao-edar
 * @see Obtainer.api.php
 */
class UsaoEdarObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-edar press releases.
 *
 * @package doj_migration
 * @subpackage usao-edar
 * @see Obtainer.api.php
 */
class UsaoEdarObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-edar press releases.
 *
 * @package doj_migration
 * @subpackage usao-edar
 * @see Obtainer.api.php
 */
class UsaoEdarObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-edar.
 *
 * @package doj_migration
 * @subpackage usao-edar
 * @see Obtainer.api.php
 */
class UsaoEdarObtainDate extends ObtainDate {
}
