<?php

/**
 * @file
 * Defines migration classes for District of New Hampshire.
 */

/**
 * Migrates .html files from /usao-nh to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-nh
 */
class UsaoNhPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/nh/hr',
      1 => 'usao/nh',
      5 => 'usao/nh/aboutus/divisions',
      6 => 'usao/nh/aboutus',
      7 => 'usao/nh/french',
      8 => 'usao/nh/spanish',
      9 => 'usao/nh/programs',
    );

    $arguments['source_parser_class'] = "UsaoNhPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNhObtainTitlePage");
    $title->addMethod('findH1First');
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
    $this->description = t('Migrates pages from the District of New Hampshire.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-nh')
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
 * Migrates files (*.pdf) from /usao-nh.
 *
 * @package doj_migration
 * @subpackage usao-nh
 */
class UsaoNhFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/nh subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/nh',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-nh/docs';

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
    return "usao-nh";
  }
}

/**
 * Migrates press releases from /usao-nh.
 *
 * @package doj_migration
 * @subpackage usao-nh
 */
class UsaoNhPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-nh";
    $arguments['component_tid'] = 1951;
    $arguments['source_parser_class'] = "UsaoNhPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNhObtainTitlePressRelease");
    $title->addMethod('findH1Any');
    $title->addMethod('findIdContentstartDivH2Sec');
    $title->addMethod('findH2First');
    $title->addMethod('findClassContentSubDivPCenterStrong');
    $title->addMethod('findClassContentSubDivDivPStrong');
    $title->addMethod('findIdHeadline');
    $title->addMethod('findPStrongEm');
    $title->addMethod('findIdContentstartDivH2');
    $title->addMethod('findDivClassContentSubDivDivCenter');

    $date = new ObtainerInfo('date', "UsaoNhObtainDate");
    $date->addMethod('findTableRow1Col2');
    $date->addMethod('findTableRow1Col1');
    $date->addMethod('findTable2Row2Col2');
    $date->addMethod('findPAlignCenter');
    $date->addMethod('findIdContentstartFirst');
    $date->addMethod('findClassNewsRight');
    $date->addMethod('findClassBottomLeftContent');
    $date->addMethod('findProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoNhObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    $source_dirs = array(
      2 => 'usao/nh/press/2015',
      3 => 'usao/nh/press/2014',
      4 => 'usao/nh/press/2013',
    );
    parent::__construct($arguments, $source_dirs);
  }
}

/**
 * SourceParser for /usao-nh pages.
 *
 * @package doj_migration
 * @subpackage usao-nh
 */
class UsaoNhPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-nh press releases.
 *
 * @package doj_migration
 * @subpackage usao-nh
 */
class UsaoNhPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    // Remove empty paragraph tags.
    parent::cleanHtml();
    foreach ($this->queryPath->find(".contentSub > div > p") as $p) {
      $text = $p->text();
      $text = StringCleanUp::superTrim($text);
      if (empty($text)) {
        $p->remove();
      }
      // I think removing the fist is enough.
      break;
    }
  }
}

/**
 * Obtainer for title property for /usao-nh pages.
 *
 * @package doj_migration
 * @subpackage usao-nh
 * @see Obtainer.api.php
 */
class UsaoNhObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-nh press releases.
 *
 * @package doj_migration
 * @subpackage usao-nh
 * @see Obtainer.api.php
 */
class UsaoNhObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-nh press releases.
 *
 * @package doj_migration
 * @subpackage usao-nh
 * @see Obtainer.api.php
 */
class UsaoNhObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-nh.
 *
 * @package doj_migration
 * @subpackage usao-nh
 * @see Obtainer.api.php
 */
class UsaoNhObtainDate extends ObtainDate {
}
