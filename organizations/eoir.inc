<?php

/**
 * @file
 * Defines migration classes for Executive Office for Immigration Review.
 */

/**
 * Migrates .html files from /eoir to page nodes.
 *
 * @package doj_migration
 * @subpackage eoir
 */
class EoirPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      'eoir',
    );

    $arguments['source_parser_class'] = "EoirSourceParser";

    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Executive Office for Immigration Review.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('eoir')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /eoir.
 *
 * @package doj_migration
 * @subpackage eoir
 */
class EoirFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the eoir subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'eoir',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///eoir/docs';

    $options = array('recurse' => TRUE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $file_id = $row->fileid;
    $enc = mb_detect_encoding($file_id);
    if ($enc === FALSE) {
      module_load_include('inc', 'doj_migration', 'includes/doj_migration');
      $row->fileid = doj_migration_random_string(10);
      drush_doj_migration_debug_output("Ignored path: {$file_id} --> stored id: {$row->fileid}");
      return FALSE;
    }
    else {
      drush_doj_migration_debug_output("Path: {$file_id} --> encoding: {$enc}");
    }

    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "eoir";
  }
}

class EoirPressReleaseMigration extends JusticeHtmlToPressReleaseMigration {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    $source_dirs = array(
      "eoir/press/2014",
      "eoir/press/2013",
    );

    $options = array(
      'recurse' => TRUE,
    );

    $arguments['source_parser_class'] = "EoirPressSourceParser";

    $arguments['obtainer_methods'] = array(
      'title' => array(
        'findSecondPAlignCenterStrong',
      ),
      'body' => array(),
      'subtitle' => array(),
      'date' => array(
        'findTable3Row3Col2',
      ),
    );

    parent::__construct($arguments, $source_dirs, $options);

    $this->dependencies = array('Organization');
    $this->description = t('Migrates Press Releases from EOIR.');
    $this->addFieldMapping('og_group_ref')->defaultValue('eoir')->sourceMigration('Organization');
  }
}
