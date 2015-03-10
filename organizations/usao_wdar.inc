<?php

/**
 * @file
 * Defines migration classes for Western District of Arkansas.
 */

/**
 * Migrates .html files from /usao-wdar to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdar
 */
class UsaoWdarPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/arw',
    );

    $arguments['source_parser_class'] = "UsaoWdarPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWdarObtainTitlePage");
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
    $this->description = t('Migrates pages from the Western District of Arkansas.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wdar')
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
 * Migrates files (*.pdf) from /usao-wdar.
 *
 * @package doj_migration
 * @subpackage usao-wdar
 */
class UsaoWdarFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/arw subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/arw',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-wdar/docs';

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
    return "usao-wdar";
  }
}

/**
 * Migrates press releases from /usao-wdar.
 *
 * @package doj_migration
 * @subpackage usao-wdar
 */
class UsaoWdarPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdar";
    $arguments['component_tid'] = 1711;
    $arguments['source_parser_class'] = "UsaoWdarPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWdarObtainTitlePressRelease");
    $title->addMethod('findH1Any');
    $title->addMethod('findIdContentstartDivH2Sec');
    $title->addMethod('findH2First');
    $title->addMethod('findClassContentSubDivPCenterStrong');
    $title->addMethod('findClassContentSubDivDivPStrong');
    $title->addMethod('findIdHeadline');
    $title->addMethod('findPStrongEm');
    $title->addMethod('findIdContentstartDivH2');
    $title->addMethod('findDivClassContentSubDivDivCenter');

    $date = new ObtainerInfo('date', "UsaoWdarObtainDate");
    $date->addMethod('findTableRow1Col2');
    $date->addMethod('findTableRow1Col1');
    $date->addMethod('findTable2Row2Col2');
    $date->addMethod('findPAlignCenter');
    $date->addMethod('findIdContentstartFirst');
    $date->addMethod('findClassNewsRight');
    $date->addMethod('findClassBottomLeftContent');
    $date->addMethod('findProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoWdarObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    $source_dirs = array(
      1 => 'usao/arw/pressreleases',
    );
    parent::__construct($arguments, $source_dirs);
  }
}

/**
 * SourceParser for /usao-wdar pages.
 *
 * @package doj_migration
 * @subpackage usao-wdar
 */
class UsaoWdarPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-wdar press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdar
 */
class UsaoWdarPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    // Any tables in these PRs are junk.
    foreach ($this->queryPath->find(".contentSub table") as $table) {
      $table->remove();
    }
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-wdar pages.
 *
 * @package doj_migration
 * @subpackage usao-wdar
 * @see Obtainer.api.php
 */
class UsaoWdarObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-wdar press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdar
 * @see Obtainer.api.php
 */
class UsaoWdarObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-wdar press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdar
 * @see Obtainer.api.php
 */
class UsaoWdarObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-wdar.
 *
 * @package doj_migration
 * @subpackage usao-wdar
 * @see Obtainer.api.php
 */
class UsaoWdarObtainDate extends ObtainDate {
}
