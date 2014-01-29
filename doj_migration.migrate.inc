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
    'groups' => array(
      'ag' => array(
        'title' => t('Office of the Attorney General'),
      ),
      'core' => array(
        'title' => t('Core Entities'),
      ),
      'careers' => array(
        'title' => t('Careers'),
      ),
      'ovw' => array(
        'title' => t('OVW - Office on Violence Against Women'),
      ),
      'usao' => array(
        'title' => t('USAO - United States Attorneys Office'),
      ),

    ),
    'migrations' => array(
      'AgPressRelease' => array(
        'group_name' => 'ag',
        'class_name' => 'AgPressReleaseMigration'
      ),
      'Organization' => array(
        'group_name' => 'core',
        'class_name' => 'OrganizationMigration'
      ),
      'CareersFile' => array(
        'group_name' => 'careers',
        'class_name' => 'CareersFileMigration'
      ),
      'CareersInternship' => array(
        'group_name' => 'careers',
        'class_name' => 'CareersInternshipMigration'
      ),
      'CareersJob' => array(
        'group_name' => 'careers',
        'class_name' => 'CareersJobMigration'
      ),
      'CareersPage' => array(
        'group_name' => 'careers',
        'class_name' => 'CareersPageMigration'
      ),
      'UsaoDistrict' => array(
        'group_name' => 'usao',
        'class_name' => 'UsaoDistrictMigration'
      ),
      'UsaoStaffProfile' => array(
        'group_name' => 'usao',
        'class_name' => 'UsaoStaffProfileMigration'
      ),
    ),
  );
  return $api;
}
