<?php

/**
 * @file
 * Defines migration classes for Central District of Illinois.
 */

/**
 * Migrates .html files from /usao-cdil to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-cdil
 */
class UsaoCdilPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ilc',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoCdilPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoCdilObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Central District of Illinois.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-cdil')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/ilc/index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-cdil.
 *
 * @package doj_migration
 * @subpackage usao-cdil
 */
class UsaoCdilFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ilc subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ilc',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-cdil/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-cdil";
  }
}

/**
 * Migrates press releases from /usao-cdil.
 *
 * @package doj_migration
 * @subpackage usao-cdil
 */
class UsaoCdilPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-cdil";
    $arguments['component_tid'] = 1816;
    $source_dirs = array(
      1 => 'usao/ilc/press',
      2 => 'usao/ilc/press/2014/06june',
      3 => 'usao/ilc/press/2014/04april',
      4 => 'usao/ilc/press/2014/03march',
      5 => 'usao/ilc/press/2014/11november',
      6 => 'usao/ilc/press/2014/10october',
      7 => 'usao/ilc/press/2014/05may',
      8 => 'usao/ilc/press/2014/08august',
      9 => 'usao/ilc/press/2014/01january',
      10 => 'usao/ilc/press/2014/02february',
      11 => 'usao/ilc/press/2014/07july',
      12 => 'usao/ilc/press/2014/12december',
      13 => 'usao/ilc/press/2014/09september',
      14 => 'usao/ilc/press/2015/04april',
      15 => 'usao/ilc/press/2015/03march',
      16 => 'usao/ilc/press/2015/05may',
      17 => 'usao/ilc/press/2015/01january',
      18 => 'usao/ilc/press/2015/02february',
      19 => 'usao/ilc/press/2013/06june',
      20 => 'usao/ilc/press/2013/04april',
      21 => 'usao/ilc/press/2013/03march',
      22 => 'usao/ilc/press/2013/11november',
      23 => 'usao/ilc/press/2013/10october',
      24 => 'usao/ilc/press/2013/05may',
      25 => 'usao/ilc/press/2013/08august',
      26 => 'usao/ilc/press/2013/01january',
      27 => 'usao/ilc/press/2013/02february',
      28 => 'usao/ilc/press/2013/07july',
      29 => 'usao/ilc/press/2013/12december',
      30 => 'usao/ilc/press/2013/09september',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoCdilPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoCdilObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("div[align='center'] > p[align='center']", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoCdilObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoCdilObtainPrNumber");

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
      '/usao/ilc/press/index_old.html',
      '/usao/ilc/press/index.html',
      '/usao/ilc/press/2013index.html',
      '/usao/ilc/press/2014index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-cdil pages.
 *
 * @package doj_migration
 * @subpackage usao-cdil
 */
class UsaoCdilPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-cdil press releases.
 *
 * @package doj_migration
 * @subpackage usao-cdil
 */
class UsaoCdilPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-cdil pages.
 *
 * @package doj_migration
 * @subpackage usao-cdil
 * @see Obtainer.api.php
 */
class UsaoCdilObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-cdil press releases.
 *
 * @package doj_migration
 * @subpackage usao-cdil
 * @see Obtainer.api.php
 */
class UsaoCdilObtainTitlePressRelease extends ObtainTitlePressRelease {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      '<strong>',
      '</strong>',
      '<br>',
      '</br>',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}

/**
 * Obtainer for prNumber property for /usao-cdil press releases.
 *
 * @package doj_migration
 * @subpackage usao-cdil
 * @see Obtainer.api.php
 */
class UsaoCdilObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-cdil.
 *
 * @package doj_migration
 * @subpackage usao-cdil
 * @see Obtainer.api.php
 */
class UsaoCdilObtainDate extends ObtainDate {
}
