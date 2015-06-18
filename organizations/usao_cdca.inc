<?php

/**
 * @file
 * Defines migration classes for Central District of California.
 */

/**
 * Migrates .html files from /usao-cdca to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-cdca
 */
class UsaoCdcaPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/cac',
      1 => 'usao/cac/community',
      2 => 'usao/cac/divisions',
      8 => 'usao/cac/Recent',
      9 => 'usao/cac/howcanwehelp/idtheft',
      10 => 'usao/cac/vw',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoCdcaPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoCdcaObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Central District of California.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-cdca')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/cac/news.html',
      '/usao/cac/news2011.html',
      '/usao/cac/news2012.html',
      '/usao/cac/news2013.html',
      '/usao/cac/news2014.html',
      '/usao/cac/index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-cdca.
 *
 * @package doj_migration
 * @subpackage usao-cdca
 */
class UsaoCdcaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/cac subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/cac',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-cdca/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-cdca";
  }
}

/**
 * Migrates press releases from /usao-cdca.
 *
 * @package doj_migration
 * @subpackage usao-cdca
 */
class UsaoCdcaPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-cdca";
    $arguments['component_tid'] = 1721;
    $source_dirs = array(
      3 => 'usao/cac/Pressroom/2014',
      4 => 'usao/cac/Pressroom/pr2011',
      5 => 'usao/cac/Pressroom/2015',
      6 => 'usao/cac/Pressroom/2013',
      7 => 'usao/cac/Pressroom',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoCdcaPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoCdcaObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("center > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("center > p", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoCdcaObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');
    $date->addMethod('pluckSelector', array("#content > p", 2));

    $pr_number = new ObtainerInfo('prNumber', "UsaoCdcaObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    parent::__construct($arguments, $source_dirs, $options, $extension_regex);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/cac/Pressroom/2015/news.html',
      '/usao/cac/Pressroom/pr2011/index.html',
      '/usao/cac/Pressroom/index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-cdca pages.
 *
 * @package doj_migration
 * @subpackage usao-cdca
 */
class UsaoCdcaPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-cdca press releases.
 *
 * @package doj_migration
 * @subpackage usao-cdca
 */
class UsaoCdcaPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-cdca pages.
 *
 * @package doj_migration
 * @subpackage usao-cdca
 * @see Obtainer.api.php
 */
class UsaoCdcaObtainTitlePage extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'The United States Attorney\'s Office',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}

/**
 * Obtainer for title property for /usao-cdca press releases.
 *
 * @package doj_migration
 * @subpackage usao-cdca
 * @see Obtainer.api.php
 */
class UsaoCdcaObtainTitlePressRelease extends ObtainTitlePressRelease {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'The United States Attorney\'s Office',
      'Central District Of California',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}

/**
 * Obtainer for prNumber property for /usao-cdca press releases.
 *
 * @package doj_migration
 * @subpackage usao-cdca
 * @see Obtainer.api.php
 */
class UsaoCdcaObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-cdca.
 *
 * @package doj_migration
 * @subpackage usao-cdca
 * @see Obtainer.api.php
 */
class UsaoCdcaObtainDate extends ObtainDate {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $text = str_replace('Julu', 'July', $text);

    return $text;
  }
}
