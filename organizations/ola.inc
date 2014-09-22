<?php
/**
 * @file
 * Office of Legislative Affairs OLA.
 *
 * Defines migration classes for the OLA section of justice.gov.
 */

class OlaTestimonyMigration extends JusticeFileSourceMigration {
  // Migration from {cms_ola_testimony} in cmsopa table.
  // Join to cms_ola_doclinks to import attachments!
}

/**
 * Migrates .html files from /ola to page nodes.
 *
 * @package doj_migration
 * @subpackage ola
 */
class OlaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $dir_json = '["ola","ola\/views-letters"]';
    // Define source directories.
    $source_dirs = drupal_json_decode($dir_json);

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    $arguments['source_parser_class'] = "OlaPageSourceParser";
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Legislative Affairs section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('ola')->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /ola.
 *
 * @package doj_migration
 * @subpackage ola
 */
class OlaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the ola subdirectory.');
    $this->dependencies = array('Organization');

    $dir_json = '["ola\/views-letters\/113","ola\/views-letters\/111-1","ola\/views-letters\/111-2","ola\/views-letters\/112"]';

    // Match .pdf files only.
    $source_dirs = drupal_json_decode($dir_json);

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///ola/docs';

    $options = array('recurse' => FALSE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "ola";
  }
}
