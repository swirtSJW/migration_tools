<?php

/**
 * @file
 * Defines migration classes for Western District of Virginia.
 */

/**
 * Migrates .html files from /usao-wdva to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-wdva
 */
class UsaoWdvaPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/vaw/media/press_events/Goonz',
      1 => 'usao/vaw/media/press_events/Goonz/pages',
      2 => 'usao/vaw/media/press_events/Kuban',
      3 => 'usao/vaw/media/press_events/Kuban/pages',
      4 => 'usao/vaw/media/press_events/Whittlesey',
      5 => 'usao/vaw/media/press_events/Whittlesey/pages',
      6 => 'usao/vaw/media/press_events/AntiViolenceForum',
      7 => 'usao/vaw/media/press_events/AntiViolenceForum/pages',
      8 => 'usao/vaw/media/press_events/NCVRW',
      9 => 'usao/vaw/media/press_events/NCVRW/pages',
      10 => 'usao/vaw/media/press_events/Heroin',
      11 => 'usao/vaw/media/press_events/Heroin/pages',
      12 => 'usao/vaw/media/press_events/Barnes',
      13 => 'usao/vaw/media/press_events/Barnes/pages',
      14 => 'usao/vaw/media/press_events/Coleman',
      15 => 'usao/vaw/media/press_events/Coleman/pages',
      16 => 'usao/vaw/media/press_events/Keller',
      17 => 'usao/vaw/media/press_events/Keller/pages',
      18 => 'usao/vaw/media/press_events/RalphJackson',
      19 => 'usao/vaw/media/press_events/RalphJackson/pages',
      20 => 'usao/vaw/media/press_events/Jones_etal',
      21 => 'usao/vaw/media/press_events/Jones_etal/pages',
      22 => 'usao/vaw/media/press_events/Jess',
      23 => 'usao/vaw/media/press_events/Jess/pages',
      24 => 'usao/vaw/media/press_events/Heaphy_SenateTestimony',
      25 => 'usao/vaw/media/press_events/Heaphy_SenateTestimony/pages',
      26 => 'usao/vaw/media/press_events/Cheek',
      27 => 'usao/vaw/media/press_events/Cheek/pages',
      28 => 'usao/vaw/media/press_events/Mechlenburg',
      29 => 'usao/vaw/media/press_events/Mechlenburg/pages',
      30 => 'usao/vaw/media/press_events/Morris',
      31 => 'usao/vaw/media/press_events/Morris/pages',
      32 => 'usao/vaw/media/press_events/2011FinancialRec',
      33 => 'usao/vaw/media/press_events/2011FinancialRec/pages',
      34 => 'usao/vaw/media/press_events/AntwanJackson',
      35 => 'usao/vaw/media/press_events/AntwanJackson/pages',
      36 => 'usao/vaw/media/press_events/Harding',
      37 => 'usao/vaw/media/press_events/Harding/pages',
      38 => 'usao/vaw/media/press_events/Richards',
      39 => 'usao/vaw/media/press_events/Richards/pages',
      40 => 'usao/vaw/media/press_events/Cisse',
      41 => 'usao/vaw/media/press_events/Cisse/pages',
      42 => 'usao/vaw/media/press_events/Mathis',
      43 => 'usao/vaw/media/press_events/Jones',
      44 => 'usao/vaw/media/press_events/Jones/pages',
      45 => 'usao/vaw/media/press_events/PCPCulpepper',
      46 => 'usao/vaw/media/press_events/PCPCulpepper/pages',
      47 => 'usao/vaw/media/press_events/AugustaCounty',
      48 => 'usao/vaw/media/press_events/AugustaCounty/pages',
      49 => 'usao/vaw/outreach/venable',
      50 => 'usao/vaw/outreach/venable/pages',
      51 => 'usao/vaw/presc_drug_summit/2012eventphotos',
      52 => 'usao/vaw/presc_drug_summit/2012eventphotos/pages',
      53 => 'usao/vaw',
      54 => 'usao/vaw/outreach',
      55 => 'usao/vaw/presc_drug_summit',
    );

    $arguments['source_parser_class'] = "UsaoWdvaPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWdvaObtainTitlePage");
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
    $this->description = t('Migrates pages from the Western District of Virginia.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-wdva')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (substr_count($row->fileid, "_jpg.htm")) {
      return FALSE;
    }

    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-wdva.
 *
 * @package doj_migration
 * @subpackage usao-wdva
 */
class UsaoWdvaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/vaw subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/vaw',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-wdva/docs';

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
    return "usao-wdva";
  }
}

/**
 * Migrates press releases from /usao-wdva.
 *
 * @package doj_migration
 * @subpackage usao-wdva
 */
class UsaoWdvaPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-wdva";
    $arguments['component_tid'] = 2111;
    $arguments['source_parser_class'] = "UsaoWdvaPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoWdvaObtainTitlePressRelease");
    $title->addMethod('findH1Any');
    $title->addMethod('findSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('findSelector', array("h2", 1));
    $title->addMethod('findSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('findSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('findSelector', array("#headline", 1));
    $title->addMethod('findSelector', array("p > strong > em", 1));
    $title->addMethod('findSelector', array("#contentstart > div > h2", 1));

    // Configuring Obtainers.
    $subtitle = new ObtainerInfo('subTitle', "UsaoWdvaObtainSubtitlePressRelease");
    $subtitle->addMethod('findSelector', array("h2", 1));

    $date = new ObtainerInfo('date', "UsaoWdvaObtainDate");
    $date->addMethod('findTableRow1Col2');
    $date->addMethod('findTableRow1Col1');
    $date->addMethod('findTable2Row2Col2');
    $date->addMethod('findSelector', array("p[align='center']", 1));
    $date->addMethod('findSelector', array("#contentstart > p", 1));
    $date->addMethod('findSelector', array(".newsRight", 1));
    $date->addMethod('findSelector', array(".BottomLeftContent", 1));
    $date->addMethod('findProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoWdvaObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $subtitle;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    $source_dirs = array(
      56 => 'usao/vaw/news',
    );
    parent::__construct($arguments, $source_dirs);
  }
}

/**
 * SourceParser for /usao-wdva pages.
 *
 * @package doj_migration
 * @subpackage usao-wdva
 */
class UsaoWdvaPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-wdva press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdva
 */
class UsaoWdvaPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }

  /**
   * Getter.
   */
  public function getSubTitle() {
    return $this->getProperty("subTitle");
  }
}

/**
 * Obtainer for title property for /usao-wdva pages.
 *
 * @package doj_migration
 * @subpackage usao-wdva
 * @see Obtainer.api.php
 */
class UsaoWdvaObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /usao-wdva press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdva
 * @see Obtainer.api.php
 */
class UsaoWdvaObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for title property for /usao-wdva press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdva
 * @see Obtainer.api.php
 */
class UsaoWdvaObtainSubtitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-wdva press releases.
 *
 * @package doj_migration
 * @subpackage usao-wdva
 * @see Obtainer.api.php
 */
class UsaoWdvaObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-wdva.
 *
 * @package doj_migration
 * @subpackage usao-wdva
 * @see Obtainer.api.php
 */
class UsaoWdvaObtainDate extends ObtainDate {
}
