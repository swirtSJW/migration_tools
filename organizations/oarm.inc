<?php

/**
 * @file
 * Office of Attorney Recruitment & Management OARM.
 * 
 * Defines migration classes for the OARM section of justice.gov.
 */

/**
 * Migrates .html files from /oarm to page nodes.
 *
 * @package doj_migration
 * @subpackage oarm
 */
class OarmPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'oarm',
      'oarm/aslrp',
      'oarm/wb',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Attorney Recruitment & Management section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('oarm')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    $title = $row->title;
    // Let's clean up the titles from » and the weird blank char also.
    $count = 1;
    while ($count > 0) {
      $title = str_replace("»", "", $title, $count);
    }
    $count = 1;
    while ($count > 0) {
      $title = str_replace(" ", "", $title, $count);
    }

    $row->title = $title;
  }
}

/**
 * Migrates files (*.pdf) from /oarm.
 *
 * @package doj_migration
 * @subpackage oarm
 */
class OarmFileMigration extends JusticeBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates files (*.pdf) from the oarm subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'oarm',
      'oarm/aslrp',
      'oarm/wb',
    );
    $regex = '/.*\.pdf/';
    $dest_dir = 'public://oarm/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }
}

/**
 * Class UsersMigration.
 *
 * @package doj_migration
 */
class OarmUserMigration extends JusticeCsvMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $source_file = __DIR__ . '/../sources/oarm-users.csv';
    parent::__construct($arguments, $source_file);
    $this->description = t('Migrates OARM users.');

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'csvrownum' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Row Number',
        ),
      ),
      MigrateDestinationUser::getKeySchema()
    );
    $this->destination = new MigrateDestinationUser();

    $this->addFieldMapping("name", "email");
    $this->addFieldMapping("mail", "email");
    $this->addFieldMapping("role_names", "roles")->separator("|");
    $this->addFieldMapping("og_user_node")->defaultValue('oarm')->sourceMigration('Organization');
    $this->addFieldMapping("status")->defaultValue(1);
  }

  /**
   * {@inheritdoc}
   */
  public function csvcolumns() {
    // Note: Remember to subtract 1 from column number.
    $columns[0] = array('first_name', 'First Name');
    $columns[1] = array('last_name', 'Last Name');
    $columns[2] = array('email', 'Email');
    $columns[3] = array('group_role', 'Group Role');
    $columns[4] = array('roles', 'Roles');

    return $columns;
  }

  /**
   * {@inheritdoc}
   */
  public function fields() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, stdClass $row) {
    $gid = $this->getGID($entity);
    if ($gid) {
      $uid = $entity->uid;
      $roles = og_roles("node", "organization");
      $rid = array_search($row->group_role, $roles);
      og_role_grant("node", $gid, $uid, $rid);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRollback($entity_id) {
    foreach ($entity_id as $uid) {
      $user = user_load($uid);
      $gid = $this->getGID($user);
      if ($gid) {
        $roles = og_roles("node", "organization");
        foreach (array_keys($roles) as $rid) {
          og_role_revoke("node", $gid, $user->uid, $rid);
        }
      }
    }
  }

  /**
   * Get the group id from a user.
   * 
   * @param object $user
   *   A user entity.
   */
  protected function getGID($user) {
    $items = field_get_items("user", $user, "og_user_node");
    if (!empty($items)) {
      foreach ($items as $item) {
        return $item['target_id'];
      }
    }
    return FALSE;
  }
}
