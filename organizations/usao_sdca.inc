<?php

/**
 * @file
 * Defines migration classes for Southern District of California.
 */

/**
 * Migrates .html files from /usao-sdca to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdca
 */
class UsaoSdcaPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/cas',
      1 => 'usao/cas/employ',
      2 => 'usao/cas/employ/2013',
      3 => 'usao/cas/employ/2014',
      4 => 'usao/cas/employ/2015',
      10 => 'usao/cas/comm',
      11 => 'usao/cas/vicwit',
      12 => 'usao/cas/vicwit/vw',
      13 => 'usao/cas/vicwit/vw/cases',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoSdcaPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdcaObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('#content h1'));
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));

    $title->addMethod('pluckAnySelectorUntilValid', array('#casdjobs div'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Southern District of California.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdca')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/cas/index.html',
      '/usao/cas/vicwit/vw/success.html',
      '/usao/cas/vicwit/vw/cases/cat_v_dog.html',
      '/usao/cas/comm/commTemplate.html',
      '/usao/cas/comm/title_image.html',
      'usao/cas/Untitled-2.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-sdca.
 *
 * @package doj_migration
 * @subpackage usao-sdca
 */
class UsaoSdcaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/cas subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/cas',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-sdca/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-sdca";
  }
}

/**
 * Migrates press releases from /usao-sdca.
 *
 * @package doj_migration
 * @subpackage usao-sdca
 */
class UsaoSdcaPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdca";
    $arguments['component_tid'] = 1736;
    $source_dirs = array(
      5 => 'usao/cas/press',
      6 => 'usao/cas/press/2014',
      7 => 'usao/cas/press/2014/trueblue',
      8 => 'usao/cas/press/2014/cryspal2',
      9 => 'usao/cas/press/2015',
      10 => 'usao/cas/press/2013',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoSdcaPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdcaObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline > p", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $subtitle = new ObtainerInfo('subtitle', "UsaoSdcaObtainTitlePressRelease");
    $subtitle->addMethod('pluckSelector', array("#headline > p > em", 1));
    $subtitle->addMethod('pluckSelector', array("#subheadl", 1));

    $date = new ObtainerInfo('date', "UsaoSdcaObtainDate");

    $date->addMethod('pluckSelector', array("#dateline", 1));
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');
    $date->addMethod('pluckSelector', array("#fir", 1));

    $pr_number = new ObtainerInfo('prNumber', "UsaoSdcaObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $subtitle;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    parent::__construct($arguments, $source_dirs, $options, $extension_regex);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/cas/press/2015/newsFillin.html',
      '/usao/cas/press/newsHelp.html',
      '/usao/cas/press/news.html',
      '/usao/cas/press/newsOld.html',
      '/usao/cas/press/ECvideo.html',
      '/usao/cas/press/newsRSS.html',
      '/usao/cas/press/newsrss1.html',

    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-sdca pages.
 *
 * @package doj_migration
 * @subpackage usao-sdca
 */
class UsaoSdcaPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-sdca press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdca
 */
class UsaoSdcaPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    HtmlCleanUp::removeTableBackgrounds($this->queryPath);
  }
}

/**
 * Obtainer for title property for /usao-sdca pages.
 *
 * @package doj_migration
 * @subpackage usao-sdca
 * @see Obtainer.api.php
 */
class UsaoSdcaObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-sdca press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdca
 * @see Obtainer.api.php
 */
class UsaoSdcaObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-sdca press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdca
 * @see Obtainer.api.php
 */
class UsaoSdcaObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-sdca.
 *
 * @package doj_migration
 * @subpackage usao-sdca
 * @see Obtainer.api.php
 */
class UsaoSdcaObtainDate extends ObtainDate {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      '–',
      'Date',
    );
    $text = str_ireplace($remove, '', $text);

    $swap = array(
      '2105',
    );
    $text = str_ireplace($swap, '2015', $text);

    return $text;
  }

}
