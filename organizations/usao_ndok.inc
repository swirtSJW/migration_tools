<?php

/**
 * @file
 * Defines migration classes for Northern District of Oklahoma.
 */

/**
 * Migrates .html files from /usao-ndok to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ndok
 */
class UsaoNdokPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/okn',
      1 => 'usao/okn/victim_witness',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoNdokPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdokObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Northern District of Oklahoma.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ndok')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/okn/index.html',
      '/usao/okn/sitemap.html',
      '/usao/okn/news.html',
      '/usao/okn/news_2010.html',
      '/usao/okn/news_2011.html',
      '/usao/okn/news_2012.html',
      '/usao/okn/news_2013.html',
      '/usao/okn/news_2014.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-ndok.
 *
 * @package doj_migration
 * @subpackage usao-ndok
 */
class UsaoNdokFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/okn subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/okn',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-ndok/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-ndok";
  }
}

/**
 * Migrates press releases from /usao-ndok.
 *
 * @package doj_migration
 * @subpackage usao-ndok
 */
class UsaoNdokPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ndok";
    $arguments['component_tid'] = 2006;
    $source_dirs = array(
      2 => 'usao/okn/news/2014',
      3 => 'usao/okn/news/2015',
      4 => 'usao/okn/news/2013',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoNdokPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdokObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array("p.style1", 1));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoNdokObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckSelector', array("div[align='right']", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoNdokObtainPrNumber");

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
 * SourceParser for /usao-ndok pages.
 *
 * @package doj_migration
 * @subpackage usao-ndok
 */
class UsaoNdokPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-ndok press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndok
 */
class UsaoNdokPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-ndok pages.
 *
 * @package doj_migration
 * @subpackage usao-ndok
 * @see Obtainer.api.php
 */
class UsaoNdokObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-ndok press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndok
 * @see Obtainer.api.php
 */
class UsaoNdokObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-ndok press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndok
 * @see Obtainer.api.php
 */
class UsaoNdokObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-ndok.
 *
 * @package doj_migration
 * @subpackage usao-ndok
 * @see Obtainer.api.php
 */
class UsaoNdokObtainDate extends ObtainDate {
}
