<?php

/**
 * @file
 * Defines migration classes for Northern District of Illinois.
 */

/**
 * Migrates .html files from /usao-ndil to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ndil
 */
class UsaoNdilPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/iln/programs/victimwitness',
      1 => 'usao/iln',
      2 => 'usao/iln/hot',
      3 => 'usao/iln/largecases',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoNdilPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdilObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $body = new ObtainerInfo('body', 'UsaoNdilObtainBody');
    $body->addMethod('pluckSelector', array('.contentSub', 1, 'html'));
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $body;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Northern District of Illinois.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ndil')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/iln/index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-ndil.
 *
 * @package doj_migration
 * @subpackage usao-ndil
 */
class UsaoNdilFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/iln subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/iln',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-ndil/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-ndil";
  }
}

/**
 * Migrates press releases from /usao-ndil.
 *
 * @package doj_migration
 * @subpackage usao-ndil
 */
class UsaoNdilPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ndil";
    $arguments['component_tid'] = 1821;
    $source_dirs = array(
      1 => 'usao/iln/pr/rockford/2014',
      2 => 'usao/iln/pr/rockford/2003',
      3 => 'usao/iln/pr/rockford/2015',
      4 => 'usao/iln/pr/rockford/2013',
      5 => 'usao/iln/pr/rockford/2004',
      6 => 'usao/iln/pr/rockford/2002',
      7 => 'usao/iln/pr/chicago/2014',
      8 => 'usao/iln/pr/chicago/2015',
      9 => 'usao/iln/pr/chicago/2013',
      10 => 'usao/iln/pr',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoNdilPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdilObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoNdilObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckSelector', array('td[align="right"]', 1));
    $date->addMethod('pluckProbableDate');

    $body = new ObtainerInfo('body', 'UsaoNdilObtainBody');
    $body->addMethod('pluckSelector', array("div[align='justify']", 1, 'html'));
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $pr_number = new ObtainerInfo('prNumber', "UsaoNdilObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;
    $arguments['obtainers_info'][] = $body;

    parent::__construct($arguments, $source_dirs, $options, $extension_regex);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/iln/pr/index.html',
      '/usao/iln/pr/chicago/2013/index.html',
      '/usao/iln/pr/chicago/2014/index.html',
      '/usao/iln/pr/chicago/2015/index.html',
      '/usao/iln/pr/rockford/2013/index.html',
      '/usao/iln/pr/rockford/2014/index.html',
      '/usao/iln/pr/rockford/2015/index.html',
      '/usao/iln/pr/rockford/2002/index.html',
      '/usao/iln/pr/rockford/2003/index.html',
      '/usao/iln/pr/rockford/2004/index.html',
      '/usao/iln/pr/rockford/2015/pr0313_01.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-ndil pages.
 *
 * @package doj_migration
 * @subpackage usao-ndil
 */
class UsaoNdilPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-ndil press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndil
 */
class UsaoNdilPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-ndil pages.
 *
 * @package doj_migration
 * @subpackage usao-ndil
 * @see Obtainer.api.php
 */
class UsaoNdilObtainTitlePage extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'Victim Witness Unit',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}

/**
 * Obtainer for title property for /usao-ndil press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndil
 * @see Obtainer.api.php
 */
class UsaoNdilObtainTitlePressRelease extends ObtainTitlePressRelease {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'Chicago News And Press Releases',
      'Rockford News And Press Releases',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}

/**
 * Obtainer for prNumber property for /usao-ndil press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndil
 * @see Obtainer.api.php
 */
class UsaoNdilObtainPrNumber extends ObtainId {
  /**
   * Finds the press release number from markup.
   */
  protected function findPrNumber() {
    $text = '';

    return $text;
  }
}

/**
 * Custom obtainer for date property for /usao-ndil.
 *
 * @package doj_migration
 * @subpackage usao-ndil
 * @see Obtainer.api.php
 */
class UsaoNdilObtainDate extends ObtainDate {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = str_ireplace("Jully", "July", $text);
    $text = str_ireplace("3014", "2014", $text);
    $text = str_ireplace("3013", "2013", $text);
    $text = str_ireplace("April, 24", "April 24", $text);

    return $text;
  }
}

/**
 * Obtainer for body property for /usao-ndil press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndil
 * @see Obtainer.api.php
 */
class UsaoNdilObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {

    $text = parent::cleanString($text);

    return $text;
  }
}
