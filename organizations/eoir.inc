<?php

/**
 * @file
 * Defines migration classes for Executive Office for Immigration Review.
 */

/**
 * Migrates .html files from /eoir to page nodes.
 *
 * @package doj_migration
 * @subpackage eoir
 */
class EoirPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      0 => 'eoir/OcahoMain',
      1 => 'eoir/OcahoMain/publisheddecisions/Hardbound/Volume2',
      2 => 'eoir/OcahoMain/publisheddecisions/Hardbound/Volume1',
      3 => 'eoir/OcahoMain/publisheddecisions/Hardbound/Volume4',
      4 => 'eoir/OcahoMain/publisheddecisions/Hardbound/Volume6',
      5 => 'eoir/OcahoMain/publisheddecisions/Hardbound/Volume7',
      6 => 'eoir/OcahoMain/publisheddecisions/Hardbound/Volume3',
      7 => 'eoir/OcahoMain/publisheddecisions/Hardbound/Volume5',
      8 => 'eoir/OcahoMain/publisheddecisions/Hardbound/Volume8',
      9 => 'eoir/OcahoMain/publisheddecisions/Looseleaf/Volume9',
      10 => 'eoir/OcahoMain/publisheddecisions/Looseleaf/Volume11',
      11 => 'eoir/OcahoMain/publisheddecisions/Looseleaf/Volume10',
      12 => 'eoir/OcahoMain/anniversary',
      13 => 'eoir',
      14 => 'eoir/press',
      34 => 'eoir/profcond',
      35 => 'eoir/sibpages/pho',
      36 => 'eoir/sibpages/oma',
      37 => 'eoir/sibpages/bos',
      38 => 'eoir/sibpages/new',
      39 => 'eoir/sibpages/nol',
      40 => 'eoir/sibpages/tuc',
      41 => 'eoir/sibpages/adl',
      42 => 'eoir/sibpages/sna',
      43 => 'eoir/sibpages/tac',
      44 => 'eoir/sibpages/dal',
      45 => 'eoir/sibpages/sfr',
      46 => 'eoir/sibpages/den',
      47 => 'eoir/sibpages/snd',
      48 => 'eoir/sibpages/hlg',
      49 => 'eoir/sibpages/hou',
      50 => 'eoir/sibpages/chl',
      51 => 'eoir/sibpages/cle',
      52 => 'eoir/sibpages/uls',
      53 => 'eoir/sibpages',
      54 => 'eoir/sibpages/atl',
      55 => 'eoir/sibpages/chi',
      56 => 'eoir/sibpages/hod',
      57 => 'eoir/sibpages/yor',
      58 => 'eoir/sibpages/elo',
      59 => 'eoir/sibpages/har',
      60 => 'eoir/sibpages/was',
      61 => 'eoir/sibpages/sin',
      62 => 'eoir/sibpages/lfr',
      63 => 'eoir/sibpages/IJConduct',
      64 => 'eoir/sibpages/buf',
      65 => 'eoir/sibpages/slc',
      66 => 'eoir/sibpages/mem',
      67 => 'eoir/sibpages/mia',
      68 => 'eoir/sibpages/los',
      69 => 'eoir/sibpages/sea',
      70 => 'eoir/sibpages/elz',
      71 => 'eoir/sibpages/saj',
      72 => 'eoir/sibpages/flo',
      73 => 'eoir/sibpages/imp',
      74 => 'eoir/sibpages/btv',
      75 => 'eoir/sibpages/oty',
      76 => 'eoir/sibpages/phi',
      77 => 'eoir/sibpages/poo',
      78 => 'eoir/sibpages/nyd',
      79 => 'eoir/sibpages/hon',
      80 => 'eoir/sibpages/lvg',
      81 => 'eoir/sibpages/elp',
      82 => 'eoir/sibpages/orl',
      83 => 'eoir/sibpages/det',
      84 => 'eoir/sibpages/bal',
      85 => 'eoir/sibpages/spm',
      86 => 'eoir/sibpages/oak',
      87 => 'eoir/sibpages/nyc',
      88 => 'eoir/sibpages/lgd',
      89 => 'eoir/sibpages/kro',
      90 => 'eoir/sibpages/kan',
      91 => 'eoir/engage',
      92 => 'eoir/engage/eRegistry',
      93 => 'eoir/ogc/SantillanPadilla',
      94 => 'eoir/ogc/NgwanyiaVGonzales',
      95 => 'eoir/efoia/ocij',
      96 => 'eoir/efoia',
      97 => 'eoir/eoirforms',
      98 => 'eoir/eoirforms/eoir33',
      99 => 'eoir/vll/country/canada_coi/colombia',
      100 => 'eoir/vll/intdec',
      101 => 'eoir/vll',
      102 => 'eoir/vll/OCIJPracManual/_test',
      103 => 'eoir/vll/OCIJPracManual',
      104 => 'eoir/vll/benchbook/templates',
      105 => 'eoir/vll/benchbook/tools/MHI',
      106 => 'eoir/vll/benchbook/resources',
      107 => 'eoir/vll/benchbook',
      108 => 'eoir/vll/benchbook/alerts',
      109 => 'eoir/vll/qapracmanual',
      110 => 'eoir/vll/fedreg',
      111 => 'eoir/fs',
      112 => 'eoir/ra',
      113 => 'eoir/statspub',
      114 => 'eoir/probono',
      115 => 'eoir/fraudanddiscipline',
      116 => 'eoir/vll/country',
      117 => 'eoir/vll/intdec/precedent_chart',
      118 => 'eoir/vll/genifo',
      119 => 'eoir/vll/benchbook/resources/sfoutline',
      120 => 'eoir/vll/qapracmanual/pracmanual/updates',
      121 => 'eoir/vll/fedreg/1996_1997',
      122 => 'eoir/vll/fedreg/2000_2001',
      123 => 'eoir/vll/ILA-Newsleter',
    );

    $arguments['source_parser_class'] = "EoirSourceParser";

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Executive Office for Immigration Review.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('eoir')
    ->sourceMigration('Organization');
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
 * Migrates files (*.pdf) from /eoir.
 *
 * @package doj_migration
 * @subpackage eoir
 */
class EoirFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the eoir subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'eoir',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///eoir/docs';

    $options = array('recurse' => TRUE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $file_id = $row->fileid;
    $enc = mb_detect_encoding($file_id);
    if ($enc === FALSE) {
      module_load_include('inc', 'doj_migration', 'includes/doj_migration');
      $row->fileid = doj_migration_random_string(10);
      drush_doj_migration_debug_output("Ignored path: {$file_id} --> stored id: {$row->fileid}");
      return FALSE;
    }
    else {
      drush_doj_migration_debug_output("Path: {$file_id} --> encoding: {$enc}");
    }

    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "eoir";
  }
}

class EoirPressReleaseMigration extends JusticeHtmlToPressReleaseMigration {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    $source_dirs = array(
      "eoir/press/2014",
      "eoir/press/2013",
    );

    $options = array(
      'recurse' => TRUE,
    );

    $arguments['source_parser_class'] = "EoirPressSourceParser";

    $arguments['obtainer_methods'] = array(
      'title' => array(
        'findSecondPAlignCenterStrong',
      ),
      'body' => array(),
      'subtitle' => array(),
      'date' => array(
        'findTable3Row3Col2',
      ),
    );

    parent::__construct($arguments, $source_dirs, $options);

    $this->dependencies = array('Organization');
    $this->description = t('Migrates Press Releases from EOIR.');
    $this->addFieldMapping('og_group_ref')->defaultValue('eoir')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $row->field_pr_component = "Executive Office for Immigration Review";
  }
}
