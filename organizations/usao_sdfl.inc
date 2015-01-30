<?php

/**
 * @file
 * Defines migration classes for Southern District of Florida.
 */

/**
 * Migrates .html files from /usao-sdfl to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdfl
 */
class UsaoSdflPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/fls',
      7 => 'usao/fls/newsArchive',
      12 => 'usao/fls/programs/VictimWitness',
      13 => 'usao/fls/programs',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Southern District of Florida.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdfl')
    ->sourceMigration('UsaoDistrict');
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
 * Migrates .html files from /usao-sdfl to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdfl
 */
class UsaoSdflPageEspanolMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/fls/Languages/Spanish',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates Spanish pages from the Southern District of Florida.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdfl')
    ->sourceMigration('UsaoDistrict');
    $this->addFieldMapping('language')->defaultValue('es');
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
 * Migrates files (*.pdf) from /usao-sdfl.
 *
 * @package doj_migration
 * @subpackage usao-sdfl
 */
class UsaoSdflFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/fls subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/fls',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-sdfl/docs';

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
    return "usao-sdfl";
  }
}

/**
 * Migrates press releases from /usao-sdfl.
 *
 * @package doj_migration
 * @subpackage usao-sdfl
 */
class UsaoSdflPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdfl";
    $arguments['component_tid'] = 1771;

    $source_dirs = array(
      2 => 'usao/fls/PressReleases/2013',
      5 => 'usao/fls/PressReleases/2014',
      6 => 'usao/fls/PressReleases',
    );
    $options = array('recurse' => FALSE);
    parent::__construct($arguments, $source_dirs, $options);
  }
}
