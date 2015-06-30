<?php

/**
 * @file
 * Defines migration classes for Eastern District of New York.
 */

/**
 * Migrates .html files from /usao-edny to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-edny
 */
class UsaoEdnyPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/nye',
      3 => 'usao/nye/vw',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoEdnyPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoEdnyObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Eastern District of New York.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-edny')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/nye/index.html',
      '/usao/nye/index_old.html',
      '/usao/nye/vw/VW verification.htm',
      '/usao/nye/default.html',
      '/usao/nye/notice.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-edny.
 *
 * @package doj_migration
 * @subpackage usao-edny
 */
class UsaoEdnyFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/nye subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/nye',
    );
    $options = array('recurse' => TRUE);
    $regex = NULL;
    $dest_dir = 'public:///usao-edny/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "usao-edny";
  }
}

/**
 * Migrates press releases from /usao-edny.
 *
 * @package doj_migration
 * @subpackage usao-edny
 */
class UsaoEdnyPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-edny";
    $arguments['component_tid'] = 1971;

    $source_dirs = array(
      1 => 'usao/nye/pr',
      2 => 'usao/nye/pr/2013',
      15 => 'usao/nye/pr/January14',
      18 => 'usao/nye/pr/February14',
      19 => 'usao/nye/pr/March14',
      14 => 'usao/nye/pr/April14',
      17 => 'usao/nye/pr/May14',
      9 => 'usao/nye/pr/June14',
      20 => 'usao/nye/pr/July14',
      16 => 'usao/nye/pr/August14',
      8 => 'usao/nye/pr/September14',
      11 => 'usao/nye/pr/October14',
      5 => 'usao/nye/pr/November14',
      6 => 'usao/nye/pr/December14',
      13 => 'usao/nye/pr/January15',
      12 => 'usao/nye/pr/February15',
      4 => 'usao/nye/pr/March15',
      10 => 'usao/nye/pr/April15',
      7 => 'usao/nye/pr/May15',
      30 => 'usao/nye/pr/June15',
      31 => 'usao/nye/pr/July15',
      32 => 'usao/nye/pr/August15',
      33 => 'usao/nye/pr/September15',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(htm|html|php)$/i';

    $arguments['source_parser_class'] = "UsaoEdnyPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoEdnyObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));

    $subtitle = new ObtainerInfo('subtitle', "UsaoEdnyObtainTitlePressRelease");
    $subtitle->addMethod('pluckAnySelectorUntilValid', array('h3'));

    $date = new ObtainerInfo('date', "UsaoEdnyObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "UsaoEdnyObtainPrNumber");

    $body = new ObtainerInfo('body', 'UsaoEdnyPressreleaseObtainBody');
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $subtitle;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;
    $arguments['obtainers_info'][] = $body;

    parent::__construct($arguments, $source_dirs, $options, $extension_regex);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/nye/pr/pressrel_2011.html',
      '/usao/nye/pr/pressrel_2005.html',
      '/usao/nye/pr/pressrel_2007.html',
      '/usao/nye/pr/pressrel.html',
      '/usao/nye/pr/pressrel_2012.html',
      '/usao/nye/pr/pressrel_2010.html',
      '/usao/nye/pr/pressrel_2008.html',
      '/usao/nye/pr/pressrel_2013.html',
      '/usao/nye/pr/pressrel_2006.html',
      '/usao/nye/pr/pressrel_2009.html',
      '/usao/nye/pr/pressrel_2011.html',
      '/usao/nye/pr/index.php',
      '/usao/nye/pr/attachments.php',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-edny pages.
 *
 * @package doj_migration
 * @subpackage usao-edny
 */
class UsaoEdnyPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /usao-edny press releases.
 *
 * @package doj_migration
 * @subpackage usao-edny
 */
class UsaoEdnyPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();

    $this->queryPath->find('div.printer')->remove();
    $this->queryPath->find('.twitter-follow-button')->remove();
    $this->queryPath->find('.twitter-share-button')->remove();
    $this->queryPath->find('hr')->remove();
    $this->queryPath->find("br[clear = 'left']")->remove();
  }
}

/**
 * Obtainer for title property for /usao-edny pages.
 *
 * @package doj_migration
 * @subpackage usao-edny
 * @see Obtainer.api.php
 */
class UsaoEdnyObtainTitlePage extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'The United States Attorney\'s Office',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}

/**
 * Obtainer for title property for /usao-edny press releases.
 *
 * @package doj_migration
 * @subpackage usao-edny
 * @see Obtainer.api.php
 */
class UsaoEdnyObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-edny press releases.
 *
 * @package doj_migration
 * @subpackage usao-edny
 * @see Obtainer.api.php
 */
class UsaoEdnyObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-edny.
 *
 * @package doj_migration
 * @subpackage usao-edny
 * @see Obtainer.api.php
 */
class UsaoEdnyObtainDate extends ObtainDate {
}

/**
 * Obtainer for body property for /usao-edny press releases.
 *
 * @package doj_migration
 * @subpackage usao-edny
 * @see Obtainer.api.php
 */
class UsaoEdnyPressreleaseObtainBody extends ObtainBody {
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
      '<a href="https://twitter.com/SDNYNews" class="twitter-follow-button" data-show-count="false">Follow @EDNYNews</a>',
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
      '<a class="twitter-follow-button" data-show-count="false" href="https://twitter.com/EDNYNews">Follow @EDNYNews</a>',
      '<?php',
      '<br/>"; ?>',
      '"; ?>',
      '?>',
      "else\n",
      'class="printer"',
      ' ? ',
      'margin-bottom:19px;',
      'margin-right:12px;',
      'text-indent:48px',
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
