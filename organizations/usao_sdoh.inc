<?php

/**
 * @file
 * Defines migration classes for Southern District of Ohio.
 */

/**
 * Migrates .html files from /usao-sdoh to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdoh
 */
class UsaoSdohPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ohs',
      // 1 => 'usao/ohs/psn',  Public service videos intentionally skipped.
      // 2 => 'usao/ohs/spanish', Spanish pages for a separate migration.
      4 => 'usao/ohs/images',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoSdohPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdohObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Southern District of Ohio.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdoh')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/ohs/images/index.htm',
      '/usao/ohs/index.html',
      '/usao/ohs/news_2012.html',
      '/usao/ohs/news_2011.html',
      '/usao/ohs/news_2010.html',
      '/usao/ohs/news_2013.html',
      '/usao/ohs/news_2014.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-sdoh.
 *
 * @package doj_migration
 * @subpackage usao-sdoh
 */
class UsaoSdohFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ohs subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ohs',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-sdoh/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-sdoh";
  }
}

/**
 * Migrates press releases from /usao-sdoh.
 *
 * @package doj_migration
 * @subpackage usao-sdoh
 */
class UsaoSdohPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdoh";
    $arguments['component_tid'] = 1996;
    $source_dirs = array(
      3 => 'usao/ohs/news',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoSdohPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdohObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > div > p[align='center'] > u > strong", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > div[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));
    // One offs.
    $title->addMethod('pluckSelector', array(".contentSub > div > div > p[align='center']", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > div[align='center'] > p > strong > u", 1));
    $title->addMethod('pluckSelector', array('h4', 1));

    $subtitle = new ObtainerInfo('subtitle');
    $subtitle->addMethod('pluckSelector', array(".contentSub > div > div > p[align='center'] > em", 1));

    $date = new ObtainerInfo('date', "UsaoSdohObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoSdohObtainPrNumber");
    $body = new ObtainerInfo('body', 'UsaoSdohObtainBody');
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $subtitle;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;
    $arguments['obtainers_info'][] = $body;

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

    // Skip any press releases prior to 2013.
    if (!doj_migration_date_after($row->field_pr_date, '12/31/2012', FALSE)) {
      $message = '@fileid ------> Dated prior to 2013. Skipped: intentionally.';
      $this->migrationMessage($message, array('@fileid' => $row->fileid), WATCHDOG_WARNING);
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-sdoh pages.
 *
 * @package doj_migration
 * @subpackage usao-sdoh
 */
class UsaoSdohPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-sdoh press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdoh
 */
class UsaoSdohPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-sdoh pages.
 *
 * @package doj_migration
 * @subpackage usao-sdoh
 * @see Obtainer.api.php
 */
class UsaoSdohObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-sdoh press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdoh
 * @see Obtainer.api.php
 */
class UsaoSdohObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-sdoh press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdoh
 * @see Obtainer.api.php
 */
class UsaoSdohObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-sdoh.
 *
 * @package doj_migration
 * @subpackage usao-sdoh
 * @see Obtainer.api.php
 */
class UsaoSdohObtainDate extends ObtainDate {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'http://www justice gov/usao/ohs/',
      'http://www justice gov/usao/ohs',
    );
    $text = str_ireplace($remove, '', $text);
    $text = str_ireplace('2103', '2013', $text);

    return $text;
  }
}

/**
 * Custom obtainer for body property for /usao-sdoh.
 *
 * @package doj_migration
 * @subpackage usao-sdoh
 * @see Obtainer.api.php
 */
class UsaoSdohObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'CONTACT: Jennifer Thornton',
      '(614) 469-5715',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}
