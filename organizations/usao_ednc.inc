<?php

/**
 * @file
 * Defines migration classes for Eastern District of North Carolina.
 */

/**
 * Migrates .html files from /usao-ednc to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ednc
 */
class UsaoEdncPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/nce',
      5 => 'usao/nce/vwa',
      6 => 'usao/nce/military',
      7 => 'usao/nce/images',
      8 => 'usao/nce/espanol',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoEdncPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoEdncObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of North Carolina.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ednc')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/nce/news.html',
      '/usao/nce/index.html',
      '/usao/nce/slidesindex.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-ednc.
 *
 * @package doj_migration
 * @subpackage usao-ednc
 */
class UsaoEdncFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/nce subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/nce',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-ednc/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-ednc";
  }
}

/**
 * Migrates press releases from /usao-ednc.
 *
 * @package doj_migration
 * @subpackage usao-ednc
 */
class UsaoEdncPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ednc";
    $arguments['component_tid'] = 1926;
    $source_dirs = array(
      1 => 'usao/nce/press/2014',
      3 => 'usao/nce/press/2015',
      4 => 'usao/nce/press/2013',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoEdncPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoEdncObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoEdncObtainDate");
    $date->addMethod('pluckXpath', array("id('content')/div[1]/div[3]/p[2]/strong", 1));
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoEdncObtainPrNumber");

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
      '/usao/nce/press/2013/index.html',
      '/usao/nce/press/2014/index.html',
      '/usao/nce/press/2015/2015-may-12_02.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-ednc pages.
 *
 * @package doj_migration
 * @subpackage usao-ednc
 */
class UsaoEdncPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-ednc press releases.
 *
 * @package doj_migration
 * @subpackage usao-ednc
 */
class UsaoEdncPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-ednc pages.
 *
 * @package doj_migration
 * @subpackage usao-ednc
 * @see Obtainer.api.php
 */
class UsaoEdncObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-ednc press releases.
 *
 * @package doj_migration
 * @subpackage usao-ednc
 * @see Obtainer.api.php
 */
class UsaoEdncObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-ednc press releases.
 *
 * @package doj_migration
 * @subpackage usao-ednc
 * @see Obtainer.api.php
 */
class UsaoEdncObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-ednc.
 *
 * @package doj_migration
 * @subpackage usao-ednc
 * @see Obtainer.api.php
 */
class UsaoEdncObtainDate extends ObtainDate {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'Monday',
      'Tuesday',
      'Wednesday',
      'Thursday',
      'Friday',
      'Saturday',
      'Sunday',
    );
    $text = str_ireplace($remove, '', $text);
    $text = str_ireplace('Ferbruary', 'February', $text);

    return $text;
  }
}
