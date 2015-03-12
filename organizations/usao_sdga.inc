<?php

/**
 * @file
 * Defines migration classes for Southern District of Georgia.
 */

/**
 * Migrates .html files from /usao-sdga to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdga
 */
class UsaoSdgaPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/gas',
      1 => 'usao/gas/press_releases',
    );

    $arguments['source_parser_class'] = "UsaoSdgaPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdgaObtainTitlePage");
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
    $this->description = t('Migrates pages from the Southern District of Georgia.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdga')
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
 * Migrates files (*.pdf) from /usao-sdga.
 *
 * @package doj_migration
 * @subpackage usao-sdga
 */
class UsaoSdgaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/gas subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/gas',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-sdga/docs';

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
    return "usao-sdga";
  }
}

/**
 * Migrates press releases from /usao-sdga.
 *
 * @package doj_migration
 * @subpackage usao-sdga
 */
class UsaoSdgaPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdga";
    $arguments['component_tid'] = 1786;
    $arguments['source_parser_class'] = "UsaoSdgaPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdgaObtainTitlePressRelease");
    $title->addMethod('findH1Any');
    $title->addMethod('findSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('findSelector', array("h2", 1));
    $title->addMethod('findSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('findSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('findSelector', array("#headline", 1));
    $title->addMethod('findSelector', array("p > strong > em", 1));
    $title->addMethod('findSelector', array("#contentstart > div > h2", 1));

    $date = new ObtainerInfo('date', "UsaoSdgaObtainDate");
    $date->addMethod('findTableRow1Col2');
    $date->addMethod('findTableRow1Col1');
    $date->addMethod('findTable2Row2Col2');
    $date->addMethod('findSelector', array("p[align='center']", 1));
    $date->addMethod('findSelector', array("#contentstart > p", 1));
    $date->addMethod('findSelector', array(".newsRight > p", 1));
    $date->addMethod('findSelector', array(".BottomLeftContent", 1));
    $date->addMethod('findProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoSdgaObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    $source_dirs = array(
      2 => 'usao/gas/press_releases/2015',
      3 => 'usao/gas/press_releases/2014',
      4 => 'usao/gas/press_releases/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}

/**
 * SourceParser for /usao-sdga pages.
 *
 * @package doj_migration
 * @subpackage usao-sdga
 */
class UsaoSdgaPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-sdga press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdga
 */
class UsaoSdgaPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-sdga pages.
 *
 * @package doj_migration
 * @subpackage usao-sdga
 * @see Obtainer.api.php
 */
class UsaoSdgaObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-sdga press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdga
 * @see Obtainer.api.php
 */
class UsaoSdgaObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-sdga press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdga
 * @see Obtainer.api.php
 */
class UsaoSdgaObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-sdga.
 *
 * @package doj_migration
 * @subpackage usao-sdga
 * @see Obtainer.api.php
 */
class UsaoSdgaObtainDate extends ObtainDate {
}
