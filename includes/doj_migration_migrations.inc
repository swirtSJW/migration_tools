<?php
/**
 * @file
 * Migrations.
 */

// @codingStandardsIgnoreStart
return array (
  'api' => 2,
  'groups' => 
  array (
    'core' => 
    array (
      'title' => 'Core Entities',
    ),
    'doj' => 
    array (
      'title' => 'DOJ - Department of Justice',
    ),
    'eousa' => 
    array (
      'title' => 'EOUSA - Executive Office for United States Attorneys',
    ),
    'espanol' => 
    array (
      'title' => 'espanol - Justice.gov en Espanol',
    ),
    'nsd-ovt' => 
    array (
      'title' => 'NSD OVT - National Security Division Office of Justice for Victims of Overseas Terrorism',
    ),
    'ola' => 
    array (
      'title' => 'OLA - Office of Legislative Affairs',
    ),
    'usao-az' => 
    array (
      'title' => 'District of Arizona',
    ),
    'usao' => 
    array (
      'title' => 'USAO - United States Attorneys Office',
    ),
    'usao-espanol' => 
    array (
      'title' => 'USAO Espanol - Fiscales Federales',
    ),
    'usao-edla' => 
    array (
      'title' => 'Eastern District of Louisiana',
    ),
    'usao-edpa' => 
    array (
      'title' => 'Eastern District of Pennsylvania',
    ),
    'usao-gu' => 
    array (
      'title' => 'District of Guam & Northern Mariana Islands',
    ),
    'usao-id' => 
    array (
      'title' => 'District of Idaho',
    ),
    'usao-ks' => 
    array (
      'title' => 'District of Kansas',
    ),
    'usao-mt' => 
    array (
      'title' => 'District of Montana',
    ),
    'usao-ndca' => 
    array (
      'title' => 'Northern District of California',
    ),
    'usao-ndwv' => 
    array (
      'title' => 'Northern District of West Virginia',
    ),
    'usao-wdny' => 
    array (
      'title' => 'Western District of New York',
    ),
    'usao-wdok' => 
    array (
      'title' => 'Western District of Oklahoma',
    ),
    'usao-wdtx' => 
    array (
      'title' => 'Western District of Texas',
    ),
    'usao-wdky' => 
    array (
      'title' => 'Western District of Kentucky',
    ),
    'usao-wdmo' => 
    array (
      'title' => 'Western District of Missouri',
    ),
    'usao-sdwv' => 
    array (
      'title' => 'Southern District of West Virginia',
    ),
    'usao-ma' => 
    array (
      'title' => 'District of Massachusetts',
    ),
    'usao-mdla' => 
    array (
      'title' => 'Middle District of Louisiana',
    ),
    'usao-sdms' => 
    array (
      'title' => 'Southern District of Mississippi',
    ),
    'pardon' => 
    array (
      'title' => 'Office of the Pardon',
    ),
    'uspc' => 
    array (
      'title' => 'U.S. Parole Commission',
    ),
    'eoir' => 
    array (
      'title' => 'Executive Office for Immigration Review',
    ),
    'mii' => 
    array (
      'title' => 'Medical Integrity Institute',
    ),
    'usao-sdal' => 
    array (
      'title' => 'Southern District of Alabama',
    ),
    'usao-ndfl' => 
    array (
      'title' => 'Northern District of Florida',
    ),
    'usao-mdfl' => 
    array (
      'title' => 'Middle District of Florida',
    ),
    'usao-sdin' => 
    array (
      'title' => 'Southern District of Indiana',
    ),
    'usao-wdla' => 
    array (
      'title' => 'Western District of Louisiana',
    ),
    'usao-md' => 
    array (
      'title' => 'District of Maryland',
    ),
  ),
  'migrations' => 
  array (
    'EspanolFile' => 
    array (
      'group_name' => 'espanol',
      'class_name' => 'EspanolFileMigration',
    ),
    'EspanolPage' => 
    array (
      'group_name' => 'espanol',
      'class_name' => 'EspanolPageMigration',
    ),
    'EspanolPressRelease' => 
    array (
      'group_name' => 'espanol',
      'class_name' => 'EspanolPressReleaseMigration',
    ),
    'EspanolSpeech' => 
    array (
      'group_name' => 'espanol',
      'class_name' => 'EspanolSpeechMigration',
    ),
    'Organization' => 
    array (
      'group_name' => 'core',
      'class_name' => 'OrganizationMigration',
    ),
    'OlaFile' => 
    array (
      'group_name' => 'ola',
      'class_name' => 'OlaFileMigration',
    ),
    'OlaPage' => 
    array (
      'group_name' => 'ola',
      'class_name' => 'OlaPageMigration',
    ),
    'OlaTestimony' => 
    array (
      'group_name' => 'ola',
      'class_name' => 'OlaTestimonyMigration',
    ),
    'OlaWitness' => 
    array (
      'group_name' => 'ola',
      'class_name' => 'OlaWitnessMigration',
    ),
    'OlaDbTestimony' => 
    array (
      'group_name' => 'ola',
      'class_name' => 'OlaDbTestimonyMigration',
    ),
    'OlaDbWitness' => 
    array (
      'group_name' => 'ola',
      'class_name' => 'OlaDbWitnessMigration',
    ),
    'NsdOvtPage' => 
    array (
      'group_name' => 'nsd-ovt',
      'class_name' => 'NsdOvtPageMigration',
    ),
    'NsdOvtFile' => 
    array (
      'group_name' => 'nsd-ovt',
      'class_name' => 'NsdOvtFileMigration',
    ),
    'UsaoAzFile' => 
    array (
      'group_name' => 'usao-az',
      'class_name' => 'UsaoAzFileMigration',
    ),
    'UsaoAzPage' => 
    array (
      'group_name' => 'usao-az',
      'class_name' => 'UsaoAZPageMigration',
    ),
    'UsaoAzPressRelease' => 
    array (
      'group_name' => 'usao-az',
      'class_name' => 'UsaoAzPressReleaseMigration',
    ),
    'UsaoDistrict' => 
    array (
      'group_name' => 'usao',
      'class_name' => 'UsaoDistrictMigration',
    ),
    'UsaoStaffProfile' => 
    array (
      'group_name' => 'usao',
      'class_name' => 'UsaoStaffProfileMigration',
    ),
    'UsaoFile' => 
    array (
      'group_name' => 'usao',
      'class_name' => 'UsaoFileMigration',
    ),
    'UsaoPage' => 
    array (
      'group_name' => 'usao',
      'class_name' => 'UsaoPageMigration',
    ),
    'UsaoEspanolFile' => 
    array (
      'group_name' => 'usao-espanol',
      'class_name' => 'UsaoEspanolFileMigration',
    ),
    'UsaoEspanolPage' => 
    array (
      'group_name' => 'usao-espanol',
      'class_name' => 'UsaoEspanolPageMigration',
    ),
    'UsaoEdlaFile' => 
    array (
      'group_name' => 'usao-edla',
      'class_name' => 'UsaoEdlaFileMigration',
    ),
    'UsaoEdlaPage' => 
    array (
      'group_name' => 'usao-edla',
      'class_name' => 'UsaoEdlaPageMigration',
    ),
    'UsaoEdlaPressRelease' => 
    array (
      'group_name' => 'usao-edla',
      'class_name' => 'UsaoEdlaPressReleaseMigration',
    ),
    'UsaoEdpaFile' => 
    array (
      'group_name' => 'usao-edpa',
      'class_name' => 'UsaoEdpaFileMigration',
    ),
    'UsaoEdpaPage' => 
    array (
      'group_name' => 'usao-edpa',
      'class_name' => 'UsaoEdpaPageMigration',
    ),
    'UsaoEdpaPressRelease' => 
    array (
      'group_name' => 'usao-edpa',
      'class_name' => 'UsaoEdpaPressReleaseMigration',
    ),
    'UsaoGuFile' => 
    array (
      'group_name' => 'usao-gu',
      'class_name' => 'UsaoGuFileMigration',
    ),
    'UsaoGuPage' => 
    array (
      'group_name' => 'usao-gu',
      'class_name' => 'UsaoGuPageMigration',
    ),
    'UsaoGuPressRelease' => 
    array (
      'group_name' => 'usao-gu',
      'class_name' => 'UsaoGuPressReleaseMigration',
    ),
    'UsaoIdFile' => 
    array (
      'group_name' => 'usao-id',
      'class_name' => 'UsaoIdFileMigration',
    ),
    'UsaoIdPage' => 
    array (
      'group_name' => 'usao-id',
      'class_name' => 'UsaoIdPageMigration',
    ),
    'UsaoIdPressRelease' => 
    array (
      'group_name' => 'usao-id',
      'class_name' => 'UsaoIdPressReleaseMigration',
    ),
    'UsaoKsFile' => 
    array (
      'group_name' => 'usao-ks',
      'class_name' => 'UsaoKsFileMigration',
    ),
    'UsaoKsPage' => 
    array (
      'group_name' => 'usao-ks',
      'class_name' => 'UsaoKsPageMigration',
    ),
    'UsaoKsPressRelease' => 
    array (
      'group_name' => 'usao-ks',
      'class_name' => 'UsaoKsPressReleaseMigration',
    ),
    'UsaoMtFile' => 
    array (
      'group_name' => 'usao-mt',
      'class_name' => 'UsaoMtFileMigration',
    ),
    'UsaoMtPage' => 
    array (
      'group_name' => 'usao-mt',
      'class_name' => 'UsaoMtPageMigration',
    ),
    'UsaoMtPressRelease' => 
    array (
      'group_name' => 'usao-mt',
      'class_name' => 'UsaoMtPressReleaseMigration',
    ),
    'UsaoNdcaFile' => 
    array (
      'group_name' => 'usao-ndca',
      'class_name' => 'UsaoNdcaFileMigration',
    ),
    'UsaoNdcaPage' => 
    array (
      'group_name' => 'usao-ndca',
      'class_name' => 'UsaoNdcaPageMigration',
    ),
    'UsaoNdcaPressRelease' => 
    array (
      'group_name' => 'usao-ndca',
      'class_name' => 'UsaoNdcaPressReleaseMigration',
    ),
    'UsaoNdwvFile' => 
    array (
      'group_name' => 'usao-ndwv',
      'class_name' => 'UsaoNdwvFileMigration',
    ),
    'UsaoNdwvPage' => 
    array (
      'group_name' => 'usao-ndwv',
      'class_name' => 'UsaoNdwvPageMigration',
    ),
    'UsaoNdwvPressRelease' => 
    array (
      'group_name' => 'usao-ndwv',
      'class_name' => 'UsaoNdwvPressReleaseMigration',
    ),
    'UsaoWdnyFile' => 
    array (
      'group_name' => 'usao-wdny',
      'class_name' => 'UsaoWdnyFileMigration',
    ),
    'UsaoWdnyPage' => 
    array (
      'group_name' => 'usao-wdny',
      'class_name' => 'UsaoWdnyPageMigration',
    ),
    'UsaoWdnyPressRelease' => 
    array (
      'group_name' => 'usao-wdny',
      'class_name' => 'UsaoWdnyPressReleaseMigration',
    ),
    'UsaoWdokFile' => 
    array (
      'group_name' => 'usao-wdok',
      'class_name' => 'UsaoWdokFileMigration',
    ),
    'UsaoWdokPage' => 
    array (
      'group_name' => 'usao-wdok',
      'class_name' => 'UsaoWdokPageMigration',
    ),
    'UsaoWdokPressRelease' => 
    array (
      'group_name' => 'usao-wdok',
      'class_name' => 'UsaoWdokPressReleaseMigration',
    ),
    'UsaoWdtxFile' => 
    array (
      'group_name' => 'usao-wdtx',
      'class_name' => 'UsaoWdtxFileMigration',
    ),
    'UsaoWdtxPage' => 
    array (
      'group_name' => 'usao-wdtx',
      'class_name' => 'UsaoWdtxPageMigration',
    ),
    'UsaoWdtxPressRelease' => 
    array (
      'group_name' => 'usao-wdtx',
      'class_name' => 'UsaoWdtxPressReleaseMigration',
    ),
    'UsaoWdkyFile' => 
    array (
      'group_name' => 'usao-wdky',
      'class_name' => 'UsaoWdkyFileMigration',
    ),
    'UsaoWdkyPage' => 
    array (
      'group_name' => 'usao-wdky',
      'class_name' => 'UsaoWdkyPageMigration',
    ),
    'UsaoWdkyPressRelease' => 
    array (
      'group_name' => 'usao-wdky',
      'class_name' => 'UsaoWdkyPressReleaseMigration',
    ),
    'UsaoWdmoFile' => 
    array (
      'group_name' => 'usao-wdmo',
      'class_name' => 'UsaoWdmoFileMigration',
    ),
    'UsaoWdmoPage' => 
    array (
      'group_name' => 'usao-wdmo',
      'class_name' => 'UsaoWdmoPageMigration',
    ),
    'UsaoWdmoPressRelease' => 
    array (
      'group_name' => 'usao-wdmo',
      'class_name' => 'UsaoWdmoPressReleaseMigration',
    ),
    'UsaoSdwvFile' => 
    array (
      'group_name' => 'usao-sdwv',
      'class_name' => 'UsaoSdwvFileMigration',
    ),
    'UsaoSdwvPage' => 
    array (
      'group_name' => 'usao-sdwv',
      'class_name' => 'UsaoSdwvPageMigration',
    ),
    'UsaoSdwvPressRelease' => 
    array (
      'group_name' => 'usao-sdwv',
      'class_name' => 'UsaoSdwvPressReleaseMigration',
    ),
    'UsaoMaFile' => 
    array (
      'group_name' => 'usao-ma',
      'class_name' => 'UsaoMaFileMigration',
    ),
    'UsaoMaPage' => 
    array (
      'group_name' => 'usao-ma',
      'class_name' => 'UsaoMaPageMigration',
    ),
    'UsaoMaPressRelease' => 
    array (
      'group_name' => 'usao-ma',
      'class_name' => 'UsaoMaPressReleaseMigration',
    ),
    'UsaoMdlaFile' => 
    array (
      'group_name' => 'usao-mdla',
      'class_name' => 'UsaoMdlaFileMigration',
    ),
    'UsaoMdlaPage' => 
    array (
      'group_name' => 'usao-mdla',
      'class_name' => 'UsaoMdlaPageMigration',
    ),
    'UsaoMdlaPressRelease' => 
    array (
      'group_name' => 'usao-mdla',
      'class_name' => 'UsaoMdlaPressReleaseMigration',
    ),
    'UsaoSdmsFile' => 
    array (
      'group_name' => 'usao-sdms',
      'class_name' => 'UsaoSdmsFileMigration',
    ),
    'UsaoSdmsPage' => 
    array (
      'group_name' => 'usao-sdms',
      'class_name' => 'UsaoSdmsPageMigration',
    ),
    'UsaoSdmsPressRelease' => 
    array (
      'group_name' => 'usao-sdms',
      'class_name' => 'UsaoSdmsPressReleaseMigration',
    ),
    'PardonFile' => 
    array (
      'group_name' => 'pardon',
      'class_name' => 'PardonFileMigration',
    ),
    'PardonPage' => 
    array (
      'group_name' => 'pardon',
      'class_name' => 'PardonPageMigration',
    ),
    'UspcFile' => 
    array (
      'group_name' => 'uspc',
      'class_name' => 'UspcFileMigration',
    ),
    'UspcPage' => 
    array (
      'group_name' => 'uspc',
      'class_name' => 'UspcPageMigration',
    ),
    'EoirFile' => 
    array (
      'group_name' => 'eoir',
      'class_name' => 'EoirFileMigration',
    ),
    'EoirPage' => 
    array (
      'group_name' => 'eoir',
      'class_name' => 'EoirPageMigration',
    ),
    'MiiFile' => 
    array (
      'group_name' => 'mii',
      'class_name' => 'MiiFileMigration',
    ),
    'MiiPage' => 
    array (
      'group_name' => 'mii',
      'class_name' => 'MiiPageMigration',
    ),
    'UsaoSdalFile' => 
    array (
      'group_name' => 'usao-sdal',
      'class_name' => 'UsaoSdalFileMigration',
    ),
    'UsaoSdalPage' => 
    array (
      'group_name' => 'usao-sdal',
      'class_name' => 'UsaoSdalPageMigration',
    ),
    'UsaoSdalPressRelease' => 
    array (
      'group_name' => 'usao-sdal',
      'class_name' => 'UsaoSdalPressReleaseMigration',
    ),
    'UsaoNdflFile' => 
    array (
      'group_name' => 'usao-ndfl',
      'class_name' => 'UsaoNdflFileMigration',
    ),
    'UsaoNdflPage' => 
    array (
      'group_name' => 'usao-ndfl',
      'class_name' => 'UsaoNdflPageMigration',
    ),
    'UsaoNdflPressRelease' => 
    array (
      'group_name' => 'usao-ndfl',
      'class_name' => 'UsaoNdflPressReleaseMigration',
    ),
    'UsaoMdflFile' => 
    array (
      'group_name' => 'usao-mdfl',
      'class_name' => 'UsaoMdflFileMigration',
    ),
    'UsaoMdflPage' => 
    array (
      'group_name' => 'usao-mdfl',
      'class_name' => 'UsaoMdflPageMigration',
    ),
    'UsaoMdflPressRelease' => 
    array (
      'group_name' => 'usao-mdfl',
      'class_name' => 'UsaoMdflPressReleaseMigration',
    ),
    'UsaoSdinFile' => 
    array (
      'group_name' => 'usao-sdin',
      'class_name' => 'UsaoSdinFileMigration',
    ),
    'UsaoSdinPage' => 
    array (
      'group_name' => 'usao-sdin',
      'class_name' => 'UsaoSdinPageMigration',
    ),
    'UsaoWdlaFile' => 
    array (
      'group_name' => 'usao-wdla',
      'class_name' => 'UsaoWdlaFileMigration',
    ),
    'UsaoWdlaPage' => 
    array (
      'group_name' => 'usao-wdla',
      'class_name' => 'UsaoWdlaPageMigration',
    ),
    'UsaoWdlaPressRelease' => 
    array (
      'group_name' => 'usao-wdla',
      'class_name' => 'UsaoWdlaPressReleaseMigration',
    ),
    'UsaoMdFile' => 
    array (
      'group_name' => 'usao-md',
      'class_name' => 'UsaoMdFileMigration',
    ),
    'UsaoMdPage' => 
    array (
      'group_name' => 'usao-md',
      'class_name' => 'UsaoMdPageMigration',
    ),
    'UsaoMdPressRelease' => 
    array (
      'group_name' => 'usao-md',
      'class_name' => 'UsaoMdPressReleaseMigration',
    ),
  ),
);
// @codingStandardsIgnoreEnd
