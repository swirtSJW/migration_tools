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
      'usao-az' => array(
        'title' => t('District of Arizona'),
        'disable_hooks' => $disable_hooks,
      ),
      'usao' => array(
        'title' => t('USAO - United States Attorneys Office'),
        'disable_hooks' => $disable_hooks,
      ),
      'usao-edla' => array(
        'title' => t('Eastern District of Louisiana'),
        'disable_hooks' => $disable_hooks,
      ),
      'usao-id' => array(
        'title' => t('District of Idaho'),
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
      'usao-wdok' => array(
        'title' => t('Western District of Oklahoma'),
        'disable_hooks' => $disable_hooks,
      ),
      'usao-wdtx' => array(
        'title' => t('Western District of Texas'),
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
      'UsaoAzFile' => array(
        'group_name' => 'usao-az',
        'class_name' => 'UsaoAzFileMigration',
      ),
      'UsaoAzPage' => array(
        'group_name' => 'usao-az',
        'class_name' => 'UsaoAZPageMigration',
      ),
      'UsaoAzPressRelease' => array(
        'group_name' => 'usao-az',
        'class_name' => 'UsaoAzPressReleaseMigration',
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
      'UsaoEdlaFile' => array(
        'group_name' => 'usao-edla',
        'class_name' => 'UsaoEdlaFileMigration',
      ),
      'UsaoEdlaPage' => array(
        'group_name' => 'usao-edla',
        'class_name' => 'UsaoEdlaPageMigration',
      ),
      'UsaoEdlaPressRelease' => array(
        'group_name' => 'usao-edla',
        'class_name' => 'UsaoEdlaPressReleaseMigration',
      ),
      'UsaoIdFile' => array(
        'group_name' => 'usao-id',
        'class_name' => 'UsaoIdFileMigration',
      ),
      'UsaoIdPage' => array(
        'group_name' => 'usao-id',
        'class_name' => 'UsaoIdPageMigration',
      ),
      'UsaoIdPressRelease' => array(
        'group_name' => 'usao-id',
        'class_name' => 'UsaoIdPressReleaseMigration',
      ),
      'UsaoNdcaFile' => array(
        'group_name' => 'usao-ndca',
        'class_name' => 'UsaoNdcaFileMigration',
      ),
      'UsaoNdcaPage' => array(
        'group_name' => 'usao-ndca',
        'class_name' => 'UsaoNdcaPageMigration',
      ),
      'UsaoNdcaPressRelease' => array(
        'group_name' => 'usao-ndca',
        'class_name' => 'UsaoNdcaPressReleaseMigration',
      ),
      'UsaoWdnyFile' => array(
        'group_name' => 'usao-wdny',
        'class_name' => 'UsaoWdnyFileMigration',
      ),
      'UsaoWdnyPage' => array(
        'group_name' => 'usao-wdny',
        'class_name' => 'UsaoWdnyPageMigration',
      ),
      'UsaoWdnyPressRelease' => array(
        'group_name' => 'usao-wdny',
        'class_name' => 'UsaoWdnyPressReleaseMigration',
      ),
      'UsaoWdokFile' => array(
        'group_name' => 'usao-wdok',
        'class_name' => 'UsaoWdokFileMigration',
      ),
      'UsaoWdokPage' => array(
        'group_name' => 'usao-wdok',
        'class_name' => 'UsaoWdokPageMigration',
      ),
      'UsaoWdokPressRelease' => array(
        'group_name' => 'usao-wdok',
        'class_name' => 'UsaoWdokPressReleaseMigration',
      ),
      'UsaoWdtxFile' => array(
        'group_name' => 'usao-wdtx',
        'class_name' => 'UsaoWdtxFileMigration',
      ),
      'UsaoWdtxPage' => array(
        'group_name' => 'usao-wdtx',
        'class_name' => 'UsaoWdtxPageMigration',
      ),
      'UsaoWdtxPressRelease' => array(
        'group_name' => 'usao-wdtx',
        'class_name' => 'UsaoWdtxPressReleaseMigration',
      ),
    ),
  );
  return $api;
}
