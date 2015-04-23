<?php

/**
 * @file
 * Defines migration classes for Organized Crime and Gang Section.
 */

/**
 * Migrates .html files from /criminal-ocgs to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-ocgs
 */
class CriminalOcgsPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/ocgs',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "CriminalOcgsPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalOcgsObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array('div.headline', 1));
    $title->addMethod('pluckSelector', array('div.headline', 2));
    $title->addMethod('findSelectorAttribute', array("div#print_content > div > img", "alt"));
    $title->addMethod('findSelectorAttribute', array("div.headline > img", "alt"));
    $title->addMethod('pluckSelector', array("title", 1));
    $title->addMethod('pluckSelector', array("h1", 1));

    $body = new ObtainerInfo('body', 'CriminalOcgsObtainBody');
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $body;

    // $extension_regex = /.*\.(html|htm)$/i;
    $extension_regex = NULL;
    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Organized Crime and Gang Section.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-ocgs')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/criminal/ocgs/index.html',
    );
    if ((parent::prepareRow($row) === FALSE) || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /criminal-ocgs.
 *
 * @package doj_migration
 * @subpackage criminal-ocgs
 */
class CriminalOcgsFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/ocgs subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/ocgs',
    );

    $dest_dir = 'public:///criminal-ocgs/docs';
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
    return "criminal-ocgs";
  }
}

/**
 * SourceParser for /criminal-ocgs pages.
 *
 * @package doj_migration
 * @subpackage criminal-ocgs
 */
class CriminalOcgsPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-ocgs pages.
 *
 * @package doj_migration
 * @subpackage criminal-ocgs
 * @see Obtainer.api.php
 */
class CriminalOcgsObtainTitlePage extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'USDOJ: CRM: OCGS: Organized Crime Groups: International Organized Crime',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}

/**
 * Obtainer for body property for /criminal-ocgs pages.
 * @package doj_migration
 * @subpackage usao-ndin
 * @see Obtainer.api.php
 */
class CriminalOcgsObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $remove = array(
      'margin-bottom:-15px',
      'margin-bottom:-10px',
      '<h2>Read More in PDF</h2>',
      '<h2>Statutes to Combat Organized Crime</h2>',
    );
    $text = str_ireplace($remove, '', $text);
    $text = parent::cleanString($text);

    return $text;
  }
}
