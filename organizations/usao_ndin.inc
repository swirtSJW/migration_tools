<?php

/**
 * @file
 * Defines migration classes for Northern District of Indiana.
 */

/**
 * Migrates .html files from /usao-ndin to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ndin
 */
class UsaoNdinPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      17 => 'usao/inn',
    );

    $arguments['source_parser_class'] = "UsaoNdinPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdinObtainTitlePage");
    $title->addMethod('findH1Any');
    $title->addMethod('findClassBreadcrumbLast');
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Northern District of Indiana.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ndin')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/inn/COPY index.html',
      '/usao/inn/TEST index.html',
      '/usao/inn/news2010.html',
      '/usao/inn/Copy of outreach.html',
      '/usao/inn/ index.html',
      '/usao/inn/news2010.html',
      '/usao/inn/news2011.html',
      '/usao/inn/news2012.html',
      '/usao/inn/news2013.html',
      '/usao/inn/news.html',
      '/usao/inn/news-B.html',
      '/usao/inn/news2014.html',
    );

    if ((parent::prepareRow($row) === FALSE) || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-ndin.
 *
 * @package doj_migration
 * @subpackage usao-ndin
 */
class UsaoNdinFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/inn subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/inn',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-ndin/docs';

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
    return "usao-ndin";
  }
}

/**
 * Migrates press releases from /usao-ndin.
 *
 * @package doj_migration
 * @subpackage usao-ndin
 */
class UsaoNdinPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ndin";
    $arguments['component_tid'] = 1831;
    $arguments['source_parser_class'] = "UsaoNdinPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdinObtainTitlePressRelease");
    $title->addMethod('findH1Any');
    $title->addMethod('findBUAny');
    $title->addMethod('findSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('findSelector', array('h2', 1));
    $title->addMethod('findSelector', array('h2', 2));
    $title->addMethod('findSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('findSelector', array('p.MsoNormal > span[style="font-size:12.0pt;mso-fareast-font-family:Calibri;mso-ansi-language:EN-CA"]', 1));
    $title->addMethod('findSelector', array('p.MsoNormal > b > u > span[style="font-size:12.0pt;mso-ansi-language:EN-CA"]', 1));
    $title->addMethod('findSelector', array('p.MsoNormal > b > u > span[style="font-size:12.0pt;mso-ansi-language:EN-CA"]', 2));
    $title->addMethod('findSelector', array('p.MsoNormal > b > u > span[style="font-size:12.0pt;mso-ansi-language:EN-CA"]', 3));
    $title->addMethod('findSelector', array('p.s4', 1));
    $title->addMethod('findSelector', array("#headline", 1));

    $date = new ObtainerInfo('date', "UsaoNdinObtainDate");
    $date->addMethod('findSelector', array(".newsRight", 1));
    $date->addMethod('findSelector', array('p[align="center"] > strong'), 2);

    $date->addMethod('findSelector', array('p.MsoNormal > span[style="font-size:12.0pt"]'), 1);
    $date->addMethod('findSelector', array('p.MsoNormal > span[style="font-size:12.0pt"]'), 3);
    $date->addMethod('findSelector', array('p.MsoNormal > span[style="font-size:12.0pt"]'), 4);
    $date->addMethod('findSelector', array('p.MsoNormal > span[style="font-size:12.0pt"]'), 5);
    $date->addMethod('findSelector', array("p.MsoNormal > span[style='font-size:12.0pt']"), 1);
    $date->addMethod('findSelector', array("p.MsoNormal > span[style='font-size:12.0pt']"), 3);
    $date->addMethod('findSelector', array("p.MsoNormal > span[style='font-size:12.0pt']"), 4);
    $date->addMethod('findSelector', array("p.MsoNormal > span[style='font-size:12.0pt']"), 5);

    $date->addMethod('findPRDateAny');
    $date->addMethod('findProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoNdinObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    $source_dirs = array(
      0 => 'usao/inn/press_release/2014/May_14',
      1 => 'usao/inn/press_release/2014/Mar_14',
      2 => 'usao/inn/press_release/2014/Jan_14',
      3 => 'usao/inn/press_release/2014/Feb_14',
      4 => 'usao/inn/press_release/2014/Apr_14',
      5 => 'usao/inn/press_release/2013/Mar_2013',
      6 => 'usao/inn/press_release/2013/Feb_2013',
      7 => 'usao/inn/press_release/2013/Apr_2013',
      8 => 'usao/inn/press_release/2013',
      9 => 'usao/inn/press_release/2013/Aug_13',
      10 => 'usao/inn/press_release/2013/Dec_13',
      11 => 'usao/inn/press_release/2013/Jul_13',
      12 => 'usao/inn/press_release/2013/May_13',
      13 => 'usao/inn/press_release/2013/Jun_13',
      14 => 'usao/inn/press_release/2013/Oct_13',
      15 => 'usao/inn/press_release/2013/Sept_13',
      16 => 'usao/inn/press_release/2013/Nov_13',
      18 => 'usao/inn/press_release/2014/Sept_14',
      19 => 'usao/inn/press_release/2014/Jul_14',
      20 => 'usao/inn/press_release/2014/Aug_14',
      21 => 'usao/inn/press_release/2014/Nov_14',
      22 => 'usao/inn/press_release/2014/Oct_14',
      23 => 'usao/inn/press_release/2014/June_14',
      24 => 'usao/inn/press_release/2014/Dec_14',
      25 => 'usao/inn/press_release/2015/Mar_15',
      26 => 'usao/inn/press_release/2015/Feb_15',
      27 => 'usao/inn/press_release/2015/Jan_15',
    );
    parent::__construct($arguments, $source_dirs);
  }
}

/**
 * SourceParser for /usao-ndin pages.
 *
 * @package doj_migration
 * @subpackage usao-ndin
 */
class UsaoNdinPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-ndin press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndin
 */
class UsaoNdinPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-ndin pages.
 *
 * @package doj_migration
 * @subpackage usao-ndin
 * @see Obtainer.api.php
 */
class UsaoNdinObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-ndin press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndin
 * @see Obtainer.api.php
 */
class UsaoNdinObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-ndin press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndin
 * @see Obtainer.api.php
 */
class UsaoNdinObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-ndin.
 *
 * @package doj_migration
 * @subpackage usao-ndin
 * @see Obtainer.api.php
 */
class UsaoNdinObtainDate extends ObtainDate {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $text = preg_replace('/-[1-9][0-9]|-[1-9]/', '', $text);
    $remove = array(
      'Mary Hatton',
    );
    $text = str_ireplace($remove, ' ', $text);

    $years = array('2012','2013','2014');
    foreach ($years as $year) {
      $pos = strpos($text, $year);
      if (!empty($pos)) {
        $text = substr($text, 0, ($pos + 4));
      }
    }

    return $text;
  }
}
