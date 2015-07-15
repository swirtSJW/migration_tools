<?php

/**
 * @file
 * Defines migration classes for United States v. City of New York FDNY
 * Employment Discrimination Case.
 */

/**
 * Migrates .html files from /crt-fdny to page nodes.
 *
 * @package doj_migration
 * @subpackage crt-fdny
 */
class CrtFdnyPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'crt/spec_topics/fdny',
    );

    $options = array('recurse' => TRUE);
    $extension_regex = '/.*\.(htm|html|php)$/i';

    $arguments['source_parser_class'] = "CrtFdnyPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CrtFdnyObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("title", 1));
    $body = new ObtainerInfo('body', 'CrtFdnyObtainBody');
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $body;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the United States v. City of New York FDNY Employment Discrimination Case.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('crt-fdny')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/crt/spec_topics/fdny/form/problem.php',
      '/crt/spec_topics/fdny/form/index.php',
      '/crt/spec_topics/fdny/form/filled.php',
      '/crt/spec_topics/fdny/form/submitted.php',
      '/crt/spec_topics/fdny/index.php',
      '/crt/spec_topics/fdny/_include.php',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /crt-fdny.
 *
 * @package doj_migration
 * @subpackage crt-fdny
 */
class CrtFdnyFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the crt/spec_topics/fdny subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'crt/spec_topics/fdny',
    );

    $dest_dir = 'public:///crt-fdny/docs';
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
    return "crt-fdny";
  }
}

/**
 * SourceParser for /crt-fdny pages.
 *
 * @package doj_migration
 * @subpackage crt-fdny
 */
class CrtFdnyPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /crt-fdny pages.
 *
 * @package doj_migration
 * @subpackage crt-fdny
 * @see Obtainer.api.php
 */
class CrtFdnyObtainTitlePage extends ObtainTitle {

}

/**
 * Custom obtainer for body property for /crt-fdny.
 *
 * @package doj_migration
 * @subpackage crt-fdny
 * @see Obtainer.api.php
 */
class CrtFdnyObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      '$page->',
      '$page-',
      'end();',
      "include 'disclaimer.inc';",
      'start();',
      "option_set( 'charset', 'UTF-8' );",
      "add_crumb( 'Overview' );",
      "include '_include.php';",
      "option_set( 'date-updated', '2011-04-21' );",
      "// optional. If not used no updated date will be displayed.",
      "option_set( 'date-updated', '2010-12-10' );",

      "add_meta( 'date created', '2010-12-28' );",
      "add_meta( 'date created', '2010-12-10' );",
      ". ' Employment, Fire Department, New York, case overview'",
      ". ' Employment, Fire Department, New York, FAQ'",
      "'Civil Rights Division, '",
      "add_meta( 'keywords',",
      "add_meta( 'description',",
      "add_crumb( 'FAQ' );",
      "add_crumb( 'FDNY', '/crt/spec_topics/fdny' );",
      "add_crumb( 'Documents' );",
      "add_crumb( 'Case Timeline",
      "= new pageLayout( 'Civil Rights Division FDNY Overview' );",
      'include_once "{$_SERVER[ \'DOCUMENT_ROOT\' ]}/crt/_layout.class";',
      "'This is the contact information for the Employment section case against the Fire Department of New York.'",
      "'This is the frequently asked questions about the Employment section case against the Fire Department of New York.'",
      ". ' Employment, Fire Department, New York, Court Decisions, Case-Related Documents'",
      '		);',
      "add_to_head( '",
      "' );",
      '<?php',
      '?>',
      ' ? ',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}
