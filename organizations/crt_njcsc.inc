<?php

/**
 * @file
 * Migration classes for US v. State of New Jersey Civil Service Commission.
 */

/**
 * Migrates .html files from /crt-njcsc to page nodes.
 *
 * @package doj_migration
 * @subpackage crt-njcsc
 */
class CrtNjcscPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'crt/spec_topics/njcsc',
    );

    $options = array('recurse' => TRUE);
    $extension_regex = '/.*\.(html|htm|php)$/i';

    $arguments['source_parser_class'] = "CrtNjcscPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CrtNjcscObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("h2", 1));
    $title->addMethod('pluckSelector', array("title", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the US v. State of New Jersey Civil Service Commission.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('crt-njcsc')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/crt/spec_topics/njcsc/documents/index.php',
      '/crt/spec_topics/njcsc/index.php',
      '/crt/spec_topics/njcsc/_page_mods.php',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /crt-njcsc.
 *
 * @package doj_migration
 * @subpackage crt-njcsc
 */
class CrtNjcscFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the crt/spec_topics/njcsc subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'crt/spec_topics/njcsc',
    );

    $dest_dir = 'public:///crt-njcsc/docs';
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
    return "crt-njcsc";
  }
}

/**
 * SourceParser for /crt-njcsc pages.
 *
 * @package doj_migration
 * @subpackage crt-njcsc
 */
class CrtNjcscPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /crt-njcsc pages.
 *
 * @package doj_migration
 * @subpackage crt-njcsc
 * @see Obtainer.api.php
 */
class CrtNjcscObtainTitlePage extends ObtainTitle {

}
