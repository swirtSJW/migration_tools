<?php

/**
 * @file
 * Defines migration classes for Northern District of Alabama.
 */

/**
 * Migrates .html files from /usao-ndal to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ndal
 */
class UsaoNdalPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      4 => 'usao/aln',
    );

    $arguments['source_parser_class'] = "UsaoNdalPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdalObtainTitlePage");
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
    $this->description = t('Migrates pages from the Northern District of Alabama.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ndal')
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
 * Migrates files (*.pdf) from /usao-ndal.
 *
 * @package doj_migration
 * @subpackage usao-ndal
 */
class UsaoNdalFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/aln subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/aln',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-ndal/docs';

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
    return "usao-ndal";
  }
}

/**
 * Migrates press releases from /usao-ndal.
 *
 * @package doj_migration
 * @subpackage usao-ndal
 */
class UsaoNdalPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ndal";
    $arguments['component_tid'] = 1696;
    $arguments['source_parser_class'] = "UsaoNdalPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoNdalObtainTitlePressRelease");
    $title->addMethod('findH1Any');
    $title->addMethod('findSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('findSelector', array("h2", 1));
    $title->addMethod('findSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('findSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('findSelector', array("#headline", 1));
    $title->addMethod('findSelector', array("p > strong > em", 1));
    $title->addMethod('findSelector', array("#contentstart > div > h2", 1));

    $date = new ObtainerInfo('date', "UsaoNdalObtainDate");
    $date->addMethod('findTableRow1Col2');
    $date->addMethod('findTableRow1Col1');
    $date->addMethod('findTable2Row2Col2');
    $date->addMethod('findSelector', array("p[align='center']", 1));
    $date->addMethod('findSelector', array("#contentstart > p", 1));
    $date->addMethod('findSelector', array(".newsRight > p", 1));
    $date->addMethod('findSelector', array(".BottomLeftContent", 1));
    $date->addMethod('findProbableDate');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;

    $source_dirs = array(
      0 => 'usao/aln/News/November 2013',
      1 => 'usao/aln/News/June 2013',
      2 => 'usao/aln/News/October 2014',
      3 => 'usao/aln/News/December 2013',
      5 => 'usao/aln/News/October 2013',
      6 => 'usao/aln/News/January 2015',
      7 => 'usao/aln/News/August 2014',
      8 => 'usao/aln/News/September 2014',
      9 => 'usao/aln/News/July 2013',
      10 => 'usao/aln/News/July 2014',
      11 => 'usao/aln/News/May 2014',
      12 => 'usao/aln/News/May 2013',
      13 => 'usao/aln/News/January 2014',
      14 => 'usao/aln/News/August 2013',
      15 => 'usao/aln/News/February 2014',
      16 => 'usao/aln/News/February 2015',
      17 => 'usao/aln/News/November 2014',
      18 => 'usao/aln/News/April 2014',
      19 => 'usao/aln/News/December 2014',
      20 => 'usao/aln/News/September 2013',
      21 => 'usao/aln/News/March 2013',
      22 => 'usao/aln/News/February 2013',
      23 => 'usao/aln/News/April 2013',
      24 => 'usao/aln/News/June 2014',
      25 => 'usao/aln/News/January 2013',
      26 => 'usao/aln/News/March 2014',
    );
    parent::__construct($arguments, $source_dirs);
  }
}

/**
 * SourceParser for /usao-ndal pages.
 *
 * @package doj_migration
 * @subpackage usao-ndal
 */
class UsaoNdalPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-ndal press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndal
 */
class UsaoNdalPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-ndal pages.
 *
 * @package doj_migration
 * @subpackage usao-ndal
 * @see Obtainer.api.php
 */
class UsaoNdalObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-ndal press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndal
 * @see Obtainer.api.php
 */
class UsaoNdalObtainTitlePressRelease extends ObtainTitlePressRelease {
}


/**
 * Custom obtainer for date property for /usao-ndal.
 *
 * @package doj_migration
 * @subpackage usao-ndal
 * @see Obtainer.api.php
 */
class UsaoNdalObtainDate extends ObtainDate {
}
