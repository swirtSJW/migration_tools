<?php

/**
 * @file
 * Defines migration classes for District of Colorado.
 */

/**
 * Migrates .html files from /usao-co to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-co
 */
class UsaoCoPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/co',
      43 => 'usao/co/caseupdates',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoCoPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoCoObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Colorado.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-co')
    ->sourceMigration('UsaoDistrict');
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
 * Migrates files (*.pdf) from /usao-co.
 *
 * @package doj_migration
 * @subpackage usao-co
 */
class UsaoCoFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/co subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/co',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-co/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-co";
  }
}

/**
 * Migrates press releases from /usao-co.
 *
 * @package doj_migration
 * @subpackage usao-co
 */
class UsaoCoPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-co";
    $arguments['component_tid'] = 1741;
    $source_dirs = array(
      1 => 'usao/co/news/2013/aug',
      2 => 'usao/co/news/2013/nov',
      3 => 'usao/co/news/2013/sept',
      4 => 'usao/co/news/2013/feb',
      5 => 'usao/co/news/2013/dec',
      6 => 'usao/co/news/2013/jun',
      7 => 'usao/co/news/2013/apr',
      8 => 'usao/co/news/2013/may',
      9 => 'usao/co/news/2013/mar',
      10 => 'usao/co/news/2013/oct',
      11 => 'usao/co/news/2013/jan',
      12 => 'usao/co/news/2013/july',
      13 => 'usao/co/news/2015/feb',
      14 => 'usao/co/news/2015/apr',
      15 => 'usao/co/news/2015/may',
      16 => 'usao/co/news/2015/mar',
      17 => 'usao/co/news/2015/jan',
      18 => 'usao/co/news/2011/January2011',
      19 => 'usao/co/news/2011/February2011',
      20 => 'usao/co/news/2011',
      21 => 'usao/co/news/2011/July2011',
      22 => 'usao/co/news/2011/April2011',
      23 => 'usao/co/news/2011/June2011',
      24 => 'usao/co/news/2011/August2011',
      25 => 'usao/co/news/2011/May2011',
      26 => 'usao/co/news/2011/September2011',
      27 => 'usao/co/news/2011/March2011',
      28 => 'usao/co/news/2011/December2011',
      29 => 'usao/co/news/2011/November2011',
      30 => 'usao/co/news/2011/October2011',
      31 => 'usao/co/news/2014/aug',
      32 => 'usao/co/news/2014/jul',
      33 => 'usao/co/news/2014/nov',
      34 => 'usao/co/news/2014/sept',
      35 => 'usao/co/news/2014/feb',
      36 => 'usao/co/news/2014/dec',
      37 => 'usao/co/news/2014/jun',
      38 => 'usao/co/news/2014/apr',
      39 => 'usao/co/news/2014/may',
      40 => 'usao/co/news/2014/mar',
      41 => 'usao/co/news/2014/oct',
      42 => 'usao/co/news/2014/jan',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoCoPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoCoObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));
    $title->addMethod('pluckSelector', array(".contentSub > p > strong", 1));

    $date = new ObtainerInfo('date', "UsaoCoObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckSelector', array(".contentSub > div > div > p[align='center']", 1));
    $date->addMethod('pluckSelector', array(".contentSub > div > p[align='center']", 1));
    $date->addMethod('pluckSelector', array(".contentSub > div > p[align='center']", 2));
    $date->addMethod('pluckSelector', array(".contentSub > div > p[align='center']", 3));
    $date->addMethod('pluckSelector', array(".contentSub > div > div > p[style='text-align:center']", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoCoObtainPrNumber");

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
      0 => '/usao/co/news/2011/October2011/index.html',
      1 => '/usao/co/news/2011/November2011/index.html',
      2 => '/usao/co/news/2011/December2011/index.html',
      3 => '/usao/co/news/2011/March2011/index.html',
      4 => '/usao/co/news/2011/September2011/index.html',
      5 => '/usao/co/news/2011/May2011/index.html',
      6 => '/usao/co/news/2011/August2011/index.html',
      7 => '/usao/co/news/2011/June2011/index.html',
      8 => '/usao/co/news/2011/April2011/index.html',
      9 => '/usao/co/news/2011/July2011/index.html',
      10 => '/usao/co/news/2011/index.html',
      11 => '/usao/co/news/2011/February2011/index.html',
      12 => '/usao/co/news/2011/January2011/index.html',
      13 => '/usao/co/news/2014/jun/6-17-14.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-co pages.
 *
 * @package doj_migration
 * @subpackage usao-co
 */
class UsaoCoPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-co press releases.
 *
 * @package doj_migration
 * @subpackage usao-co
 */
class UsaoCoPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-co pages.
 *
 * @package doj_migration
 * @subpackage usao-co
 * @see Obtainer.api.php
 */
class UsaoCoObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-co press releases.
 *
 * @package doj_migration
 * @subpackage usao-co
 * @see Obtainer.api.php
 */
class UsaoCoObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-co press releases.
 *
 * @package doj_migration
 * @subpackage usao-co
 * @see Obtainer.api.php
 */
class UsaoCoObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-co.
 *
 * @package doj_migration
 * @subpackage usao-co
 * @see Obtainer.api.php
 */
class UsaoCoObtainDate extends ObtainDate {
}
