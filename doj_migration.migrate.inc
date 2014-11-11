<?php

/**
 * @file
 * Contains definitions of doj migrations.
 *
 * @package doj_migration
 */

/**
 * Implements hook_migrate_api().
 */
function doj_migration_migrate_api() {
  $disable_hooks = array(
    'node_update' => array(
      'akamai',
      'expire',
    ),
    'node_delete' => array(
      'akamai',
      'expire',
    ),
  );

  $api = array(
    'api' => 2,
    'groups' => array(
      'core' => array(
        'title' => t('Core Entities'),
        'disable_hooks' => $disable_hooks,
      ),
      'doj' => array(
        'title' => t('DOJ - Department of Justice'),
        'disable_hooks' => $disable_hooks,
      ),
      'eousa' => array(
        'title' => t('EOUSA - Executive Office for United States Attorneys'),
        'disable_hooks' => $disable_hooks,
      ),
      'espanol' => array(
        'title' => t('espanol - Justice.gov en Espanol'),
        'disable_hooks' => $disable_hooks,
      ),
      'ola' => array(
        'title' => t('OLA - Office of Legislative Affairs'),
        'disable_hooks' => $disable_hooks,
      ),
      'usao' => array(
        'title' => t('USAO - United States Attorneys Office'),
        'disable_hooks' => $disable_hooks,
      ),
      'usao-ndca' => array(
        'title' => t('Northern District of California'),
        'disable_hooks' => $disable_hooks,
      ),
      'usao-wdny' => array(
        'title' => t('Western District of New York'),
        'disable_hooks' => $disable_hooks,
      ),
    ),
    'migrations' => array(
      'EspanolFile' => array(
        'group_name' => 'espanol',
        'class_name' => 'EspanolFileMigration',
      ),
      'EspanolPage' => array(
        'group_name' => 'espanol',
        'class_name' => 'EspanolPageMigration',
      ),
      'EspanolPressRelease' => array(
        'group_name' => 'espanol',
        'class_name' => 'EspanolPressReleaseMigration',
      ),
      'EspanolSpeech' => array(
        'group_name' => 'espanol',
        'class_name' => 'EspanolSpeechMigration',
      ),
      'Organization' => array(
        'group_name' => 'core',
        'class_name' => 'OrganizationMigration',
      ),
      'OlaFile' => array(
        'group_name' => 'ola',
        'class_name' => 'OlaFileMigration',
      ),
      'OlaPage' => array(
        'group_name' => 'ola',
        'class_name' => 'OlaPageMigration',
      ),
      'OlaTestimony' => array(
        'group_name' => 'ola',
        'class_name' => 'OlaTestimonyMigration',
      ),
      'OlaWitness' => array(
        'group_name' => 'ola',
        'class_name' => 'OlaWitnessMigration',
      ),
      'OlaDbTestimony' => array(
        'group_name' => 'ola',
        'class_name' => 'OlaDbTestimonyMigration',
      ),
      'OlaDbWitness' => array(
        'group_name' => 'ola',
        'class_name' => 'OlaDbWitnessMigration',
      ),
      'UsaoDistrict' => array(
        'group_name' => 'usao',
        'class_name' => 'UsaoDistrictMigration',
      ),
      'UsaoStaffProfile' => array(
        'group_name' => 'usao',
        'class_name' => 'UsaoStaffProfileMigration',
      ),
      'UsaoFile' => array(
        'group_name' => 'usao',
        'class_name' => 'UsaoFileMigration',
      ),
      'UsaoPage' => array(
        'group_name' => 'usao',
        'class_name' => 'UsaoPageMigration',
      ),
      'UsaoNdcaFile' => array(
        'group_name' => 'usao-ndca',
        'class_name' => 'UsaoNdcaFileMigration',
      ),
      'UsaoNdcaPage' => array(
        'group_name' => 'usao-ndca',
        'class_name' => 'UsaoNdcaPageMigration',
      ),
      'UsaoWdnyFile' => array(
        'group_name' => 'usao-wdny',
        'class_name' => 'UsaoWdnyFileMigration',
      ),
      'UsaoWdnyPage' => array(
        'group_name' => 'usao-wdny',
        'class_name' => 'UsaoWdnyPageMigration',
      ),
    ),
  );
  return $api;
}
