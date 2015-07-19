<?php

/**
 * @file
 * Defines migration classes for Service Members.
 */

/**
 * Migrates .html files from /crt-military to page nodes.
 *
 * @package doj_migration
 * @subpackage crt-military
 */
class CrtMilitaryPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'crt/spec_topics/military',
    );

    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm|php)$/i';

    $arguments['source_parser_class'] = "CrtMilitaryPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CrtMilitaryObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("title", 1));

    $body = new ObtainerInfo('body', 'CrtMilitaryObtainBody');
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $body;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Service Members.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('crt-military')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/crt/spec_topics/military/divisions.html',
      '/crt/spec_topics/military/about.html',
      '/crt/spec_topics/military/meetattorney.html',
      '/crt/spec_topics/military/news.html',
      '/crt/spec_topics/military/cases.htm',
      '/crt/spec_topics/military/index_old.html',
      '/crt/spec_topics/military/uformatted_news.html',
      '/crt/spec_topics/military/statutes/statutes.html',
      '/crt/spec_topics/military/contact_us.htm',
      '/crt/spec_topics/military/press_releases.htm',
      '/crt/spec_topics/military/contact.html',
      '/crt/spec_topics/military/faq.htm',
      '/crt/spec_topics/military/faq.html',
      '/crt/spec_topics/military/programs.html',
      '/crt/spec_topics/military/file_claim.htm',
      '/crt/spec_topics/military/news_old.html',
      '/crt/spec_topics/military/other_sources.htm',
      '/crt/spec_topics/military/news.php',
      '/crt/spec_topics/military/news_template.php',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /crt-military.
 *
 * @package doj_migration
 * @subpackage crt-military
 */
class CrtMilitaryFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the crt/spec_topics/military subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'crt/spec_topics/military',
    );

    $dest_dir = 'public:///crt-military/docs';
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
    return "crt-military";
  }
}

/**
 * SourceParser for /crt-military pages.
 *
 * @package doj_migration
 * @subpackage crt-military
 */
class CrtMilitaryPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /crt-military pages.
 *
 * @package doj_migration
 * @subpackage crt-military
 * @see Obtainer.api.php
 */
class CrtMilitaryObtainTitlePage extends ObtainTitle {

}

/**
 * Obtainer for body property for /usao-ndil press releases.
 *
 * @package doj_migration
 * @subpackage usao-ndil
 * @see Obtainer.api.php
 */
class CrtMilitaryObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {

    $text = parent::cleanString($text);
    $text = preg_replace("/<h1\><img[^>]+\><\/h1\>/i", "", $text);
    $text = preg_replace("/<h1\><a name=\"top\" id=\"top\" \> <img[^>]+\><\/a\><\/h1\>/i", "", $text);
    $text = preg_replace("/<h1\><a id=\"top\" name=\"top\" \> <img[^>]+\><\/a\><\/h1\>/i", "", $text);
    $text = preg_replace('/<div class=\"breadcrumb\">(.*?)<\/div>/s', "", $text);

    return $text;
  }
}
