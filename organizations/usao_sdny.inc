<?php

/**
 * @file
 * Defines migration classes for Southern District of New York.
 */

/**
 * Migrates .html files from /usao-sdny to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 */
class UsaoSdnyPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      1 => 'usao/nys/images/old',
      2 => 'usao/nys',
      3 => 'usao/nys/vw_cases',
      4 => 'usao/nys/pressconference',
      5 => 'usao/nys/officehistory',
      // 6 => 'usao/nys/gallery/aapi',
      // 7 => 'usao/nys/gallery/hardvardclassday2014',
      // 8 => 'usao/nys/gallery/bobfiskevisit',
      // 9 => 'usao/nys/gallery/cardozocommencement2014',
      // 10 => 'usao/nys/gallery/morelandcommissiontestimony',
      // 11 => 'usao/nys/gallery/daviddinkinsvisit',
      // 12 => 'usao/nys/gallery/avenueamericas',
      // 13 => 'usao/nys/gallery/rcnls',
      // 14 => 'usao/nys/gallery/comey',
      // 15 => 'usao/nys/gallery/jehjohnsonvisit',
      // 16 => 'usao/nys/gallery/lisamonacovisit',
      // 17 => 'usao/nys/gallery/cybersecurityconference',
      // 18 => 'usao/nys/gallery/jdac2014',
      // 19 => 'usao/nys/gallery/crainnybreakfast',
      // 20 => 'usao/nys/gallery/ericholdervisit',
      // 54 => 'usao/nys/videos',
    );

    $arguments['source_parser_class'] = "UsaoSdnyPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdnyObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array('h2'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');
    $title->addMethod('pluckSelector', array('h3 > strong'));
    $title->addMethod('pluckSelector', array('h3'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    // This one is risky because it will almost always find a title.
    $title->addMethod('pluckSelector', array('title'));

    $arguments['obtainers_info'][] = $title;

    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(htm|html|php)$/i';

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Southern District of New York.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdny')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/nys/test2.php',
      '/usao/nys/test.php',
      '/usao/nys/emailform.php',
      '/usao/nys/ausasubmit2.php',
      '/usao/nys/images/old/skiing.html',
      '/usao/nys/internemail.php',
      '/usao/nys/test.html',
      '/usao/nys/internsubmit.php',
    );

    if (doj_migration_skip_file($row->fileid, $skip_these) || (parent::prepareRow($row) === FALSE)) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-sdny.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 */
class UsaoSdnyFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/nys subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/nys',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///usao-sdny/docs';

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
    return "usao-sdny";
  }
}

/**
 * Migrates press releases from /usao-sdny.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 */
class UsaoSdnyPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdny";
    $arguments['component_tid'] = 1981;
    $arguments['source_parser_class'] = "UsaoSdnyPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdnyObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $subtitle = new ObtainerInfo('subtitle');
    $subtitle->addMethod('pluckSelector', array("h3[align='center'] > em", 1));

    $date = new ObtainerInfo('date', "UsaoSdnyObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoSdnyObtainPrNumber");
    $pr_number->addMethod('pluckAnySelectorUntilValidDrillUp', array('p', 20));

    $body = new ObtainerInfo('body', 'UsaoSdnyPressreleaseObtainBody');
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $subtitle;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;
    $arguments['obtainers_info'][] = $body;

    $source_dirs = array(
      0 => 'usao/nys/pressreleases/April14',
      27 => 'usao/nys/pressreleases/January13',
      28 => 'usao/nys/pressreleases/July13',
      29 => 'usao/nys/pressreleases/March15',
      30 => 'usao/nys/pressreleases/November14',
      31 => 'usao/nys/pressreleases/August13',
      32 => 'usao/nys/pressreleases/September13',
      33 => 'usao/nys/pressreleases/November13',
      34 => 'usao/nys/pressreleases/December14',
      35 => 'usao/nys/pressreleases/September14',
      36 => 'usao/nys/pressreleases/December13',
      37 => 'usao/nys/pressreleases/June14',
      38 => 'usao/nys/pressreleases/October14',
      39 => 'usao/nys/pressreleases/February13',
      40 => 'usao/nys/pressreleases/February15',
      41 => 'usao/nys/pressreleases/March13',
      42 => 'usao/nys/pressreleases/June13',
      43 => 'usao/nys/pressreleases/April13',
      44 => 'usao/nys/pressreleases/January15',
      45 => 'usao/nys/pressreleases/January14',
      46 => 'usao/nys/pressreleases/August14',
      47 => 'usao/nys/pressreleases/October13',
      48 => 'usao/nys/pressreleases/May14',
      49 => 'usao/nys/pressreleases/May14/AbuHamza',
      50 => 'usao/nys/pressreleases/February14',
      51 => 'usao/nys/pressreleases/May13',
      52 => 'usao/nys/pressreleases/March14',
      53 => 'usao/nys/pressreleases/July14',
      60 => 'usao/nys/pressreleases/April15',
      61 => 'usao/nys/pressreleases/May15',
      62 => 'usao/nys/pressreleases/June15',
      63 => 'usao/nys/pressreleases/July15',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(htm|html|php)$/i';
    parent::__construct($arguments, $source_dirs, $options, $extension_regex);
  }


  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/nys/pressreleases/May14/AbuHamza/abuhamza.php',
      '/usao/nys/pressreleases/May14/AbuHamza.php',

    );

    if (doj_migration_skip_file($row->fileid, $skip_these) || (parent::prepareRow($row) === FALSE)) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-sdny pages.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 */
class UsaoSdnyPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-sdny press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 */
class UsaoSdnyPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();

    $this->queryPath->find('div.printer')->remove();
    $this->queryPath->find('twitter-follow-button')->remove();
    $this->queryPath->find('twitter-share-button')->remove();
  }
}

/**
 * Obtainer for title property for /usao-sdny pages.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 * @see Obtainer.api.php
 */
class UsaoSdnyObtainTitlePage extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'SDNY',
    );
    $text = str_replace($remove, '', $text);

    return $text;
  }
}

/**
 * Obtainer for title property for /usao-sdny press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 * @see Obtainer.api.php
 */
class UsaoSdnyObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-sdny press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 * @see Obtainer.api.php
 */
class UsaoSdnyObtainPrNumber extends ObtainId {

}

/**
 * Custom obtainer for date property for /usao-sdny.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 * @see Obtainer.api.php
 */
class UsaoSdnyObtainDate extends ObtainDate {
}


/**
 * Obtainer for body property for /usao-sdny press releases.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 * @see Obtainer.api.php
 */
class UsaoSdnyPressreleaseObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      '<?php if(!$print) echo "',
      '<?php include("../attachments.php"); ?>',
      '$print = $_GET["print"];',
      'if(!$print)',
      'echo "<link rel=\'stylesheet\' href=\'../../includes/usao-style.css\' type=\'text/css\' media=\'all\' />";',
      'include("../../includes/usao-header.ssi")',
      '<a href="https://twitter.com/SDNYNews" class="twitter-follow-button" data-show-count="false">Follow @SDNYNews</a>',
      'include("../attachments.php"); ?',
      'style="float: left;"',
      'echo "<link rel=\'stylesheet\' href=\'../../css/usao_print.css\' type=\'text/css\' media=\'all\' />";',
      '<link rel="stylesheet" href="../../css/usao_print.css" type="text/css" media="all" />',
      'style="float: right;',
      '<p>"; else echo "</p>',
      '"; else     echo "',
      '<p>"; ?&gt;</p>',
      'include("../../includes/usao-footer.ssi");',
      'include("../../includes/usao-rightCol.ssi");',
      '<a class="twitter-follow-button" data-show-count="false" href="https://twitter.com/SDNYNews">Follow @SDNYNews</a>',
      '<?php',
      '<br/>"; ?>',
      '"; ?>',
      '?>',
      "else\n",
      'class="printer"',
      ' ? ',
    );

    $text = str_ireplace($remove, '', $text);
    // Fix the tweet link that gets mangled by the sourceparser as it goes from
    // php to html.
    $text = str_ireplace("'Tweet<br />", "'>Tweet</a><br/>", $text);

    // Now that it is fixed, remove the twitter link.
    $regex = "/<a\shref=\'https:\/\/twitter\.com([^\"]*)\'>([^\"]*)<\/a>/i";
    $text = preg_replace($regex, '', $text);

    return $text;
  }
}

