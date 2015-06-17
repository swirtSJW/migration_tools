<?php

/**
 * @file
 * Defines migration classes for District of Rhode Island.
 */

/**
 * Migrates .html files from /usao-ri to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-ri
 */
class UsaoRiPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ri/about',
      1 => 'usao/ri',
      2 => 'usao/ri/projects',
      3 => 'usao/ri/bios',
      4 => 'usao/ri/federal_notes',
    );

    $arguments['source_parser_class'] = "UsaoRiPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoRiObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the District of Rhode Island.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-ri')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/ri/index.html',
      '/usao/ri/newsfeed.html',
      '/usao/ri/news.html',
      '/usao/ri/projects/psn_copy.html',
      '/usao/ri/complaint.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-ri.
 *
 * @package doj_migration
 * @subpackage usao-ri
 */
class UsaoRiFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ri subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ri',
    );

    $dest_dir = 'public:///usao-ri/docs';
    $regex = NULL;
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
    return "usao-ri";
  }
}

/**
 * Migrates press releases from /usao-ri.
 *
 * @package doj_migration
 * @subpackage usao-ri
 */
class UsaoRiPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-ri";
    $arguments['component_tid'] = 2046;
    $arguments['source_parser_class'] = "UsaoRiPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoRiObtainTitlePressRelease");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array('h2', 1));
    $title->addMethod('pluckAndConcatAnySelector', array("[align='center'] > strong > u"));
    $title->addMethod('pluckAndConcatAnySelector', array("p[align='center'] > strong > u"));
    $title->addMethod('pluckAndConcatAnySelector', array("p[align='center'] > strong"));
    $title->addMethod('pluckAndConcatAnySelector', array("h3[align='center'] > strong"));
    $title->addMethod('pluckAndConcatAnySelector', array("p[align='center'] > span > u"));
    $title->addMethod('pluckAndConcatAnySelector', array("p[align='center'] > u"));

    $title->addMethod('pluckSelector', array("#headline", 1));

    $subtitle = new ObtainerInfo('subtitle', "UsaoRiObtainTitlePressRelease");
    $subtitle->addMethod('pluckSelector', array("p[align='center'] > strong > em", 1));
    $subtitle->addMethod('pluckAndConcatAnySelector', array("p[align='center'] > em"));
    $subtitle->addMethod('pluckAndConcatAnySelector', array("p[align='center'] > span"));

    $date = new ObtainerInfo('date', "UsaoRiObtainDate");
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckAnySelectorUntilValid', array('p > strong'));
    $date->addMethod('pluckProbableDate');
    $date->addMethod('pluckAndConcatAnySelector', array("p[align='center']"));

    $pr_number = new ObtainerInfo('prNumber', "UsaoRiObtainPrNumber");

    $body = new ObtainerInfo('body', "UsaoRiObtainBody");
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $subtitle;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;
    $arguments['obtainers_info'][] = $body;

    $source_dirs = array(
      1 => 'usao/ri/news/2013',
      2 => 'usao/ri/news/2014',
      3 => 'usao/ri/news/2015',
    );
    $options = array('recurse' => TRUE);
    $extension_regex = '/.*\.(html|htm)$/i';
    parent::__construct($arguments, $source_dirs, $options, $extension_regex);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/ri/news/2014/sept2014/newsfeed.html',
      '/usao/ri/news/2014/index.html',
      '/usao/ri/news/2013/index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-ri pages.
 *
 * @package doj_migration
 * @subpackage usao-ri
 */
class UsaoRiPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();

    $this->queryPath->find('#header')->remove();
    $this->queryPath->find('#headerimage')->remove();
    $this->queryPath->find('#top')->remove();
    $this->queryPath->find('#left')->remove();
    $this->queryPath->find('#right')->remove();
    $this->queryPath->find('#footer')->remove();
    $this->queryPath->find('#topnav')->remove();
    $this->queryPath->find('#nav')->remove();
    $this->queryPath->find('#gs')->remove();
    $this->queryPath->find('form')->remove();
    $this->queryPath->find('img[alt="Placeholder Banner Image"]')->remove();
    $this->queryPath->find('img[alt="DOJ Seal"]')->remove();
    $this->queryPath->find('a[title="AccessKey 2: Skip to Content"]')->remove();
  }
}

/**
 * SourceParser for /usao-ri press releases.
 *
 * @package doj_migration
 * @subpackage usao-ri
 */
class UsaoRiPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();

    $this->queryPath->find('p >strong')->remove();
  }
}

/**
 * Obtainer for title property for /usao-ri pages.
 *
 * @package doj_migration
 * @subpackage usao-ri
 * @see Obtainer.api.php
 */
class UsaoRiObtainTitlePage extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public function validateString($string) {
    if (stripos($string, "United States Attorney's Office") !== FALSE) {
      return FALSE;
    }
    return parent::validateString($string);
  }
}

/**
 * Obtainer for title property for /usao-ri press releases.
 *
 * @package doj_migration
 * @subpackage usao-ri
 * @see Obtainer.api.php
 */
class UsaoRiObtainTitlePressRelease extends ObtainTitlePressRelease {
}

/**
 * Obtainer for prNumber property for /usao-ri press releases.
 *
 * @package doj_migration
 * @subpackage usao-ri
 * @see Obtainer.api.php
 */
class UsaoRiObtainPrNumber extends ObtainId {
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
 * Custom obtainer for date property for /usao-ri.
 *
 * @package doj_migration
 * @subpackage usao-ri
 * @see Obtainer.api.php
 */
class UsaoRiObtainDate extends ObtainDate {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'CRT',
      'U S Department of Justice',
    );
    $text = str_replace($remove, '', $text);

    return $text;
  }
}

/**
 * Custom obtainer for body property for /usao-ri.
 *
 * @package doj_migration
 * @subpackage usao-ri
 * @see Obtainer.api.php
 */
class UsaoRiObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);

    return $text;
  }
}
