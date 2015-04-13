<?php

/**
 * @file
 * Defines migration classes for Computer Crime & Intellectual Property Section.
 */

/**
 * Migrates .html files from /criminal-ccips to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-ccips
 */
class CriminalCcipsPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/cybercrime',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "CriminalCcipsPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalCcipsObtainTitlePage");
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findSelector', array('div.headline', 1));
    $title->addMethod('findSelector', array('div.headline', 2));
    $title->addMethod('findSelectorAttribute', array("div.headline > img", "alt"));
    $title->addMethod('findSelector', array("title", 1));
    $title->addMethod('findSelector', array("h1", 1));
    $title->addMethod('findSelector', array("h3", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Computer Crime &amp; Intellectual Property Section.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-ccips')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/criminal/cybercrime/press-releases/2004/dinhSent.htm',
    );

    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /criminal-ccips.
 *
 * @package doj_migration
 * @subpackage criminal-ccips
 */
class CriminalCcipsFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/cybercrime subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/cybercrime',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///criminal-ccips/docs';

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
    return "criminal-ccips";
  }
}

/**
 * SourceParser for /criminal-ccips pages.
 *
 * @package doj_migration
 * @subpackage criminal-ccips
 */
class CriminalCcipsPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-ccips pages.
 *
 * @package doj_migration
 * @subpackage criminal-ccips
 * @see Obtainer.api.php
 */
class CriminalCcipsObtainTitlePage extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $about_page = strstr($text, 'About The Computer Crime & Intellectual Property Section');
    if ($about_page === FALSE) {
      $remove = array(
        'Computer Crime & Intellectual Property Section',
        'USDOJ: CRM:',
      );
    }
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}
