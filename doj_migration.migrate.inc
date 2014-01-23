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
      'CareersFile' => array('class_name' => 'CareersFileMigration'),
      'CareersInternship' => array('class_name' => 'CareersInternshipMigration'),
      'CareersJob' => array('class_name' => 'CareersJobMigration'),
      'CareersPage' => array('class_name' => 'CareersPageMigration'),
      'UsaoDistrict' => array('class_name' => 'UsaoDistrictMigration'),
      'UsaoStaffProfile' => array('class_name' => 'UsaoStaffProfileMigration'),
      'Organization' => array('class_name' => 'OrganizationMigration'),
    ),
  );
  return $api;
}
