<?php

/**
 * @file
 * Defines JusticeBinaryFileMigration class.
 */

/**
 * Class JusticeBinaryFileMigration.
 *
 * @package doj_migration
 */
abstract class JusticeBinaryFileMigration extends JusticeFileSourceMigration {

  /**
   * Prepare a file migration.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   * @param array $source_dirs
   *   An array of source directories, relative to $this->baseDir.
   * @param string $dest_dir
   *   The destination directory uri, with public:// or private::/ prefix.
   * @param string $regex
   *   The file mask. Only filenames matching this regex will be migrated.
   */
  public function __construct($arguments, $source_dirs, $dest_dir, $regex, $options = array()) {
    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'fileid' => t('File Location'),
    );
    // Parent constructor will setup $this->source.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

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
    $this->addFieldMapping('destination_dir', 'destination_dir')->defaultValue($dest_dir);

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
    $row->filepath = $this->baseDir . $row->fileid;

    // Nest migrated files deeply in directories. This will help to avoid
    // the gluster file system limitation caused by having too many files in a
    // single directory.
    $org_abbr = $this->getOrganizationAbbreviation();
    $file_created = filemtime($row->filepath);
    $year_created = date('Y', $file_created);
    $month_created = date('m', $file_created);
    $date_created = date('d', $file_created);
    $row->destination_dir = "public:///$org_abbr/legacy/$year_created/$month_created/$date_created";
  }

  /**
   * Implements Migration::complete().
   *
   * Creates a redirect from the legacy filepath.
   */
  public function complete($file, $row) {
    if (module_exists('redirect')) {
      // Strip leading /.
      $src = substr($row->fileid, 1);
      $redirect = redirect_load_by_source($src);
      if (!$redirect) {
        // Strip leading public://.
        $file_public_path = variable_get('file_public_path', conf_path() . '/files/');
        $dest = str_replace('public:/', $file_public_path, $file->uri);
        // Create redirect.
        $redirect = new stdClass();
        redirect_object_prepare($redirect);
        $redirect->source = $src;
        $redirect->redirect = $dest;

        redirect_save($redirect);
      }
    }
  }

  /**
   * Implements Migration::prepareRollback().
   *
   * Deletes redirects for entity.
   */
  public function prepareRollback($entity_ids) {
    // $entity_ids is in the form of array('destid1' => [id]).
    $entity_id = $entity_ids['destid1'];
    $entity_type = 'file';
    $entities = entity_load($entity_type, array($entity_id));
    $entity = reset($entities);
    // Strip leading public://.
    $file_public_path = variable_get('file_public_path', conf_path() . '/files/');
    $dest = str_replace('public://', $file_public_path, $entity->uri);
    redirect_delete_by_path($dest);
  }
}

/**
 * Adding functionality that allows files to be added to a deployment plan
 */
abstract class JusticeDeployableBinaryFileMigration extends JusticeBinaryFileMigration {

  protected $deploymentPlan;

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $source_dirs, $dest_dir, $regex, $options = array()) {
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
    $abbrev = $this->getOrganizationAbbreviation();
    $this->deploymentPlan = _doj_deploy_find_plan_by_organization_abbreviation($abbrev);
  }

  /**
   * {@inheritdoc}
   */
  public function complete($file, $row) {
    parent::complete($file, $row);

    // Let's add files to a deployment plan.
    if ($this->deploymentPlan) {
      deploy_manager_add_to_plan($this->deploymentPlan, "file", $file);
    }
  }

  /**
   * Get the abbreviation of an organization.
   *
   * @return string
   *   The abbreviation.
   */
  abstract public function getOrganizationAbbreviation();
}
