<?php

/**
 * @file
 * Defines migration classes for Victim Notification Program.
 */

/**
 * Migrates .html files from /criminal-vns to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-vns
 */
class CriminalVnsPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/vns/about',
    );

    $options = array('recurse' => FALSE);

    $arguments['source_parser_class'] = "CriminalVnsPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalVnsObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("title", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Victim Notification Program.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-vns')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array();
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /criminal-vns.
 *
 * @package doj_migration
 * @subpackage criminal-vns
 */
class CriminalVnsFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/vns subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/vns',
    );

    $dest_dir = 'public:///criminal-vns/docs';
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
    return "criminal-vns";
  }
}

/**
 * Migrates .html files from /criminal-vns to case nodes.
 *
 * @package doj_migration
 * @subpackage criminal/vns
 */
class CriminalVnsCaseMigration extends NGJusticeHtmlToCaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    $arguments['organization'] = "criminal-vns";

    // Define source directories.
    $source_dirs = array(
      'criminal/vns/caseup',
    );

    $options = array('recurse' => FALSE);

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalVnsObtainTitleCase");
    $title->addMethod('pluckSelector', array('p[align="center"] > b', 1));
    $title->addMethod('pluckSelector', array("title", 1));

    $arguments['obtainers_info'][] = $title;
    $arguments['source_parser_class'] = "CriminalVnsCaseSourceParser";
    $arguments['component_tid'] = 4276;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates cases from the Fraud Section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('criminal-fraud')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/criminal/vns/index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /criminal-vns pages.
 *
 * @package doj_migration
 * @subpackage criminal-vns
 */
class CriminalVnsPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /criminal-fraud cases.
 *
 * @package doj_migration
 * @subpackage criminal-ceos
 */
class CriminalVnsCaseSourceParser  extends NGCaseSourceParser {

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {

    parent::cleanHtml();
  }
}


/**
 * Obtainer for title property for /criminal-vns pages.
 *
 * @package doj_migration
 * @subpackage criminal-vns
 * @see Obtainer.api.php
 */
class CriminalVnsObtainTitlePage extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'USDOJ: CRM: VNS:',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}

/**
 * Obtainer for title property for /criminal-vns cases.
 *
 * @package doj_migration
 * @subpackage criminal-vns
 * @see Obtainer.api.php
 */
class CriminalVnsObtainTitleCase extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'USDOJ: CRM: VNS:',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}
