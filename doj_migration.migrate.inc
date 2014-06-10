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
      'fcsc' => array(
        'title' => t('FCSC - Foreign Claims Settlement Commission'),
      ),
      'interpol' => array(
        'title' => t('INTERPOL'),
      ),
      'oarm' => array(
        'title' => t('OARM - Office of Attorney Recruitment & Management'),
      ),
      'olp' => array(
        'title' => t('OLP - Office of Legal Policy'),
      ),
      'opa' => array(
        'title' => t('OPA - Office of Public Affairs'),
      ),
      'opcl' => array(
        'title' => t('OPCL - Office of Privacy and Civil Liberties'),
      ),
      'otj' => array(
        'title' => t('OTJ - Office of Tribal Justice'),
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
        'class_name' => 'AgPressReleaseMigration',
      ),
      'CareersFile' => array(
        'group_name' => 'careers',
        'class_name' => 'CareersFileMigration',
      ),
      'CareersInternship' => array(
        'group_name' => 'careers',
        'class_name' => 'CareersInternshipMigration',
      ),
      'CareersJob' => array(
        'group_name' => 'careers',
        'class_name' => 'CareersJobMigration',
      ),
      'CareersPage' => array(
        'group_name' => 'careers',
        'class_name' => 'CareersPageMigration',
      ),
      'FcscPage' => array(
        'group_name' => 'fcsc',
        'class_name' => 'FcscPageMigration',
      ),
      'FcscFile' => array(
        'group_name' => 'fcsc',
        'class_name' => 'FcscFileMigration',
      ),
      'InterpolPage' => array(
        'group_name' => 'interpol',
        'class_name' => 'InterpolPageMigration',
      ),
      'InterpolNews' => array(
        'group_name' => 'interpol',
        'class_name' => 'InterpolNewsMigration',
      ),
      'InterpolFile' => array(
        'group_name' => 'interpol',
        'class_name' => 'InterpolFileMigration',
      ),
      'Organization' => array(
        'group_name' => 'core',
        'class_name' => 'OrganizationMigration',
      ),
      'OARMFile' => array(
        'group_name' => 'oarm',
        'class_name' => 'OarmFileMigration',
      ),
      'OARMPage' => array(
        'group_name' => 'oarm',
        'class_name' => 'OarmPageMigration',
      ),
      'OARMUser' => array(
        'group_name' => 'oarm',
        'class_name' => 'OarmUserMigration',
      ),
      'OlpPage' => array(
        'group_name' => 'olp',
        'class_name' => 'OlpPageMigration',
      ),
      'OpaBlogEntry' => array(
        'group_name' => 'opa',
        'class_name' => 'OpaBlogEntryMigration',
      ),
      'OpaAuthor' => array(
        'group_name' => 'opa',
        'class_name' => 'OpaAuthorMigration',
      ),
      'OpaAttachment' => array(
        'group_name' => 'opa',
        'class_name' => 'OpaAttachmentMigration',
      ),
      'OpclPage' => array(
        'group_name' => 'opcl',
        'class_name' => 'OpclPageMigration',
      ),
      'OpclFile' => array(
        'group_name' => 'opcl',
        'class_name' => 'OpclFileMigration',
      ),
      'OtjPage' => array(
        'group_name' => 'otj',
        'class_name' => 'OtjPageMigration',
      ),
      'OtjFile' => array(
        'group_name' => 'otj',
        'class_name' => 'OtjFileMigration',
      ),
      'UsaoDistrict' => array(
        'group_name' => 'usao',
        'class_name' => 'UsaoDistrictMigration',
      ),
      'UsaoStaffProfile' => array(
        'group_name' => 'usao',
        'class_name' => 'UsaoStaffProfileMigration',
      ),
    ),
  );
  return $api;
}
