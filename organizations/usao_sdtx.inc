<?php

/**
 * @file
 * Defines migration classes for Southern District of Texas.
 */

/**
 * Migrates .html files from /usao-sdtx to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdtx
 */
class UsaoSdtxPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/txs/1News/Index pages',
      14 => 'usao/txs',
      15 => 'usao/txs/1Employment',
      16 => 'usao/txs/1Programs',
      17 => 'usao/txs/1About Us',
      18 => 'usao/txs/1Contact',
      19 => 'usao/txs/1Divisions',
      46 => 'usao/txs/1News',
    );

    $arguments['source_parser_class'] = "UsaoSdtxPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdtxObtainTitlePage");
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Southern District of Texas.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdtx')
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
 * Migrates files (*.pdf) from /usao-sdtx.
 *
 * @package doj_migration
 * @subpackage usao-sdtx
 */
class UsaoSdtxFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/txs subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/txs',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-sdtx/docs';

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
    return "usao-sdtx";
  }
}

/**
 * Migrates press releases from /usao-sdtx.
 *
 * @package doj_migration
 * @subpackage usao-sdtx
 */
class UsaoSdtxPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdtx";
    $arguments['component_tid'] = 2091;
    $arguments['source_parser_class'] = "UsaoSdtxPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdtxObtainTitlePress");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array("h2", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > div > p", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));
    $title->addMethod('pluckSelector', array("p > strong > em", 1));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 1));
    $title->addMethod("findCenteredParagraphStrong");

    $subtitle = new ObtainerInfo('subtitle', "UsaoSdtxObtainSubtitlePress");
    $subtitle->addMethod("findCenteredParagraphStrong");

    $date = new ObtainerInfo('date', "UsaoSdtxObtainDate");
    $date->addMethod('pluckTableContents', array(1, 1, 1));
    $date->addMethod('pluckTableContents', array(1, 1, 2));
    $date->addMethod('pluckTableContents', array(2, 2, 2));
    $date->addMethod('pluckSelector', array("p[align='center']", 1));
    $date->addMethod('pluckSelector', array("#contentstart > p", 1));
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckSelector', array(".BottomLeftContent", 1));
    $date->addMethod('pluckProbableDate');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $subtitle;
    $arguments['obtainers_info'][] = $date;

    $source_dirs = array(
      50 => 'usao/txs/1News/Releases/2015 March',
      51 => 'usao/txs/1News/Releases/2015 April',
      52 => 'usao/txs/1News/Releases/2015 May',
      53 => 'usao/txs/1News/Releases/2015 June',
      20 => 'usao/txs/1News/Releases/2015 February',
      21 => 'usao/txs/1News/Releases/2014 August',
      22 => 'usao/txs/1News/Releases/2013 May',
      23 => 'usao/txs/1News/Releases/2014 November',
      24 => 'usao/txs/1News/Releases/2014 June',
      25 => 'usao/txs/1News/Releases/2014 Ocotber',
      26 => 'usao/txs/1News/Releases/2013 March',
      27 => 'usao/txs/1News/Releases/2013 June',
      28 => 'usao/txs/1News/Releases/2013 January',
      29 => 'usao/txs/1News/Releases/2014 February',
      30 => 'usao/txs/1News/Releases/2013 August',
      31 => 'usao/txs/1News/Releases/2014 December',
      32 => 'usao/txs/1News/Releases/2015 January',
      33 => 'usao/txs/1News/Releases/2013 September',
      34 => 'usao/txs/1News/Releases/2013 February',
      35 => 'usao/txs/1News/Releases/2014 September',
      36 => 'usao/txs/1News/Releases/2014 March',
      37 => 'usao/txs/1News/Releases/2013 July',
      38 => 'usao/txs/1News/Releases/2013 October',
      39 => 'usao/txs/1News/Releases/2014 July',
      40 => 'usao/txs/1News/Releases/2013 November',
      41 => 'usao/txs/1News/Releases/2014 January',
      42 => 'usao/txs/1News/Releases/2014 May',
      43 => 'usao/txs/1News/Releases/2014 April',
      44 => 'usao/txs/1News/Releases/2013 April',
      45 => 'usao/txs/1News/Releases/2013 December',
    );
    parent::__construct($arguments, $source_dirs);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip = array(
      "/usao/txs/1News/Releases/2013 June/130614 - Bjork.html",
    );
    if (substr_count($row->fileid, "_print") > 0 || doj_migration_skip_file($row->fileid, $skip)) {
      return FALSE;
    }

    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }

}

class UsaoSdtxPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

class UsaoSdtxPressSourceParser extends NGDistrictPressReleaseSourceParser {
  protected $subtitle;
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    // Get the date before HTML is cleaned, as it will be removed.
    $date = $this->getDate();

    foreach ($this->queryPath->find("table") as $table) {
      $text = $table->text();
      if (substr_count($text, "Print")) {
        $table->remove();
      }
    }
    parent::cleanHtml();
  }

  /**
   * Getter.
   */
  public function getSubtitle() {
    $subtitle = $this->getProperty("subtitle");
    return $subtitle;
  }

  /**
   * {@inheritdoc}
   */
  public function getBody() {
    // The processing of this "empty" bodies takes and insane amount of time
    // We will ignore them as they are identified.
    $body_text = $this->queryPath->find("body")->text();
    $body_text = preg_replace('/\s+/', '', $body_text);
    if (strlen($body_text) < 300) {
      return "";
    }

    return parent::getBody();
  }
}

class UsaoSdtxObtainTitlePage extends ObtainTitle {
}

class UsaoSdtxObtainTitlePress extends ObtainTitlePressRelease {
  /**
   * Get the first strong inside of a centered paragraph.
   */
  protected function findCenteredParagraphStrong() {
    $subtitle = $this->queryPath->find("p[align='center']>strong")->first();
    $this->setElementToRemove($subtitle);
    return $subtitle->text();
  }

  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    // Remove strong tags before parent cleanString method runs.
    $remove = array(
      'A criminal complaint is a formal accusation of  criminal conduct, not evidence. ',
      'A  complaint or indictment is a formal accusation of criminal conduct, not  evidence.',
      'A defendant is presumed innocent unless convicted through due process of law.',
      'An indictment  is a formal accusation of criminal conduct, not evidence.',
    );
    $text = str_ireplace($remove, '', $text);
    $text = parent::cleanString($text);

    return $text;
  }
}

class UsaoSdtxObtainSubtitlePress extends UsaoSdtxObtainTitlePress {

}

class UsaoSdtxObtainDate extends ObtainDate {

}
