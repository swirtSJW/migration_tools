<?php

/**
 * @file
 * Defines migration classes for Office of International Affairs.
 */

/**
 * Migrates .html files from /criminal-oia to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-oia
 */
class CriminalOiaPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/oia',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "CriminalOiaPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalOiaObtainTitlePage");
    $title->addMethod('pluckSelector', array(".headline", 1));
    $title->addMethod('pluckSelector', array("h1", 1));

    $title->addMethod('pluckSelector', array("title", 1));

    $arguments['obtainers_info'][] = $title;

    $extension_regex = "/.*\.(html|htm)$/i";
    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Office of International Affairs.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-oia')
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
 * Migrates binary files from /criminal-oia.
 *
 * @package doj_migration
 * @subpackage criminal-oia
 */
class CriminalOiaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/oia subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/oia',
    );

    $dest_dir = 'public:///criminal-oia/docs';
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
    return "criminal-oia";
  }
}

/**
 * SourceParser for /criminal-oia pages.
 *
 * @package doj_migration
 * @subpackage criminal-oia
 */
class CriminalOiaPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-oia pages.
 *
 * @package doj_migration
 * @subpackage criminal-oia
 * @see Obtainer.api.php
 */
class CriminalOiaObtainTitlePage extends ObtainTitle {

}
