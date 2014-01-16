<?php

/**
 * @file
 * Defines JusticeBinaryFileMigration class.
 */

/**
 * Class JusticeBinaryFileMigration.
 *

 */
abstract class JusticeBinaryFileMigration extends JusticeFileSourceMigration {

  /**
   * Prepare a file migration.
   *
   * @param $group
   *   A migration group returned by MigrateGroup::getInstance().
   * @param array $source_dirs
   *   An array of source directories, relative to $this->base_dir.
   * @param string $regex
   *   The file mask. Only filenames matching this regex will be migrated.
   * @param string $dest_dir
   *   The destination directory uri, with public:// or private::/ prefix.
   */
  public function __construct($group, $source_dirs, $dest_dir, $regex) {
    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'fileid' => t('File Location'),
    );
    // Parent constructor will setup $this->source.
    parent::__construct($group, $source_fields, $source_dirs, $regex);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'fileid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationFile::getKeySchema()
    );

    // Set up destination.
    $this->destination = new MigrateDestinationFile();

    // Set up field mappings.
    $this->addFieldMapping('value', 'filepath');
    $this->addFieldMapping('destination_dir')->defaultValue($dest_dir);

    $this->addUnmigratedDestinations(array('fid', 'uid', 'timestamp', 'destination_file', 'source_dir', 'preserve_files',
      'file_replace'));
    $this->removeFieldMapping('path');
    $this->removeFieldMapping('pathauto');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);
    $row->filepath = $this->base_dir . $row->fileid;
  }
}
