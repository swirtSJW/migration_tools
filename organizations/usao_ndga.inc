<?php

/**
 * @file
 * Defines migration classes for Northern District of Georgia.
 */

/**
 * Migrates .html files from /usao-ndga to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ndga
 */
class UsaoNdgaPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/gan',
      1 => 'usao/gan/press',
      5 => 'usao/gan/divisions',
      6 => 'usao/gan/community',
      7 => 'usao/gan/community/youthjusticesummit',
      8 => 'usao/gan/community/youthjusticesummit/videos',
      9 => 'usao/gan/community/humantrafficking',
      10 => 'usao/gan/community/humantrafficking/videos',
      11 => 'usao/gan/faq',
      12 => 'usao/gan/careers',
      13 => 'usao/gan/citizen',
      14 => 'usao/gan/vicwit',
      15 => 'usao/gan/vicwit/Carters',
      16 => 'usao/gan/vicwit/Enrico',
      17 => 'usao/gan/vicwit/ERT',
      18 => 'usao/gan/contact',
      19 => 'usao/gan/vw',
      20 => 'usao/gan/vw/cases/Carters',
      21 => 'usao/gan/vw/cases/Enrico',
      22 => 'usao/gan/vw/cases/SpyEye',
      23 => 'usao/gan/vw/cases/ERT',
      24 => 'usao/gan/usattorney',
      25 => 'usao/gan/about',
      26 => 'usao/gan/programs',
      27 => 'usao/gan/programs/internetsafety',
      28 => 'usao/gan/programs/grants',
    );

    $arguments['source_parser_class'] = "UsaoNdgaPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdgaObtainTitlePage");
    $title->addMethod('findSelector', array("h1", 1));
    $title->addMethod('findAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Northern District of Georgia.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ndga')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-ndga.
 *
 * @package doj_migration
 * @subpackage usao-ndga
 */
class UsaoNdgaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/gan subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/gan',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-ndga/docs';

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
    return "usao-ndga";
  }
}

/**
 * Migrates press releases from /usao-ndga.
 *
 * @package doj_migration
 * @subpackage usao-ndga
 */
class UsaoNdgaPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ndga";
    $arguments['component_tid'] = 1781;
    $arguments['source_parser_class'] = "UsaoNdgaPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdgaObtainTitlePressRelease");
    $title->addMethod('findAnySelectorUntilValid', array('h1'));
    $title->addMethod('findSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('findSelector', array("h2", 1));
    $title->addMethod('findSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('findSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('findSelector', array("#headline", 1));
    $title->addMethod('findSelector', array("p > strong > em", 1));
    $title->addMethod('findSelector', array("#contentstart > div > h2", 1));

    $date = new ObtainerInfo('date', "UsaoNdgaObtainDate");
    $date->addMethod('findTableContents', array(1, 1, 2));
    $date->addMethod('findTableContents', array(1, 1, 1));
    $date->addMethod('findTableContents', array(2, 2, 2));
    $date->addMethod('findSelector', array("p[align='center']", 1));
    $date->addMethod('findSelector', array("#contentstart > p", 1));
    $date->addMethod('findSelector', array(".newsRight", 1));
    $date->addMethod('findSelector', array(".newsRight", 1));
    $date->addMethod('findSelector', array(".BottomLeftContent", 1));
    $date->addMethod('findProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoNdgaObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    $source_dirs = array(
      2 => 'usao/gan/press/2015',
      3 => 'usao/gan/press/2014',
      4 => 'usao/gan/press/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}

/**
 * SourceParser for /usao-ndga pages.
 *
 * @package doj_migration
 * @subpackage usao-ndga
 */
class UsaoNdgaPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-ndga press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndga
 */
class UsaoNdgaPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-ndga pages.
 *
 * @package doj_migration
 * @subpackage usao-ndga
 * @see Obtainer.api.php
 */
class UsaoNdgaObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-ndga press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndga
 * @see Obtainer.api.php
 */
class UsaoNdgaObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-ndga press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndga
 * @see Obtainer.api.php
 */
class UsaoNdgaObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-ndga.
 *
 * @package doj_migration
 * @subpackage usao-ndga
 * @see Obtainer.api.php
 */
class UsaoNdgaObtainDate extends ObtainDate {
}
