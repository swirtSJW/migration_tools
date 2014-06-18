<?php

/**
 * @file
 * Community Relations Service.
 *
 * Defines migration classes for the CRS section of justice.gov.
 */

/**
 * Migrates .html files from /crs to page nodes.
 *
 * @package doj_migration
 * @subpackage crs
 */
class CrsPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'crs',
      'crs/pubs',
      'crs/video',
      'crs/training_video/3to5_100k',
      'crs/training_video/3to5_300k',
      'crs/training_video/3to5_56k',
      'crs/training_video/3to5_lan',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Community Relations Service section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('crs')->sourceMigration('Organization');
  }
}


/**
 * Migrates non-image files from the /crs.
 *
 * @package doj_migration
 * @subpackage crs
 */
class CrsFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the crs subdirectory.');
    $this->dependencies = array('Organization');

    // Match non-image files.
    $source_dirs = array(
      'crs',
      'crs/pubs',
      'crs/video',
      'crs/training_video/3to5_100k',
      'crs/training_video/3to5_300k',
      'crs/training_video/3to5_56k',
      'crs/training_video/3to5_lan',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx|swf|flv)/';
    $dest_dir = 'public:///crs/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return 'crs';
  }
}

/**
 * Migrates users for CRS organization.
 */
class CrsUserMigration extends JusticeUserMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $source_file = __DIR__ . '/../sources/crs-users.csv';
    $this->description = t('Migrates CRS users.');

    parent::__construct($arguments, $source_file);

    $this->addFieldMapping("og_user_node")->defaultValue('crs')->sourceMigration('Organization');
  }
}
