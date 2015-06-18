<?php

/**
 * @file
 * Defines migration classes for District of Northern Texas.
 */

/**
 * Migrates .html files from /usao-ndtx to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ndtx
 */
class UsaoNdtxPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/txn',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoNdtxPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdtxObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array('style1 style2'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');
    $title->addMethod('pluckSelector', array("p[align='center'] > a > strong", 1));
    $title->addMethod('pluckSelector', array("p[align='center'] > strong > span[class='style1 style2']", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Northern Texas.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ndtx')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/txn/index.html',
      '/usao/txn/news.html',
      '/usao/txn/pressreleases.html',
      '/usao/txn/pressreleases06.html',
      '/usao/txn/pressreleases07.html',
      '/usao/txn/pressreleases08.html',
      '/usao/txn/pressreleases09.html',
      '/usao/txn/pressreleases10.html',
      '/usao/txn/pressreleases11.html',
      '/usao/txn/sitemap.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-ndtx.
 *
 * @package doj_migration
 * @subpackage usao-ndtx
 */
class UsaoNdtxFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/txn subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/txn',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-ndtx/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-ndtx";
  }
}

/**
 * Migrates press releases from /usao-ndtx.
 *
 * @package doj_migration
 * @subpackage usao-ndtx
 */
class UsaoNdtxPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ndtx";
    $arguments['component_tid'] = 2081;
    $source_dirs = array(
      1 => 'usao/txn/PressRelease/2014/MAR2014',
      2 => 'usao/txn/PressRelease/2014/JUN2014',
      3 => 'usao/txn/PressRelease/2014/OCT2014',
      4 => 'usao/txn/PressRelease/2014/SEP2014',
      5 => 'usao/txn/PressRelease/2014/MAY2014',
      6 => 'usao/txn/PressRelease/2014/NOV2014',
      7 => 'usao/txn/PressRelease/2014',
      8 => 'usao/txn/PressRelease/2014/DEC2014',
      9 => 'usao/txn/PressRelease/2014/APR2014',
      10 => 'usao/txn/PressRelease/2014/JAN2014',
      11 => 'usao/txn/PressRelease/2014/FEB2014',
      12 => 'usao/txn/PressRelease/2014/AUG2014',
      13 => 'usao/txn/PressRelease/2014/JUL2014',
      14 => 'usao/txn/PressRelease/2015/APR2015',
      15 => 'usao/txn/PressRelease/2015/MAY2015',
      16 => 'usao/txn/PressRelease/2015/MAR2015',
      17 => 'usao/txn/PressRelease/2015/JAN2015',
      18 => 'usao/txn/PressRelease/2015/FEB2015',
      19 => 'usao/txn/PressRelease/2013/OCT2013',
      20 => 'usao/txn/PressRelease/2013/AUG2013',
      21 => 'usao/txn/PressRelease/2013/MAY2013',
      22 => 'usao/txn/PressRelease/2013/APR2013',
      23 => 'usao/txn/PressRelease/2013/DEC2013',
      24 => 'usao/txn/PressRelease/2013',
      25 => 'usao/txn/PressRelease/2013/FEB2013',
      26 => 'usao/txn/PressRelease/2013/SEP2013',
      27 => 'usao/txn/PressRelease/2013/NOV2013',
      28 => 'usao/txn/PressRelease/2013/JUN2013',
      29 => 'usao/txn/PressRelease/2013/JUL2013',
      30 => 'usao/txn/PressRelease/2013/MAR2013',
      31 => 'usao/txn/PressRelease/2013/JAN2013',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoNdtxPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdtxObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $subtitle = new ObtainerInfo('subtitle');
    $subtitle->addMethod('pluckSelector', array('h2 > em', 1));

    $date = new ObtainerInfo('date', "UsaoNdtxObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoNdtxObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $subtitle;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    parent::__construct($arguments, $source_dirs, $options, $extension_regex);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/txn/PressRelease/2013/news2013.html',
      '/usao/txn/PressRelease/2014/news2014.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-ndtx pages.
 *
 * @package doj_migration
 * @subpackage usao-ndtx
 */
class UsaoNdtxPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-ndtx press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndtx
 */
class UsaoNdtxPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-ndtx pages.
 *
 * @package doj_migration
 * @subpackage usao-ndtx
 * @see Obtainer.api.php
 */
class UsaoNdtxObtainTitlePage extends ObtainTitle {
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
 * Obtainer for title property for /usao-ndtx press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndtx
 * @see Obtainer.api.php
 */
class UsaoNdtxObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-ndtx press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndtx
 * @see Obtainer.api.php
 */
class UsaoNdtxObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-ndtx.
 *
 * @package doj_migration
 * @subpackage usao-ndtx
 * @see Obtainer.api.php
 */
class UsaoNdtxObtainDate extends ObtainDate {
}
