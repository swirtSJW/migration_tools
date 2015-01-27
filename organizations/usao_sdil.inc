<?php

/**
 * @file
 * Defines migration classes for Southern District of Illinois.
 */

/**
 * Migrates .html files from /usao-sdil to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdil
 */
class UsaoSdilPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ils/Programs/VWA',
      1 => 'usao/ils',
      2 => 'usao/ils/About',
      3 => 'usao/ils/Programs',
      4 => 'usao/ils/Programs/VWA/Vacation Max/Nicosia',
      5 => 'usao/ils/Programs/VWA/Vacation Max/Marrs',
      6 => 'usao/ils/Programs/VWA/Vacation Max/Mathews',
      7 => 'usao/ils/Programs/VWA/Vacation Max/Sullivan',
      8 => 'usao/ils/Programs/VWA/Vacation Max/Eddy-Aldava',
      9 => 'usao/ils/Programs/VWA/Vacation Max/Nosack',
      10 => 'usao/ils/Programs/VWA/AFD Medical Advisors/Scebba',
      11 => 'usao/ils/Programs/VWA/AFD Medical Advisors',
      12 => 'usao/ils/Programs/VWA/brochures',
      13 => 'usao/ils/Programs/VWA/National Solutions/Atorrasagasti',
      14 => 'usao/ils/Programs/VWA/National Solutions/Falkner',
      15 => 'usao/ils/Programs/VWA/National Solutions/Velazquez',
      16 => 'usao/ils/Programs/VWA/National Solutions/Garten',
      17 => 'usao/ils/Programs/VWA/National Solutions/Tzen',
      18 => 'usao/ils/Programs/VWA/National Solutions/Currey',
      19 => 'usao/ils/Programs/VWA/National Solutions/Brooke',
      20 => 'usao/ils/Programs/VWA/National Solutions/Picache',
      21 => 'usao/ils/Programs/VWA/National Solutions/Gonzalez',
      22 => 'usao/ils/Programs/VWA/National Solutions/Charles',
      23 => 'usao/ils/Programs/VWA/National Solutions/Garten, Christopher',
      24 => 'usao/ils/Programs/VWA/National Solutions/Jones',
      25 => 'usao/ils/Programs/VWA/National Solutions/Roberts',
      26 => 'usao/ils/Programs/VWA/Prime One Financial/Price',
      27 => 'usao/ils/Programs/VWA/Madison County Tax Liens/McLean',
      28 => 'usao/ils/Programs/VWA/Madison County Tax Liens/Bathon',
      29 => 'usao/ils/Programs/VWA/Madison County Tax Liens/Rochman',
      30 => 'usao/ils/Programs/VWA/Madison County Tax Liens/Vassen',
      31 => 'usao/ils/Programs/VWA/Timeshare Recovery Scheme',
      32 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Acevedo',
      33 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Kartman',
      34 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/O Loughlin',
      35 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Rafferty',
      36 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Lopicola',
      37 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Goyos',
      38 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Wagner',
      39 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Aldred',
      40 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Krapac',
      41 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Tibbs',
      42 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Cameron',
      43 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Zaric',
      44 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Derbaremdiker',
      45 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Robertson',
      46 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Dorman',
      47 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Rock',
      48 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Christian',
      49 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Schmier',
      50 => 'usao/ils/Programs/VWA/Premier Timeshare Solutions/Miller',
      51 => 'usao/ils/Programs/VWA/CVS and UMS/Brazel, Morris, Folan',
      52 => 'usao/ils/Programs/VWA/CVS and UMS/Johnson and Todd',
      53 => 'usao/ils/Programs/VWA/CVS and UMS/Gilkey',
      54 => 'usao/ils/Programs/VWA/CVS and UMS/Pomerantz',
      55 => 'usao/ils/Programs/VWA/CVS and UMS/Marquez',
      56 => 'usao/ils/Programs/VWA/CVS and UMS/Johnson',
      57 => 'usao/ils/Programs/VWA/CVS and UMS/Intravaia',
      58 => 'usao/ils/Programs/VWA/CVS and UMS/Ferrara',
      59 => 'usao/ils/Programs/VWA/CVS and UMS/Santiago',
      60 => 'usao/ils/Programs/VWA/CVS and UMS/Schneidau',
      61 => 'usao/ils/Programs/VWA/CVS and UMS/Martinangelo',
      62 => 'usao/ils/Programs/VWA/CVS and UMS/Foote',
      63 => 'usao/ils/Programs/VWA/CVS and UMS/Stone',
      64 => 'usao/ils/Programs/VWA/CVS and UMS/John Robert Eddy',
      65 => 'usao/ils/Programs/VWA/CVS and UMS/Schmucker',
      66 => 'usao/ils/Programs/VWA/CVS and UMS/Colon',
      67 => 'usao/ils/Programs/VWA/CVS and UMS/Grizzanti',
      68 => 'usao/ils/Programs/VWA/CVS and UMS/Quimby',
      69 => 'usao/ils/Programs/VWA/CVS and UMS/Holmes',
      70 => 'usao/ils/Programs/VWA/CVS and UMS/Thomas',
      71 => 'usao/ils/Programs/VWA/CVS and UMS/Gregg',
      72 => 'usao/ils/Programs/VWA/CVS and UMS/Brown',
      73 => 'usao/ils/Programs/VWA/CVS and UMS/Myers',
      74 => 'usao/ils/Programs/VWA/CVS and UMS/Egan',
      75 => 'usao/ils/Programs/VWA/CVS and UMS/Dye',
      76 => 'usao/ils/Programs/VWA/CVS and UMS/Kirk',
      77 => 'usao/ils/Programs/VWA/CVS and UMS/Fields',
      78 => 'usao/ils/Programs/VWA/American Marketing Group/Massimino',
      79 => 'usao/ils/Programs/VWA/American Marketing Group/White',
      80 => 'usao/ils/Contact',
      81 => 'usao/ils/Featured Resources',
      82 => 'usao/ils/Divisions',
    );

    $arguments['source_parser_class'] = "DistrictsSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Southern District of Illinois.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdil')
    ->sourceMigration('UsaoDistrict');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /usao-sdil.
 *
 * @package doj_migration
 * @subpackage usao-sdil
 */
class UsaoSdilFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/ils subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/ils',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-sdil/docs';

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
    return "usao-sdil";
  }
}

/**
 * Migrates press releases from /usao-sdil.
 *
 * @package doj_migration
 * @subpackage usao-sdil
 */
class UsaoSdilPressReleaseMigration extends DistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-sdil";
    $arguments['component_tid'] = 1826;

    $source_dirs = array(
      83 => 'usao/ils/News/2015',
      84 => 'usao/ils/News/2014/Nov',
      85 => 'usao/ils/News/2014/May',
      86 => 'usao/ils/News/2014/Apr',
      87 => 'usao/ils/News/2014/Jul',
      88 => 'usao/ils/News/2014/Dec',
      89 => 'usao/ils/News/2014/Jan',
      90 => 'usao/ils/News/2014/Jun',
      91 => 'usao/ils/News/2014/Mar',
      92 => 'usao/ils/News/2014/Feb',
      93 => 'usao/ils/News/2014/Oct',
      94 => 'usao/ils/News/2014/Aug',
      95 => 'usao/ils/News/2014/Sep',
      96 => 'usao/ils/News/2013/Nov',
      97 => 'usao/ils/News/2013/May',
      98 => 'usao/ils/News/2013/Apr',
      99 => 'usao/ils/News/2013/Dec',
      100 => 'usao/ils/News/2013/Jan',
      101 => 'usao/ils/News/2013/Jun',
      102 => 'usao/ils/News/2013/Mar',
      103 => 'usao/ils/News/2013/Feb',
      104 => 'usao/ils/News/2013/Oct',
      105 => 'usao/ils/News/2013/Aug',
      106 => 'usao/ils/News/2013/July',
      107 => 'usao/ils/News/2013/Sep',
    );
    parent::__construct($arguments, $source_dirs);
  }
}
