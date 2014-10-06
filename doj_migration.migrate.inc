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
    ),
  );

  $api = array(
    'api' => 2,
    'groups' => array(
      'asg' => array(
        'title' => t('Office of the Associate Attorney General'),
        'disable_hooks' => $disable_hooks,
      ),
      'ag' => array(
        'title' => t('Office of the Attorney General'),
        'disable_hooks' => $disable_hooks,
      ),
      'atj' => array(
        'title' => t('The Access to Justice Initiative'),
        'disable_hooks' => $disable_hooks,
      ),
      'core' => array(
        'title' => t('Core Entities'),
        'disable_hooks' => $disable_hooks,
      ),
      'careers' => array(
        'title' => t('Careers'),
        'disable_hooks' => $disable_hooks,
      ),
      'civil' => array(
        'title' => t('Civil Division'),
        'disable_hooks' => $disable_hooks,
      ),
      'crs' => array(
        'title' => t('CRS - Community Relations Service'),
        'disable_hooks' => $disable_hooks,
      ),
      'dag' => array(
        'title' => t('DAG - Office of the Deputy Attorney General'),
        'disable_hooks' => $disable_hooks,
      ),
      'defendingchildhood' => array(
        'title' => t('Defending Childhood'),
        'disable_hooks' => $disable_hooks,
      ),
      'doj' => array(
        'title' => t('DOJ - Department of Justice'),
        'disable_hooks' => $disable_hooks,
      ),
      'ej' => array(
        'title' => t('EJ - Environmental Justice'),
        'disable_hooks' => $disable_hooks,
      ),
      'espanol' => array(
        'title' => t('espanol - Justice.gov en Espanol'),
        'disable_hooks' => $disable_hooks,
      ),
      'fcsc' => array(
        'title' => t('FCSC - Foreign Claims Settlement Commission'),
        'disable_hooks' => $disable_hooks,
      ),
      'interpol' => array(
        'title' => t('INTERPOL'),
        'disable_hooks' => $disable_hooks,
      ),
      'jmd' => array(
        'title' => t('JMD - Justice Management Division'),
        'disable_hooks' => $disable_hooks,
      ),
      'ncfs' => array(
        'title' => t('NCFS - National Commission on Forensic Science'),
        'disable_hooks' => $disable_hooks,
      ),
      'nsd' => array(
        'title' => t('NSD - National Security Division'),
        'disable_hooks' => $disable_hooks,
      ),
      'oarm' => array(
        'title' => t('OARM - Office of Attorney Recruitment & Management'),
        'disable_hooks' => $disable_hooks,
      ),
      'oip' => array(
        'title' => t('OIP - Office of Information Policy'),
        'disable_hooks' => $disable_hooks,
      ),
      'ola' => array(
        'title' => t('OLA - Office of Legislative Affairs'),
      ),
      'olc' => array(
        'title' => t('OLC - Office of Legal Counsel'),
        'disable_hooks' => $disable_hooks,
      ),
      'olp' => array(
        'title' => t('OLP - Office of Legal Policy'),
        'disable_hooks' => $disable_hooks,
      ),
      'opa' => array(
        'title' => t('OPA - Office of Public Affairs'),
        'disable_hooks' => $disable_hooks,
      ),
      'opcl' => array(
        'title' => t('OPCL - Office of Privacy and Civil Liberties'),
        'disable_hooks' => $disable_hooks,
      ),
      'open' => array(
        'title' => t('Open - Open Government'),
        'disable_hooks' => $disable_hooks,
      ),
      'osg' => array(
        'title' => t('OSG - Office of the Solicitor General'),
        'disable_hooks' => $disable_hooks,
      ),
      'otj' => array(
        'title' => t('OTJ - Office of Tribal Justice'),
        'disable_hooks' => $disable_hooks,
      ),
      'osdbu' => array(
        'title' => t('OSDBU - Office of Small and Disadvantaged Business Utilization'),
        'disable_hooks' => $disable_hooks,
      ),
      'ovw' => array(
        'title' => t('OVW - Office on Violence Against Women'),
        'disable_hooks' => $disable_hooks,
      ),
      'prao' => array(
        'title' => t('PRAO - Professional Responsibility Advisory Office'),
        'disable_hooks' => $disable_hooks,
      ),
      'psc' => array(
        'title' => t('PSC - Project Safe Childhood'),
        'disable_hooks' => $disable_hooks,
      ),
      'tribal' => array(
        'title' => t('Tribal Justice and Safety'),
        'disable_hooks' => $disable_hooks,
      ),
      'usao' => array(
        'title' => t('USAO - United States Attorneys Office'),
        'disable_hooks' => $disable_hooks,
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
      'AtjPage' => array(
        'group_name' => 'atj',
        'class_name' => 'AtjPageMigration',
      ),
      'AtjFile' => array(
        'group_name' => 'atj',
        'class_name' => 'AtjFileMigration',
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
      'CivilCase' => array(
        'group_name' => 'civil',
        'class_name' => 'CivilCaseMigration',
      ),
      'CivilFile' => array(
        'group_name' => 'civil',
        'class_name' => 'CivilFileMigration',
      ),
      'CivilPage' => array(
        'group_name' => 'civil',
        'class_name' => 'CivilPageMigration',
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
      'DefendingChildhoodPage' => array(
        'group_name' => 'defendingchildhood',
        'class_name' => 'DefendingChildhoodPageMigration',
      ),
      'DefendingChildhoodFile' => array(
        'group_name' => 'defendingchildhood',
        'class_name' => 'DefendingChildhoodFileMigration',
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
      'OlaFile' => array(
        'group_name' => 'ola',
        'class_name' => 'OlaFileMigration',
      ),
      'OlaPage' => array(
        'group_name' => 'ola',
        'class_name' => 'OlaPageMigration',
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
      'OsgBrief' => array(
        'group_name' => 'osg',
        'class_name' => 'OsgBriefMigration',
      ),
      'OsgPage' => array(
        'group_name' => 'osg',
        'class_name' => 'OsgPageMigration',
      ),
      'OsgFile' => array(
        'group_name' => 'osg',
        'class_name' => 'OsgFileMigration',
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
      'DojResourceIsoFormDb' => array(
        'group_name' => 'doj',
        'class_name' => 'DojResourceIsoFormMigration',
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
