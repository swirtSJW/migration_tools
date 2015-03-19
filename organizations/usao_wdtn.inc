<?php

/**
 * @file
 * Defines migration classes for Western District of Tennessee.
 */

/**
 * Migrates .html files from /usao-wdtn to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdtn
 */
class UsaoWdtnPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      1 => 'usao/tnw/divisions',
      2 => 'usao/tnw/about',
      3 => 'usao/tnw',
      4 => 'usao/tnw/brochures',
      5 => 'usao/tnw/disclaimers',
      6 => 'usao/tnw/programs',
      7 => 'usao/tnw/news/newsletter',
    );

    $arguments['source_parser_class'] = "UsaoWdtnPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWdtnObtainTitlePage");
    $title->addMethod('findSelector', array("h1", 1));
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
    $this->description = t('Migrates pages from the Western District of Tennessee.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wdtn')
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
 * Migrates files (*.pdf) from /usao-wdtn.
 *
 * @package doj_migration
 * @subpackage usao-wdtn
 */
class UsaoWdtnFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/tnw subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/tnw',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-wdtn/docs';

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
    return "usao-wdtn";
  }
}

/**
 * Migrates press releases from /usao-wdtn.
 *
 * @package doj_migration
 * @subpackage usao-wdtn
 */
class UsaoWdtnPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdtn";
    $arguments['component_tid'] = 2071;
    $arguments['source_parser_class'] = "UsaoWdtnPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWdtnObtainTitlePressRelease");
    $title->addMethod('findH1Any');
    $title->addMethod('findSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('findSelector', array("h2", 1));
    $title->addMethod('findSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('findSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('findSelector', array("#headline", 1));
    $title->addMethod('findSelector', array("p > strong > em", 1));
    $title->addMethod('findSelector', array("#contentstart > div > h2", 1));

    $date = new ObtainerInfo('date', "UsaoWdtnObtainDate");
    $date->addMethod('findTableRow1Col2');
    $date->addMethod('findTableRow1Col1');
    $date->addMethod('findTable2Row2Col2');
    $date->addMethod('findSelector', array("p[align='center']", 1));
    $date->addMethod('findSelector', array("#contentstart > p", 1));
    $date->addMethod('findSelector', array(".newsRight", 1));
    $date->addMethod('findSelector', array(".BottomLeftContent", 1));
    $date->addMethod('findProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoWdtnObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    $source_dirs = array(
      0 => 'usao/tnw/news/2014',
      7 => 'usao/tnw/news/2013',
      11 => 'usao/tnw/news/2015',
    );
    parent::__construct($arguments, $source_dirs);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      'usao/tnw/news/2014/news.html',
      'usao/tnw/news/2013/news.html',
      'usao/tnw/news/2015/news.html',
    );

    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->legacy_path, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-wdtn pages.
 *
 * @package doj_migration
 * @subpackage usao-wdtn
 */
class UsaoWdtnPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-wdtn press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdtn
 */
class UsaoWdtnPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-wdtn pages.
 *
 * @package doj_migration
 * @subpackage usao-wdtn
 * @see Obtainer.api.php
 */
class UsaoWdtnObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-wdtn press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdtn
 * @see Obtainer.api.php
 */
class UsaoWdtnObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-wdtn press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdtn
 * @see Obtainer.api.php
 */
class UsaoWdtnObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-wdtn.
 *
 * @package doj_migration
 * @subpackage usao-wdtn
 * @see Obtainer.api.php
 */
class UsaoWdtnObtainDate extends ObtainDate {
}
