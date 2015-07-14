<?php

/**
 * @file
 * Defines migration classes for District of Wyoming.
 */

/**
 * Migrates .html files from /usao-wy to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wy
 */
class UsaoWyPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/wy/programs/victimwitness',
      1 => 'usao/wy/programs/lecc',
      2 => 'usao/wy/programs',
      3 => 'usao/wy/divisions/civil',
      4 => 'usao/wy/divisions',
      5 => 'usao/wy/divisions/criminal',
      6 => 'usao/wy',
      41 => 'usao/wy/usattorney',
      42 => 'usao/wy/about',
      43 => 'usao/wy/contactus',
      44 => 'usao/wy/jobs',
      45 => 'usao/wy/news',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoWyPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWyObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array('div[align="center"] > p > strong', 1));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $body = new ObtainerInfo('body', 'UsaoWyObtainBody');
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $body;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Wyoming.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wy')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/wy/index.html',
      '/usao/wy/news.html',
      '/usao/wy/news/news_index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-wy.
 *
 * @package doj_migration
 * @subpackage usao-wy
 */
class UsaoWyFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/wy subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/wy',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-wy/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-wy";
  }
}

/**
 * Migrates press releases from /usao-wy.
 *
 * @package doj_migration
 * @subpackage usao-wy
 */
class UsaoWyPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wy";
    $arguments['component_tid'] = 2156;
    $source_dirs = array(
      7 => 'usao/wy/news/2014/06june',
      8 => 'usao/wy/news/2014/04april',
      9 => 'usao/wy/news/2014/03march',
      10 => 'usao/wy/news/2014/11november',
      11 => 'usao/wy/news/2014/10october',
      12 => 'usao/wy/news/2014/05may',
      13 => 'usao/wy/news/2014',
      14 => 'usao/wy/news/2014/08august',
      15 => 'usao/wy/news/2014/01january',
      16 => 'usao/wy/news/2014/02february',
      17 => 'usao/wy/news/2014/07july',
      18 => 'usao/wy/news/2014/12december',
      19 => 'usao/wy/news/2014/09september',
      20 => 'usao/wy/news/2015/06june',
      21 => 'usao/wy/news/2015/04april',
      22 => 'usao/wy/news/2015/03march',
      23 => 'usao/wy/news/2015/05may',
      24 => 'usao/wy/news/2015/01january',
      25 => 'usao/wy/news/2015',
      26 => 'usao/wy/news/2015/02february',
      27 => 'usao/wy/news',
      28 => 'usao/wy/news/2013',
      29 => 'usao/wy/news/2013/06june',
      30 => 'usao/wy/news/2013/04april',
      31 => 'usao/wy/news/2013/03march',
      32 => 'usao/wy/news/2013/11november',
      33 => 'usao/wy/news/2013/10october',
      34 => 'usao/wy/news/2013/05may',
      35 => 'usao/wy/news/2013/08august',
      36 => 'usao/wy/news/2013/01january',
      37 => 'usao/wy/news/2013/02february',
      38 => 'usao/wy/news/2013/07july',
      39 => 'usao/wy/news/2013/12december',
      40 => 'usao/wy/news/2013/09september',
      41 => 'usao/wy/news/2015/07july',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoWyPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWyObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array('div[align="center"] > p', 1));
    $title->addMethod('pluckSelector', array('div[align="center"] > p > strong', 1));

    $title->addMethod('pluckSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoWyObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoWyObtainPrNumber");

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
      '/usao/wy/news/2014/04april/14-028_apr21.html',
      '/usao/wy/news/2014/04april/14-029_apr24.html',
      '/usao/wy/news/news_index.html',
      '/usao/wy/news/2015/news_index_2015.html',
      '/usao/wy/news/2014/news_index_2014.html',
      '/usao/wy/news/2013/news_index_2013.html',
      '/usao/wy/news/conferencestrainingoutreach.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-wy pages.
 *
 * @package doj_migration
 * @subpackage usao-wy
 */
class UsaoWyPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-wy press releases.
 *
 * @package doj_migration
 * @subpackage usao-wy
 */
class UsaoWyPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-wy pages.
 *
 * @package doj_migration
 * @subpackage usao-wy
 * @see Obtainer.api.php
 */
class UsaoWyObtainTitlePage extends ObtainTitle {

}

/**
 * Obtainer for title property for /usao-wy press releases.
 *
 * @package doj_migration
 * @subpackage usao-wy
 * @see Obtainer.api.php
 */
class UsaoWyObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-wy press releases.
 *
 * @package doj_migration
 * @subpackage usao-wy
 * @see Obtainer.api.php
 */
class UsaoWyObtainPrNumber extends ObtainId {
  /**
   * Finds the press release number from markup.
   */
  protected function findPrNumber() {
    $text = '';

    return $text;
  }
}

/**
 * Custom obtainer for date property for /usao-wy.
 *
 * @package doj_migration
 * @subpackage usao-wy
 * @see Obtainer.api.php
 */
class UsaoWyObtainDate extends ObtainDate {
}

/**
 * Obtainer for body property for /usao-wy basic pages.
 *
 * @package doj_migration
 * @subpackage usao-wy
 * @see Obtainer.api.php
 */
class UsaoWyObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $remove = array(
      'align="justify"',
    );
    $text = str_ireplace($remove, '', $text);
    $text = parent::cleanString($text);

    return $text;
  }
}
