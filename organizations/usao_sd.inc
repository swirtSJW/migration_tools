<?php

/**
 * @file
 * Defines migration classes for District of South Dakota.
 */

/**
 * Migrates .html files from /usao-sd to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sd
 */
class UsaoSdPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/sd',
    );
    $options = array('recurse' => FALSE);

    $arguments['source_parser_class'] = "UsaoSdPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));

    $arguments['obtainers_info'][] = $title;

    $extension_regex = '/.*\.(html|htm)$/i';

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of South Dakota.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sd')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/sd/news-2010.html',
      '/usao/sd/news-2011.html',
      '/usao/sd/news-2012.html',
      '/usao/sd/news-2013.html',
      '/usao/sd/news-2014.html',
      '/usao/sd/news.html',
      '/usao/sd/index.html',
      '/usao/sd/contact-x.html',

    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-sd.
 *
 * @package doj_migration
 * @subpackage usao-sd
 */
class UsaoSdFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/sd subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/sd',
    );

    $regex = NULL;
    $dest_dir = 'public:///usao-sd/docs';

    $options = array('recurse' => TRUE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-sd";
  }
}

/**
 * Migrates press releases from /usao-sd.
 *
 * @package doj_migration
 * @subpackage usao-sd
 */
class UsaoSdPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sd";
    $arguments['component_tid'] = 2056;
    $arguments['source_parser_class'] = "UsaoSdPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoSdObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoSdObtainPrNumber");

    $body = new ObtainerInfo('body', 'UsaoSdPressreleaseObtainBody');
    $body->addMethod('pluckSelector', array("td[width='650']", 2, 'innerHTML'));
    $body->addMethod('pluckSelector', array("td[width='650']", 1, 'innerHTML'));
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;
    $arguments['obtainers_info'][] = $body;

    $source_dirs = array(
      3 => 'usao/sd/pressreleases/2013',
      4 => 'usao/sd/pressreleases/2014',
      5 => 'usao/sd/pressreleases/2015',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';
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
 * SourceParser for /usao-sd pages.
 *
 * @package doj_migration
 * @subpackage usao-sd
 */
class UsaoSdPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-sd press releases.
 *
 * @package doj_migration
 * @subpackage usao-sd
 */
class UsaoSdPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-sd pages.
 *
 * @package doj_migration
 * @subpackage usao-sd
 * @see Obtainer.api.php
 */
class UsaoSdObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-sd press releases.
 *
 * @package doj_migration
 * @subpackage usao-sd
 * @see Obtainer.api.php
 */
class UsaoSdObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-sd press releases.
 *
 * @package doj_migration
 * @subpackage usao-sd
 * @see Obtainer.api.php
 */
class UsaoSdObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-sd.
 *
 * @package doj_migration
 * @subpackage usao-sd
 * @see Obtainer.api.php
 */
class UsaoSdObtainDate extends ObtainDate {
}

/**
 * Obtainer for body property for /usao-sd press releases.
 *
 * @package doj_migration
 * @subpackage usao-sd
 * @see Obtainer.api.php
 */
class UsaoSdPressreleaseObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'Contact: Ace Crawford',
      "<br />\n        605-341-1915",
      '605-343-2913 ext. 2101',
    );

    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}