/**
 * Migrates speeeches from /usao-sdny.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 */
class UsaoSdnySpeechMigration extends NGDistrictSpeechMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdny";
    $arguments['component_tid'] = 1981;
    $arguments['source_parser_class'] = "UsaoSdnySpeechSourceParser";

    // Define source directories.
    $source_dirs = array(
      // 1 => 'usao/nys/pressspeeches/old',
      2 => 'usao/nys/pressspeeches/2013',
      3 => 'usao/nys/pressspeeches/2011',
      4 => 'usao/nys/pressspeeches/2012',
      5 => 'usao/nys/pressspeeches/2010',
      6 => 'usao/nys/pressspeeches/2014',
      7 => 'usao/nys/pressspeeches/2015',
    );

    $regex = '/.*\.(htm|html|php)$/i';
    $options = array('recurse' => FALSE);

    // Configuring Obtainers.
    $body = new ObtainerInfo('body', 'UsaoSdnySpeechObtainBody');
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $body;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/nys/pressspeeches/old/index.php',
      '/usao/nys/pressspeeches/old/index_old.html',
      '/usao/nys/pressspeeches/old/sentencingguidelines.html',

    );

    if (doj_migration_skip_file($row->fileid, $skip_these) || (parent::prepareRow($row) === FALSE)) {
      return FALSE;
    }
  }
}

class UsaoSdnySpeechSourceParser extends NGSpeechSourceParser {
  /**
   * Clean the html right before pulling the body.
   */
  protected function cleanHtml() {
    parent::cleanHtml();

    // Selectors to remove.
    $selectors = array(
      '.twitter-follow-button',
      '.printer',
      '#headSearch',
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);
  }

}

/**
 * Obtainer for body property for /usao-sdny speech.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 * @see Obtainer.api.php
 */
class UsaoSdnySpeechObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      '<?php if(!$print) echo "',
      '<?php include("../attachments.php"); ?>',
      '$print = $_GET["print"];',
      'if(!$print)',
      'echo "<link rel=\'stylesheet\' href=\'../../includes/usao-style.css\' type=\'text/css\' media=\'all\' />";',
      'include("../../includes/usao-header.ssi")',
      '<a href="https://twitter.com/SDNYNews" class="twitter-follow-button" data-show-count="false">Follow @SDNYNews</a>',
      '<a class="twitter-follow-button" data-show-count="false" href="https://twitter.com/SDNYNews">Follow @SDNYNews</a>',
      'include("../attachments.php"); ?',
      'style="float: left;"',
      'echo "<link rel=\'stylesheet\' href=\'../../css/usao_print.css\' type=\'text/css\' media=\'all\' />";',
      '<link rel="stylesheet" href="../../css/usao_print.css" type="text/css" media="all" />',
      '<p>"; else echo "</p>',
      '"; else     echo "',
      '<p>"; ?&gt;</p>',
      'include("../../includes/usao-footer.ssi");',
      'include("../../includes/usao-rightCol.ssi");',
      '<?php',
      '<br/>"; ?>',
      '"; ?>',
      '?>',
      "else\n",
      'class="printer"',
      ' ? ',
    );

    $text = str_ireplace($remove, '', $text);
    // Fix the tweet link that gets mangled by the sourceparser as it goes from
    // php to html.
    $text = str_ireplace("'Tweet<br />", "'>Tweet</a><br/>", $text);
    return $text;
  }
}
