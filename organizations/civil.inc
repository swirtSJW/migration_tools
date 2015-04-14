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
    $source_dirs = array(
      0 => 'civil/ocpl',
      1 => 'civil/ocpl/cases/Closed',
      2 => 'civil/ocpl/cases/Open',
      3 => 'civil/common/elecread/2004',
      4 => 'civil/common/elecread/2003',
      12 => 'civil/docs_forms',
      13 => 'civil/cpb/monograph',
      14 => 'civil/cpb/ocpl/monograph - Copy',
      15 => 'civil/cpb/ocpl',
      16 => 'civil/cpb/ocpl/odometer - Copy',
      59 => 'civil/cpb/odometer',
      60 => 'civil/torts/reca',
      61 => 'civil/torts/const',
      62 => 'civil/torts/const/reca',
      63 => 'civil',
      64 => 'civil/ocl',
      65 => 'civil/oil',
      66 => 'civil/BusSchedule',
      67 => 'civil/ocpl/cases/cases/Fidelity_ATM',
      68 => 'civil/ocpl/cases/cases/AmericanVending',
      69 => 'civil/ocpl/cases/cases/Putnam',
      70 => 'civil/orientation',
      71 => 'civil/www.justice.gov/opcl',
      72 => 'civil/appellate',
      73 => 'civil/common',
      74 => 'civil/common/press',
      75 => 'civil/common/oija',
      76 => 'civil/commercial',
      77 => 'civil/commercial/oija',
      78 => 'civil/commercial/financial',
      79 => 'civil/commercial/fraud',
      80 => 'civil/commercial/intellectual',
      81 => 'civil/commercial/national-courts',
      82 => 'civil/commercial/foreign',
      83 => 'civil/omp',
      84 => 'civil/cpb',
      168 => 'civil/cpb/Template pages tier 1',
      169 => 'civil/cpb/Template pages tier 1/Template pages tier 2/Template pages tier 3',
      170 => 'civil/cpb/Template pages tier 1/Template pages tier 2/Template pages tier 3/Template pages tier 4',
      171 => 'civil/cpb/Template pages tier 1/Template pages tier 2',
      172 => 'civil/cpb/OCL',
      173 => 'civil/css',
      174 => 'civil/torts/cstls',
      175 => 'civil/torts/aa',
      176 => 'civil/torts',
      177 => 'civil/torts/etls',
      178 => 'civil/torts/ftcals',
      179 => 'civil/employment',
      180 => 'civil/fedprog',
      181 => 'civil/oil/dcs',
      182 => 'civil/oil/as',
      183 => 'civil/pr',
    );

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
      5 => 'civil/cases/tobacco2',
      7 => 'civil/cases/cobell',
      8 => 'civil/cases/cobell/docs',
      9 => 'civil/cases/cobell/docs/pdf',
      17 => 'civil/cpb/cases',
      18 => 'civil/cpb/cases/cases/Mayne',
      19 => 'civil/cpb/cases/cases/!Misc',
      20 => 'civil/cpb/cases/cases',
      21 => 'civil/cpb/cases/cases/JusticeForAllAct2004',
      22 => 'civil/cpb/cases/cases/Moncada',
      23 => 'civil/cpb/cases/cases/Ghaziaskar',
      24 => 'civil/cpb/cases/cases/ParaTech',
      25 => 'civil/cpb/cases/Open',
      26 => 'civil/cpb/cases_old/satterfield',
      27 => 'civil/cpb/cases_old',
      28 => 'civil/cpb/cases_old/elliott',
      29 => 'civil/cpb/cases_old/001_Open-Case-Index',
      30 => 'civil/cpb/cases_old/Justice For All Act -2004',
      31 => 'civil/cpb/cases_old/test',
      32 => 'civil/cpb/cases_old/Bond_Illya',
      33 => 'civil/cpb/cases_old/stewart',
      34 => 'civil/cpb/cases_old/murkey',
      35 => 'civil/cpb/cases_old/Melton',
      36 => 'civil/cpb/cases_old/Clyburn',
      37 => 'civil/cpb/cases_old/AmeriPOS',
      38 => 'civil/cpb/cases_old/Sutton_Gary',
      39 => 'civil/cpb/cases_old/Kirkpatrick_Charles',
      40 => 'civil/cpb/cases_old/nationwide',
      41 => 'civil/cpb/cases_old/ghaziaskar',
      42 => 'civil/cpb/cases_old/Lilly',
      43 => 'civil/cpb/cases_old/norton',
      44 => 'civil/cpb/cases_old/schecter',
      45 => 'civil/cpb/cases_old/183-73-13',
      46 => 'civil/cpb/cases_old/AED',
      47 => 'civil/cpb/cases_old/Jardin',
      48 => 'civil/cpb/cases_old/002_Closed-Index-CASES',
      49 => 'civil/cpb/cases_old/002_Closed-Index-CASES/stewart',
      50 => 'civil/cpb/cases_old/002_Closed-Index-CASES/nationwide',
      51 => 'civil/cpb/cases_old/002_Closed-Index-CASES/norton',
      52 => 'civil/cpb/cases_old/002_Closed-Index-CASES/schecter',
      53 => 'civil/cpb/cases_old/002_Closed-Index-CASES/vaughan',
      54 => 'civil/cpb/cases_old/Success Vending',
      55 => 'civil/cpb/cases_old/schaefer',
      56 => 'civil/cpb/cases_old/seligh',
      57 => 'civil/cpb/cases_old/vaughan',
      58 => 'civil/cpb/cases_old/purrington',
      85 => 'civil/cpb/cases/cases/Sutton',
      86 => 'civil/cpb/cases/cases/Almanza',
      87 => 'civil/cpb/cases/cases/Carrano',
      88 => 'civil/cpb/cases/cases/Menake',
      89 => 'civil/cpb/cases/cases/Tel2Net',
      90 => 'civil/cpb/cases/cases/Aronowitz',
      91 => 'civil/cpb/cases/cases/Gokhman',
      92 => 'civil/cpb/cases/cases/Success',
      93 => 'civil/cpb/cases/cases/Merck',
      94 => 'civil/cpb/cases/cases/Satterfield',
      95 => 'civil/cpb/cases/cases/Eli_Lilly',
      96 => 'civil/cpb/cases/cases/Gold_Star_Vending',
      97 => 'civil/cpb/cases/cases/Cephalon',
      98 => 'civil/cpb/cases/cases/Elliott',
      99 => 'civil/cpb/cases/cases/Nationwide',
      100 => 'civil/cpb/cases/cases/BabyTenda',
      101 => 'civil/cpb/cases/cases/CashLink',
      102 => 'civil/cpb/cases/cases/Fidelity_ATM',
      103 => 'civil/cpb/cases/cases/CheckMate',
      104 => 'civil/cpb/cases/cases/guidant',
      105 => 'civil/cpb/cases/cases/Saleet',
      106 => 'civil/cpb/cases/cases/Creative_Concepts',
      107 => 'civil/cpb/cases/cases/Cole',
      108 => 'civil/cpb/cases/cases/Guidry',
      109 => 'civil/cpb/cases/cases/AmericanVending',
      110 => 'civil/cpb/cases/cases/Eisenberg',
      111 => 'civil/cpb/cases/cases/Melton',
      112 => 'civil/cpb/cases/cases/ForestPharm',
      113 => 'civil/cpb/cases/cases/Crofut',
      114 => 'civil/cpb/cases/cases/AmericanMerchant',
      115 => 'civil/cpb/cases/cases/GlobalResources2',
      116 => 'civil/cpb/cases/cases/Interstate',
      117 => 'civil/cpb/cases/cases/Clyburn',
      118 => 'civil/cpb/cases/cases/Schaefer',
      119 => 'civil/cpb/cases/cases/Luzula',
      120 => 'civil/cpb/cases/cases/AmeriPOS',
      121 => 'civil/cpb/cases/cases/ORourke',
      122 => 'civil/cpb/cases/cases/Seligh',
      123 => 'civil/cpb/cases/cases/VendStar',
      124 => 'civil/cpb/cases/cases/Hall',
      125 => 'civil/cpb/cases/cases/PayNGo',
      126 => 'civil/cpb/cases/cases/Farr',
      127 => 'civil/cpb/cases/cases/GlobalBrands',
      128 => 'civil/cpb/cases/cases/Bradbury',
      129 => 'civil/cpb/cases/cases/Younis',
      130 => 'civil/cpb/cases/cases/Lilly',
      131 => 'civil/cpb/cases/cases/SecurePayment_Services',
      132 => 'civil/cpb/cases/cases/GlobalResources',
      133 => 'civil/cpb/cases/cases/Vaughan',
      134 => 'civil/cpb/cases/cases/Debit_Corp',
      135 => 'civil/cpb/cases/cases/Marimo',
      136 => 'civil/cpb/cases/cases/183-73-13',
      137 => 'civil/cpb/cases/cases/Schecter',
      138 => 'civil/cpb/cases/cases/Strawbridge',
      139 => 'civil/cpb/cases/cases/PearsonSchultz',
      140 => 'civil/cpb/cases/cases/Griffin',
      141 => 'civil/cpb/cases/cases/Purrington',
      142 => 'civil/cpb/cases/cases/AED',
      143 => 'civil/cpb/cases/cases/Jardin',
      144 => 'civil/cpb/cases/cases/Norton',
      145 => 'civil/cpb/cases/cases/Stevens',
      146 => 'civil/cpb/cases/cases/Murkey',
      147 => 'civil/cpb/cases/cases/Pantheon',
      148 => 'civil/cpb/cases/cases/Rubin',
      149 => 'civil/cpb/cases/cases/MasterVend',
      150 => 'civil/cpb/cases/cases/Schichtman',
      151 => 'civil/cpb/cases/cases/Stewart',
      152 => 'civil/cpb/cases/cases/Kirkpatrick',
      153 => 'civil/cpb/cases/cases/UnitedNuclear',
      154 => 'civil/cpb/cases/cases/Putnam',
      155 => 'civil/cpb/cases/cases/Mathauda_Sirtaj',
      156 => 'civil/cpb/cases/cases/Gladstone',
      157 => 'civil/cpb/cases/cases/Stevens_Ristick',
      158 => 'civil/cpb/cases/cases/Lee',
      159 => 'civil/cpb/cases/cases/FerraraMichael',
      160 => 'civil/cpb/cases/cases/Bond',
      161 => 'civil/cpb/cases/cases/Croteau',
      162 => 'civil/cpb/cases/cases/TransNetWireless',
      163 => 'civil/cpb/cases/cases/McConnell',
      164 => 'civil/cpb/cases/cases/VendThree',
      165 => 'civil/cpb/cases/cases/Vendthree',
      166 => 'civil/cpb/cases/cases/FerraraRobert',
      167 => 'civil/cpb/cases/cases/Tamayo',
    );

    $options = array('recurse' => FALSE);
    $arguments['source_parser_class'] = "CivilCaseSourceParser";

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
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
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
