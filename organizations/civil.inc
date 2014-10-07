<?php

/**
 * @file
 * Civil Division civil.
 *
 * Defines migration classes for the civil section of justice.gov.
 */

/**
 * Migrates .html files from /civil to page nodes.
 *
 * @package doj_migration
 * @subpackage civil
 */
class CivilPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $json_dir = '["civil\/ocpl","civil\/ocpl\/cases\/Closed","civil\/ocpl\/cases\/Open","civil\/common\/elecread\/2004","civil\/common\/elecread\/2003","civil\/cases\/tobacco2","civil\/cases\/cobell","civil\/cases\/cobell\/docs","civil\/cases\/cobell\/docs\/pdf","civil\/docs_forms","civil\/cpb\/monograph","civil\/cpb\/ocpl\/monograph - Copy","civil\/cpb\/ocpl","civil\/cpb\/ocpl\/odometer - Copy","civil\/cpb\/cases","civil\/cpb\/cases\/cases\/Mayne","civil\/cpb\/cases\/cases\/!Misc","civil\/cpb\/cases\/cases","civil\/cpb\/cases\/cases\/JusticeForAllAct2004","civil\/cpb\/cases\/cases\/Moncada","civil\/cpb\/cases\/cases\/Ghaziaskar","civil\/cpb\/cases\/cases\/ParaTech","civil\/cpb\/cases\/Open","civil\/cpb\/cases_old\/satterfield","civil\/cpb\/cases_old","civil\/cpb\/cases_old\/elliott","civil\/cpb\/cases_old\/001_Open-Case-Index","civil\/cpb\/cases_old\/Justice For All Act -2004","civil\/cpb\/cases_old\/test","civil\/cpb\/cases_old\/Bond_Illya","civil\/cpb\/cases_old\/stewart","civil\/cpb\/cases_old\/murkey","civil\/cpb\/cases_old\/Melton","civil\/cpb\/cases_old\/Clyburn","civil\/cpb\/cases_old\/AmeriPOS","civil\/cpb\/cases_old\/Sutton_Gary","civil\/cpb\/cases_old\/Kirkpatrick_Charles","civil\/cpb\/cases_old\/nationwide","civil\/cpb\/cases_old\/ghaziaskar","civil\/cpb\/cases_old\/Lilly","civil\/cpb\/cases_old\/norton","civil\/cpb\/cases_old\/schecter","civil\/cpb\/cases_old\/183-73-13","civil\/cpb\/cases_old\/AED","civil\/cpb\/cases_old\/Jardin","civil\/cpb\/cases_old\/002_Closed-Index-CASES","civil\/cpb\/cases_old\/002_Closed-Index-CASES\/stewart","civil\/cpb\/cases_old\/002_Closed-Index-CASES\/nationwide","civil\/cpb\/cases_old\/002_Closed-Index-CASES\/norton","civil\/cpb\/cases_old\/002_Closed-Index-CASES\/schecter","civil\/cpb\/cases_old\/002_Closed-Index-CASES\/vaughan","civil\/cpb\/cases_old\/Success Vending","civil\/cpb\/cases_old\/schaefer","civil\/cpb\/cases_old\/seligh","civil\/cpb\/cases_old\/vaughan","civil\/cpb\/cases_old\/purrington","civil\/cpb\/odometer","civil\/torts\/reca","civil\/torts\/const","civil\/torts\/const\/reca","civil","civil\/ocl","civil\/oil","civil\/BusSchedule","civil\/ocpl\/cases\/cases\/Fidelity_ATM","civil\/ocpl\/cases\/cases\/AmericanVending","civil\/ocpl\/cases\/cases\/Putnam","civil\/orientation","civil\/www.justice.gov\/opcl","civil\/appellate","civil\/common","civil\/common\/press","civil\/common\/oija","civil\/commercial","civil\/commercial\/oija","civil\/commercial\/financial","civil\/commercial\/fraud","civil\/commercial\/intellectual","civil\/commercial\/national-courts","civil\/commercial\/foreign","civil\/omp","civil\/cpb","civil\/cpb\/cases\/cases\/Sutton","civil\/cpb\/cases\/cases\/Almanza","civil\/cpb\/cases\/cases\/Carrano","civil\/cpb\/cases\/cases\/Menake","civil\/cpb\/cases\/cases\/Tel2Net","civil\/cpb\/cases\/cases\/Aronowitz","civil\/cpb\/cases\/cases\/Gokhman","civil\/cpb\/cases\/cases\/Success","civil\/cpb\/cases\/cases\/Merck","civil\/cpb\/cases\/cases\/Satterfield","civil\/cpb\/cases\/cases\/Eli_Lilly","civil\/cpb\/cases\/cases\/Gold_Star_Vending","civil\/cpb\/cases\/cases\/Cephalon","civil\/cpb\/cases\/cases\/Elliott","civil\/cpb\/cases\/cases\/Nationwide","civil\/cpb\/cases\/cases\/BabyTenda","civil\/cpb\/cases\/cases\/CashLink","civil\/cpb\/cases\/cases\/Fidelity_ATM","civil\/cpb\/cases\/cases\/CheckMate","civil\/cpb\/cases\/cases\/guidant","civil\/cpb\/cases\/cases\/Saleet","civil\/cpb\/cases\/cases\/Creative_Concepts","civil\/cpb\/cases\/cases\/Cole","civil\/cpb\/cases\/cases\/Guidry","civil\/cpb\/cases\/cases\/AmericanVending","civil\/cpb\/cases\/cases\/Eisenberg","civil\/cpb\/cases\/cases\/Melton","civil\/cpb\/cases\/cases\/ForestPharm","civil\/cpb\/cases\/cases\/Crofut","civil\/cpb\/cases\/cases\/AmericanMerchant","civil\/cpb\/cases\/cases\/GlobalResources2","civil\/cpb\/cases\/cases\/Interstate","civil\/cpb\/cases\/cases\/Clyburn","civil\/cpb\/cases\/cases\/Schaefer","civil\/cpb\/cases\/cases\/Luzula","civil\/cpb\/cases\/cases\/AmeriPOS","civil\/cpb\/cases\/cases\/ORourke","civil\/cpb\/cases\/cases\/Seligh","civil\/cpb\/cases\/cases\/VendStar","civil\/cpb\/cases\/cases\/Hall","civil\/cpb\/cases\/cases\/PayNGo","civil\/cpb\/cases\/cases\/Farr","civil\/cpb\/cases\/cases\/GlobalBrands","civil\/cpb\/cases\/cases\/Bradbury","civil\/cpb\/cases\/cases\/Younis","civil\/cpb\/cases\/cases\/Lilly","civil\/cpb\/cases\/cases\/SecurePayment_Services","civil\/cpb\/cases\/cases\/GlobalResources","civil\/cpb\/cases\/cases\/Vaughan","civil\/cpb\/cases\/cases\/Debit_Corp","civil\/cpb\/cases\/cases\/Marimo","civil\/cpb\/cases\/cases\/183-73-13","civil\/cpb\/cases\/cases\/Schecter","civil\/cpb\/cases\/cases\/Strawbridge","civil\/cpb\/cases\/cases\/PearsonSchultz","civil\/cpb\/cases\/cases\/Griffin","civil\/cpb\/cases\/cases\/Purrington","civil\/cpb\/cases\/cases\/AED","civil\/cpb\/cases\/cases\/Jardin","civil\/cpb\/cases\/cases\/Norton","civil\/cpb\/cases\/cases\/Stevens","civil\/cpb\/cases\/cases\/Murkey","civil\/cpb\/cases\/cases\/Pantheon","civil\/cpb\/cases\/cases\/Rubin","civil\/cpb\/cases\/cases\/MasterVend","civil\/cpb\/cases\/cases\/Schichtman","civil\/cpb\/cases\/cases\/Stewart","civil\/cpb\/cases\/cases\/Kirkpatrick","civil\/cpb\/cases\/cases\/UnitedNuclear","civil\/cpb\/cases\/cases\/Putnam","civil\/cpb\/cases\/cases\/Mathauda_Sirtaj","civil\/cpb\/cases\/cases\/Gladstone","civil\/cpb\/cases\/cases\/Stevens_Ristick","civil\/cpb\/cases\/cases\/Lee","civil\/cpb\/cases\/cases\/FerraraMichael","civil\/cpb\/cases\/cases\/Bond","civil\/cpb\/cases\/cases\/Croteau","civil\/cpb\/cases\/cases\/TransNetWireless","civil\/cpb\/cases\/cases\/McConnell","civil\/cpb\/cases\/cases\/VendThree","civil\/cpb\/cases\/cases\/Vendthree","civil\/cpb\/cases\/cases\/FerraraRobert","civil\/cpb\/cases\/cases\/Tamayo","civil\/cpb\/Template pages tier 1","civil\/cpb\/Template pages tier 1\/Template pages tier 2\/Template pages tier 3","civil\/cpb\/Template pages tier 1\/Template pages tier 2\/Template pages tier 3\/Template pages tier 4","civil\/cpb\/Template pages tier 1\/Template pages tier 2","civil\/cpb\/OCL","civil\/css","civil\/torts\/cstls","civil\/torts\/aa","civil\/torts","civil\/torts\/etls","civil\/torts\/ftcals","civil\/employment","civil\/fedprog","civil\/oil\/dcs","civil\/oil\/as","civil\/pr"]';
    // Define source directories.
    $source_dirs = drupal_json_decode($json_dir);

    $options = array('recurse' => FALSE);

    $arguments['source_parser_class'] = "CivilPageSourceParser";
    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Civil Division section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('civil')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (substr_count($row->fileid, "boxchart") > 0) {
      return FALSE;
    }

    $bool = parent::prepareRow($row);
    if (isset($bool) && $bool == FALSE) {
      return $bool;
    }

    if (substr_count($row->title, "Untitled") > 0) {
      return FALSE;
    }
  }
}

/**
 * Migrates .html files from /civil to page nodes.
 *
 * @package doj_migration
 * @subpackage civil
 */
class CivilCaseMigration extends JusticeHtmlToCaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'civil/cpb',
    );
    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates cases from the Civil Division section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('civil')->sourceMigration('Organization');
  }

}

/**
 * Migrates files (*.pdf) from /civil.
 *
 * @package doj_migration
 * @subpackage civil
 */
class CivilFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the civil subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'civil',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///olp/docs';

    $options = array('recurse' => TRUE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "civil";
  }
}
