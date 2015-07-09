<?php

/**
 * @file
 * Defines migration classes for Western District of Pennsylvania.
 */

/**
 * Migrates .html files from /usao-wdpa to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 */
class UsaoWdpaPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/paw',
      32 => 'usao/paw/vw',
      33 => 'usao/paw/vw/cases',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoWdpaPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWdpaObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $type = new ObtainerInfo('content_type', "ObtainContentType");
    $type->addMethod('findClassBreadcrumbSpeech');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $type;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Western District of Pennsylvania.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wdpa')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/paw/index.html',
      '/usao/paw/news.html',
      '/usao/paw/index-slideshow.html',
      '/usao/paw/sitemap.html',
      '/usao/paw/archives.html',
      '/usao/paw/archives2011.html',
      '/usao/paw/archives2012.html',
      '/usao/paw/archives2013.html',
      '/usao/paw/archives2014.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || (parent::prepareRow($row) === FALSE) || (self::isType('speech', $row))) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-wdpa.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 */
class UsaoWdpaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/paw subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/paw',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-wdpa/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-wdpa";
  }
}

/**
 * Migrates press releases from /usao-wdpa.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 */
class UsaoWdpaPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdpa";
    $arguments['component_tid'] = 2036;
    $source_dirs = array(
      1 => 'usao/paw/news/2014/2014_october',
      2 => 'usao/paw/news/2014/2014_june',
      3 => 'usao/paw/news/2014/2014_march',
      4 => 'usao/paw/news/2014/2014_april',
      5 => 'usao/paw/news/2014/2014_august',
      6 => 'usao/paw/news/2014/2014_december',
      7 => 'usao/paw/news/2014/2014_july',
      8 => 'usao/paw/news/2014/2014_february',
      9 => 'usao/paw/news/2014/2014_january',
      10 => 'usao/paw/news/2014/2014_may',
      11 => 'usao/paw/news/2014/2014_november',
      12 => 'usao/paw/news/2014/2014_september',
      13 => 'usao/paw/news/2015/2015_february',
      14 => 'usao/paw/news/2015/2015_june',
      15 => 'usao/paw/news/2015/2015_may',
      16 => 'usao/paw/news/2015/2015_january',
      17 => 'usao/paw/news/2015/2015_july',
      18 => 'usao/paw/news/2015/2015_march',
      19 => 'usao/paw/news/2015/2015_april',
      20 => 'usao/paw/news/2013/2013_june',
      21 => 'usao/paw/news/2013/2013_october',
      22 => 'usao/paw/news/2013/2013_april',
      23 => 'usao/paw/news/2013/2013_november',
      24 => 'usao/paw/news/2013/2013_december',
      25 => 'usao/paw/news/2013/2013_january',
      26 => 'usao/paw/news/2013/2013_february',
      27 => 'usao/paw/news/2013/2013_july',
      28 => 'usao/paw/news/2013/2013_march',
      29 => 'usao/paw/news/2013/2013_may',
      30 => 'usao/paw/news/2013/2013_september',
      31 => 'usao/paw/news/2013/2013_august',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoWdpaPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWdpaObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoWdpaObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoWdpaObtainPrNumber");

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
 * SourceParser for /usao-wdpa pages.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 */
class UsaoWdpaPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-wdpa press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 */
class UsaoWdpaPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-wdpa pages.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 * @see Obtainer.api.php
 */
class UsaoWdpaObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-wdpa press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 * @see Obtainer.api.php
 */
class UsaoWdpaObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-wdpa press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 * @see Obtainer.api.php
 */
class UsaoWdpaObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-wdpa.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 * @see Obtainer.api.php
 */
class UsaoWdpaObtainDate extends ObtainDate {
}

/**
 * Migrates speeeches from /usao-wdpa.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 */
class UsaoWdpaSpeechMigration extends NGDistrictSpeechMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdpa";
    $arguments['component_tid'] = 2036;
    $arguments['source_parser_class'] = "UsaoWdpaSpeechSourceParser";

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/paw',
    );

    $regex = '/.*\.(htm|html|php)$/i';
    $options = array('recurse' => FALSE);

    // Configuring Obtainers.
    $body = new ObtainerInfo('body', 'UsaoWdpaSpeechObtainBody');
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $type = new ObtainerInfo('content_type', "ObtainContentType");
    $type->addMethod('findClassBreadcrumbSpeech');

    $arguments['obtainers_info'][] = $body;
    $arguments['obtainers_info'][] = $type;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/paw/remarks.html',
      '/usao/paw/index.html',
      '/usao/paw/news.html',
      '/usao/paw/index-slideshow.html',
      '/usao/paw/sitemap.html',
      '/usao/paw/archives.html',
      '/usao/paw/archives2011.html',
      '/usao/paw/archives2012.html',
      '/usao/paw/archives2013.html',
      '/usao/paw/archives2014.html',
    );

    if (doj_migration_skip_file($row->fileid, $skip_these) || (parent::prepareRow($row) === FALSE) || (!self::isType('speech', $row))) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-wdpa speeches.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 */
class UsaoWdpaSpeechSourceParser extends NGSpeechSourceParser {
  /**
   * Clean the html right before pulling the body.
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for body property for /usao-wdpa speech.
 *
 * @package doj_migration
 * @subpackage usao-wdpa
 * @see Obtainer.api.php
 */
class UsaoWdpaSpeechObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);

    return $text;
  }
}
