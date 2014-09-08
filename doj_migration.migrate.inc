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
      'asg' => array(
        'title' => t('Office of the Associate Attorney General'),
      ),
      'ag' => array(
        'title' => t('Office of the Attorney General'),
      ),
      'core' => array(
        'title' => t('Core Entities'),
      ),
      'careers' => array(
        'title' => t('Careers'),
      ),
      'crs' => array(
        'title' => t('CRS - Community Relations Service'),
      ),
      'dag' => array(
        'title' => t('DAG - Office of the Deputy Attorney General'),
      ),
      'doj' => array(
        'title' => t('DOJ - Department of Justice'),
      ),
      'ej' => array(
        'title' => t('EJ - Environmental Justice'),
      ),
      'espanol' => array(
        'title' => t('espanol - Justice.gov en Espanol'),
      ),
      'fcsc' => array(
        'title' => t('FCSC - Foreign Claims Settlement Commission'),
      ),
      'interpol' => array(
        'title' => t('INTERPOL'),
      ),
      'jmd' => array(
        'title' => t('JMD - Justice Management Division'),
      ),
      'ncfs' => array(
        'title' => t('NCFS - National Commission on Forensic Science'),
      ),
      'nsd' => array(
        'title' => t('NSD - National Security Division'),
      ),
      'oarm' => array(
        'title' => t('OARM - Office of Attorney Recruitment & Management'),
      ),
      'oip' => array(
        'title' => t('OIP - Office of Information Policy'),
      ),
      'olc' => array(
        'title' => t('OLC - Office of Legal Counsel'),
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
      'open' => array(
        'title' => t('Open - Open Government'),
      ),
      'otj' => array(
        'title' => t('OTJ - Office of Tribal Justice'),
      ),
      'osdbu' => array(
        'title' => t('OSDBU - Office of Small and Disadvantaged Business Utilization'),
      ),
      'ovw' => array(
        'title' => t('OVW - Office on Violence Against Women'),
      ),
      'prao' => array(
        'title' => t('PRAO - Professional Responsibility Advisory Office'),
      ),
      'psc' => array(
        'title' => t('PSC - Project Safe Childhood'),
      ),
      'tribal' => array(
        'title' => t('Tribal Justice and Safety'),
      ),
      'usao' => array(
        'title' => t('USAO - United States Attorneys Office'),
      ),
    ),
    'migrations' => array(
      'AgPage' => array(
        'group_name' => 'ag',
        'class_name' => 'AgPageMigration',
      ),
      'AgFile' => array(
        'group_name' => 'ag',
        'class_name' => 'AgFileMigration',
      ),
      'AgSpeech' => array(
        'group_name' => 'ag',
        'class_name' => 'AgSpeechMigration',
      ),
      'AgSpeechSpanish' => array(
        'group_name' => 'ag',
        'class_name' => 'AgSpeechSpanishMigration',
      ),
      'AsgPage' => array(
        'group_name' => 'asg',
        'class_name' => 'AsgPageMigration',
      ),
      'AsgFile' => array(
        'group_name' => 'asg',
        'class_name' => 'AsgFileMigration',
      ),
      'AsgSpeech' => array(
        'group_name' => 'asg',
        'class_name' => 'AsgSpeechMigration',
      ),
      'AsgSpeechEspanol' => array(
        'group_name' => 'asg',
        'class_name' => 'AsgSpeechEspanolMigration',
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
      'CrsFile' => array(
        'group_name' => 'crs',
        'class_name' => 'CrsFileMigration',
      ),
      'CrsPage' => array(
        'group_name' => 'crs',
        'class_name' => 'CrsPageMigration',
      ),
      'DagPage' => array(
        'group_name' => 'dag',
        'class_name' => 'DagPageMigration',
      ),
      'DagFile' => array(
        'group_name' => 'dag',
        'class_name' => 'DagFileMigration',
      ),
      'DojFile' => array(
        'group_name' => 'doj',
        'class_name' => 'DojFileMigration',
      ),
      'DojPage' => array(
        'group_name' => 'doj',
        'class_name' => 'DojPageMigration',
      ),
      'DojCase' => array(
        'group_name' => 'doj',
        'class_name' => 'DojCaseMigration',
      ),
      'EjFile' => array(
        'group_name' => 'ej',
        'class_name' => 'EjFileMigration',
      ),
      'EjPage' => array(
        'group_name' => 'ej',
        'class_name' => 'EjPageMigration',
      ),
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
      'FcscPage' => array(
        'group_name' => 'fcsc',
        'class_name' => 'FcscPageMigration',
      ),
      'FcscFile' => array(
        'group_name' => 'fcsc',
        'class_name' => 'FcscFileMigration',
      ),
      'Gallery' => array(
        'group_name' => 'core',
        'class_name' => 'GalleryMigration',
      ),
      'GalleryImages' => array(
        'group_name' => 'core',
        'class_name' => 'GalleryImagesMigration',
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
      'JmdPage' => array(
        'group_name' => 'jmd',
        'class_name' => 'JmdPageMigration',
      ),
      'JmdFile' => array(
        'group_name' => 'jmd',
        'class_name' => 'JmdFileMigration',
      ),
      'NcfsPage' => array(
        'group_name' => 'ncfs',
        'class_name' => 'NcfsPageMigration',
      ),
      'NcfsFile' => array(
        'group_name' => 'ncfs',
        'class_name' => 'NcfsFileMigration',
      ),
      'NsdPage' => array(
        'group_name' => 'nsd',
        'class_name' => 'NsdPageMigration',
      ),
      'NsdOvtPage' => array(
        'group_name' => 'nsd',
        'class_name' => 'NsdOvtPageMigration',
      ),
      'NsdNewemployeePage' => array(
        'group_name' => 'nsd',
        'class_name' => 'NsdNewemployeePageMigration',
      ),
      'NsdFile' => array(
        'group_name' => 'nsd',
        'class_name' => 'NsdFileMigration',
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
      'OipFoiaPost' => array(
        'group_name' => 'oip',
        'class_name' => 'OipFoiaPostMigration',
      ),
      'OipFoiaPostAuthor' => array(
        'group_name' => 'oip',
        'class_name' => 'OipFoiaPostAuthorMigration',
      ),
      'OipFoiaPostCategory' => array(
        'group_name' => 'oip',
        'class_name' => 'OipFoiaPostCategoryMigration',
      ),
      'OipFoiaUpdateBlogEntry' => array(
        'group_name' => 'oip',
        'class_name' => 'OipFoiaUpdateBlogEntryMigration',
      ),
      'OipCourtDecision' => array(
        'group_name' => 'oip',
        'class_name' => 'OipCourtMigration',
      ),
      'OipCourtDecisionAuthor' => array(
        'group_name' => 'oip',
        'class_name' => 'OipCourtAuthorMigration',
      ),
      'OipCourtDecisionCategory' => array(
        'group_name' => 'oip',
        'class_name' => 'OipCourtCategoryMigration',
      ),
      'OipFoiaPostBlogEntry' => array(
        'group_name' => 'oip',
        'class_name' => 'OipFoiaPostBlogEntryMigration',
      ),
      'OipPage' => array(
        'group_name' => 'oip',
        'class_name' => 'OipPageMigration',
      ),
      'OipFile' => array(
        'group_name' => 'oip',
        'class_name' => 'OipFileMigration',
      ),
      'OlcPage' => array(
        'group_name' => 'olc',
        'class_name' => 'OlcPageMigration',
      ),
      'OlcOpinion' => array(
        'group_name' => 'olc',
        'class_name' => 'OlcOpinionMigration',
      ),
      'OlcFile' => array(
        'group_name' => 'olc',
        'class_name' => 'OlcFileMigration',
      ),
      'OlpFile' => array(
        'group_name' => 'olp',
        'class_name' => 'OlpFileMigration',
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
      'OpaPressRelease' => array(
        'group_name' => 'opa',
        'class_name' => 'OpaPressReleaseMigration',
      ),
      'OpaSpeech' => array(
        'group_name' => 'opa',
        'class_name' => 'OpaSpeechMigration',
      ),
      'OpaEvent' => array(
        'group_name' => 'opa',
        'class_name' => 'OpaEventMigration',
      ),
      'OpaPage' => array(
        'group_name' => 'opa',
        'class_name' => 'OpaPageMigration',
      ),
      'OpaFile' => array(
        'group_name' => 'opa',
        'class_name' => 'OpaFileMigration',
      ),
      'OpclPage' => array(
        'group_name' => 'opcl',
        'class_name' => 'OpclPageMigration',
      ),
      'OpclFile' => array(
        'group_name' => 'opcl',
        'class_name' => 'OpclFileMigration',
      ),
      'OpenPage' => array(
        'group_name' => 'open',
        'class_name' => 'OpenPageMigration',
      ),
      'OpenFile' => array(
        'group_name' => 'open',
        'class_name' => 'OpenFileMigration',
      ),
      'OsdbuPage' => array(
        'group_name' => 'osdbu',
        'class_name' => 'OsdbuPageMigration',
      ),
      'OsdbuFile' => array(
        'group_name' => 'osdbu',
        'class_name' => 'OsdbuFileMigration',
      ),
      'OtjPage' => array(
        'group_name' => 'otj',
        'class_name' => 'OtjPageMigration',
      ),
      'OtjFile' => array(
        'group_name' => 'otj',
        'class_name' => 'OtjFileMigration',
      ),
      'OvwBlogEntry' => array(
        'group_name' => 'ovw',
        'class_name' => 'OvwBlogEntryMigration',
      ),
      'OvwAuthor' => array(
        'group_name' => 'ovw',
        'class_name' => 'OvwAuthorMigration',
      ),
      'OvwAttachment' => array(
        'group_name' => 'ovw',
        'class_name' => 'OvwAttachmentMigration',
      ),
      'OvwCategory' => array(
        'group_name' => 'ovw',
        'class_name' => 'OvwCategoryMigration',
      ),
      'OvwPage' => array(
        'group_name' => 'ovw',
        'class_name' => 'OvwPageMigration',
      ),
      'OvwFile' => array(
        'group_name' => 'ovw',
        'class_name' => 'OvwFileMigration',
      ),
      'PraoPage' => array(
        'group_name' => 'prao',
        'class_name' => 'PraoPageMigration',
      ),
      'PscPage' => array(
        'group_name' => 'psc',
        'class_name' => 'PscPageMigration',
      ),
      'PscFile' => array(
        'group_name' => 'psc',
        'class_name' => 'PscFileMigration',
      ),
      'TribalPage' => array(
        'group_name' => 'tribal',
        'class_name' => 'TribalPageMigration',
      ),
      'TribalFile' => array(
        'group_name' => 'tribal',
        'class_name' => 'TribalFileMigration',
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
