<?php
/**
 * @file
 * Migrations.
 */

return array(
  'api' => 2,
  'groups' => array(
    'core' => array(
      'title' => 'Core Entities',
    ),
    'doj' => array(
      'title' => 'DOJ - Department of Justice',
    ),
    'eousa' => array(
      'title' => 'EOUSA - Executive Office for United States Attorneys',
    ),
    'espanol' => array(
      'title' => 'espanol - Justice.gov en Espanol',
    ),
    'ola' => array(
      'title' => 'OLA - Office of Legislative Affairs',
    ),
    'usao-az' => array(
      'title' => 'District of Arizona',
    ),
    'usao' => array(
      'title' => 'USAO - United States Attorneys Office',
    ),
    'usao-espanol' => array(
      'title' => 'USAO Espanol - Fiscales Federales',
    ),
    'usao-edla' => array(
      'title' => 'Eastern District of Louisiana',
    ),
    'usao-edpa' => array(
      'title' => 'Eastern District of Pennsylvania',
    ),
    'usao-gu' => array(
      'title' => 'District of Guam & Northern Mariana Islands',
    ),
    'usao-id' => array(
      'title' => 'District of Idaho',
    ),
    'usao-ks' => array(
      'title' => 'District of Kansas',
    ),
    'usao-mt' => array(
      'title' => 'District of Montana',
    ),
    'usao-ndca' => array(
      'title' => 'Northern District of California',
    ),
    'usao-ndwv' => array(
      'title' => 'Northern District of West Virginia',
    ),
    'usao-wdny' => array(
      'title' => 'Western District of New York',
    ),
    'usao-wdok' => array(
      'title' => 'Western District of Oklahoma',
    ),
    'usao-wdtx' => array(
      'title' => 'Western District of Texas',
    ),
    'usao-wdky' => array(
      'title' => 'Western District of Kentucky',
    ),
    'usao-wdmo' => array(
      'title' => 'Western District of Missouri',
    ),
    'usao-sdwv' => array(
      'title' => 'Southern District of West Virginia',
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
    'UsaoEspanolFile' => array(
      'group_name' => 'usao-espanol',
      'class_name' => 'UsaoEspanolFileMigration',
    ),
    'UsaoEspanolPage' => array(
      'group_name' => 'usao-espanol',
      'class_name' => 'UsaoEspanolPageMigration',
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
    'UsaoEdpaFile' => array(
      'group_name' => 'usao-edpa',
      'class_name' => 'UsaoEdpaFileMigration',
    ),
    'UsaoEdpaPage' => array(
      'group_name' => 'usao-edpa',
      'class_name' => 'UsaoEdpaPageMigration',
    ),
    'UsaoEdpaPressRelease' => array(
      'group_name' => 'usao-edpa',
      'class_name' => 'UsaoEdpaPressReleaseMigration',
    ),
    'UsaoGuFile' => array(
      'group_name' => 'usao-gu',
      'class_name' => 'UsaoGuFileMigration',
    ),
    'UsaoGuPage' => array(
      'group_name' => 'usao-gu',
      'class_name' => 'UsaoGuPageMigration',
    ),
    'UsaoGuPressRelease' => array(
      'group_name' => 'usao-gu',
      'class_name' => 'UsaoGuPressReleaseMigration',
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
    'UsaoKsFile' => array(
      'group_name' => 'usao-ks',
      'class_name' => 'UsaoKsFileMigration',
    ),
    'UsaoKsPage' => array(
      'group_name' => 'usao-ks',
      'class_name' => 'UsaoKsPageMigration',
    ),
    'UsaoKsPressRelease' => array(
      'group_name' => 'usao-ks',
      'class_name' => 'UsaoKsPressReleaseMigration',
    ),
    'UsaoMtFile' => array(
      'group_name' => 'usao-mt',
      'class_name' => 'UsaoMtFileMigration',
    ),
    'UsaoMtPage' => array(
      'group_name' => 'usao-mt',
      'class_name' => 'UsaoMtPageMigration',
    ),
    'UsaoMtPressRelease' => array(
      'group_name' => 'usao-mt',
      'class_name' => 'UsaoMtPressReleaseMigration',
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
    'UsaoNdwvFile' => array(
      'group_name' => 'usao-ndwv',
      'class_name' => 'UsaoNdwvFileMigration',
    ),
    'UsaoNdwvPage' => array(
      'group_name' => 'usao-ndwv',
      'class_name' => 'UsaoNdwvPageMigration',
    ),
    'UsaoNdwvPressRelease' => array(
      'group_name' => 'usao-ndwv',
      'class_name' => 'UsaoNdwvPressReleaseMigration',
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
    'UsaoWdkyFile' => array(
      'group_name' => 'usao-wdky',
      'class_name' => 'UsaoWdkyFileMigration',
    ),
    'UsaoWdkyPage' => array(
      'group_name' => 'usao-wdky',
      'class_name' => 'UsaoWdkyPageMigration',
    ),
    'UsaoWdkyPressRelease' => array(
      'group_name' => 'usao-wdky',
      'class_name' => 'UsaoWdkyPressReleaseMigration',
    ),
    'UsaoWdmoFile' => array(
      'group_name' => 'usao-wdmo',
      'class_name' => 'UsaoWdmoFileMigration',
    ),
    'UsaoWdmoPage' => array(
      'group_name' => 'usao-wdmo',
      'class_name' => 'UsaoWdmoPageMigration',
    ),
    'UsaoWdmoPressRelease' => array(
      'group_name' => 'usao-wdmo',
      'class_name' => 'UsaoWdmoPressReleaseMigration',
    ),
    'UsaoSdwvFile' => array(
      'group_name' => 'usao-sdwv',
      'class_name' => 'UsaoSdwvFileMigration',
    ),
    'UsaoSdwvPage' => array(
      'group_name' => 'usao-sdwv',
      'class_name' => 'UsaoSdwvPageMigration',
    ),
    'UsaoSdwvPressRelease' => array(
      'group_name' => 'usao-sdwv',
      'class_name' => 'UsaoSdwvPressReleaseMigration',
    ),
  ),
);
