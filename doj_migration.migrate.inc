<?php

/**
 * @file
 * Contains definitions of migrations.
 */

/*
 * Implements hook_migrate_api().
 */
function doj_migration_migrate_api() {
  $api = array(
    'api' => 2,
    'migrations' => array(
      'CareersJob' => array('class_name' => 'CareersJobMigration'),
      'CareersFile' => array('class_name' => 'CareersFileMigration'),
      'CareersPage' => array('class_name' => 'CareersPageMigration'),
    ),
  );
  return $api;
}
