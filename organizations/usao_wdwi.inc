<?php

/**
 * @file
 * Defines migration classes for Western District of Wisconsin.
 */

/**
 * Migrates .html files from /usao-wdwi to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdwi
 */
class UsaoWdwiPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/wiw',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoWdwiPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWdwiObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Western District of Wisconsin.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wdwi')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/wiw/news2009.html',
      '/usao/wiw/news2010.html',
      '/usao/wiw/news2011.html',
      '/usao/wiw/news2012.html',
      '/usao/wiw/news2013.html',
      '/usao/wiw/news2014.html',
      '/usao/wiw/news2015.html',
      '/usao/wiw/index.html',
      '',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-wdwi.
 *
 * @package doj_migration
 * @subpackage usao-wdwi
 */
class UsaoWdwiFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/wiw subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/wiw',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-wdwi/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-wdwi";
  }
}

/**
 * Migrates press releases from /usao-wdwi.
 *
 * @package doj_migration
 * @subpackage usao-wdwi
 */
class UsaoWdwiPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdwi";
    $arguments['component_tid'] = 2141;
    $source_dirs = array(
      1 => 'usao/wiw/press/2013',
      2 => 'usao/wiw/press/2014',
      3 => 'usao/wiw/press/2015',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoWdwiPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWdwiObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoWdwiObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoWdwiObtainPrNumber");

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
 * SourceParser for /usao-wdwi pages.
 *
 * @package doj_migration
 * @subpackage usao-wdwi
 */
class UsaoWdwiPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-wdwi press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdwi
 */
class UsaoWdwiPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-wdwi pages.
 *
 * @package doj_migration
 * @subpackage usao-wdwi
 * @see Obtainer.api.php
 */
class UsaoWdwiObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-wdwi press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdwi
 * @see Obtainer.api.php
 */
class UsaoWdwiObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-wdwi press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdwi
 * @see Obtainer.api.php
 */
class UsaoWdwiObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-wdwi.
 *
 * @package doj_migration
 * @subpackage usao-wdwi
 * @see Obtainer.api.php
 */
class UsaoWdwiObtainDate extends ObtainDate {
}
