<?php

/**
 * @file
 * Defines migration classes for Middle District of Pennsylvania.
 */

/**
 * Migrates .html files from /usao-mdpa to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-mdpa
 */
class UsaoMdpaPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/pam/Corruption/Skrepenak',
      1 => 'usao/pam/Corruption',
      2 => 'usao/pam/Corruption/Sharkey',
      3 => 'usao/pam/Corruption/Emanski',
      4 => 'usao/pam/Corruption/Bonner',
      5 => 'usao/pam/Corruption/Greco',
      6 => 'usao/pam/Corruption/Dunn',
      7 => 'usao/pam/Corruption/Piazza',
      8 => 'usao/pam/Corruption/Arnone',
      9 => 'usao/pam/Corruption/Holly',
      10 => 'usao/pam/Corruption/Mellow',
      11 => 'usao/pam/Corruption/Spinozza',
      12 => 'usao/pam/Corruption/Toole',
      13 => 'usao/pam/Corruption/Pizzella',
      14 => 'usao/pam/Corruption/Moran',
      15 => 'usao/pam/Corruption/Ciavarella_Conahan',
      16 => 'usao/pam/Corruption/Pasonick',
      17 => 'usao/pam/Corruption/Mericle',
      18 => 'usao/pam/Corruption/Weidlich',
      19 => 'usao/pam/Corruption/Brace',
      20 => 'usao/pam/Corruption/Reilly',
      21 => 'usao/pam/Corruption/Maguire',
      22 => 'usao/pam/Corruption/Richards',
      23 => 'usao/pam/Corruption/Oliveri',
      24 => 'usao/pam/Corruption/Powell',
      25 => 'usao/pam/Corruption/Height',
      26 => 'usao/pam/Corruption/Bellas',
      27 => 'usao/pam/Corruption/Musto',
      28 => 'usao/pam/Corruption/Scarantino',
      29 => 'usao/pam/Corruption/Ostrowski',
      30 => 'usao/pam/Corruption/Cordaro_Munchak',
      31 => 'usao/pam/Corruption/Gaughan',
      32 => 'usao/pam/Corruption/Brulo',
      33 => 'usao/pam/PSC/Ostrowski',
      34 => 'usao/pam',
      36 => 'usao/pam/images',
      37 => 'usao/pam/Corruption/Scheduled_court',
      38 => 'usao/pam/js',
      39 => 'usao/pam/js/s3Slider Full',
      40 => 'usao/pam/Victim_Witness',
      41 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Skrepenak',
      42 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Sharkey',
      43 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Emanski',
      44 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Bonner',
      45 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Greco',
      46 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Dunn',
      47 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Piazza',
      48 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Arnone',
      49 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Holly',
      50 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption',
      51 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Spinozza',
      52 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Toole',
      53 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Pizzella',
      54 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Moran',
      55 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Ciavarella_Conahan',
      56 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Mericle',
      57 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Weidlich',
      58 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Brace',
      59 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Reilly',
      60 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Maguire',
      61 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Richards',
      62 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Oliveri',
      63 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Powell',
      64 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Height',
      65 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Bellas',
      66 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Musto',
      67 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Scarantino',
      68 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Scheduled_court',
      69 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Gaughan',
      70 => 'usao/pam/Victim_Witness/Luzerne_County_Corruption/Brulo',
    );

    $arguments['source_parser_class'] = "NGDistrictPageSourceParser";
    $title = new ObtainerInfo("title", 'ObtainTitle');
    $title->addMethod('findSelector', array("h1", 2));
    $title->addMethod('findH1Any');
    $arguments['obtainers_info'][] = $title;

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Middle District of Pennsylvania.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-mdpa')
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
 * Migrates files (*.pdf) from /usao-mdpa.
 *
 * @package doj_migration
 * @subpackage usao-mdpa
 */
class UsaoMdpaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the usao/pam subdirectory.');
    $this->dependencies = array('UsaoDistrict');

    // Match .pdf files only.
    $source_dirs = array(
      'usao/pam',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///usao-mdpa/docs';

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
    return "usao-mdpa";
  }
}

/**
 * Migrates press releases from /usao-mdpa.
 *
 * @package doj_migration
 * @subpackage usao-mdpa
 */
class UsaoMdpaPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-mdpa";
    $arguments['component_tid'] = 2031;
    $arguments['source_parser_class'] = "MdpaPressSourceParser";

    $source_dirs = array(
      35 => 'usao/pam/news/2013',
      72 => 'usao/pam/news/2014',
      73 => 'usao/pam/news/2015',
    );
    parent::__construct($arguments, $source_dirs, $options);
  }

}


/**
 * Migrates PRs from /usao-mdpa/Victim_Witness/Luzerne_County_Corruption.
 *
 * @package doj_migration
 * @subpackage usao-mdpa
 */
class UsaoMdpaVwPressReleaseMigration extends NGDistrictPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['district'] = "usao-mdpa";
    $arguments['component_tid'] = 2031;
    $arguments['source_parser_class'] = "MdpaPressSourceParser";
    $options = array('recurse' => TRUE);

    $source_dirs = array(
      1 => 'usao/pam/Corruption',
    );
    parent::__construct($arguments, $source_dirs);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      'usao/pam/Corruption/Musto/musto_index.html',
      'usao/pam/Corruption/Ostrowski/ostrowski_index.html',
    );

    if ((parent::prepareRow($row) === FALSE) || ($row->content_type !== 'press_release') || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }
  }

}

/**
 *  Extends the sourceparser for custom obtainer method stacks in usao-mdpa.
 */
class MdpaPressSourceParser extends NGDistrictPressReleaseSourceParser {


  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
    parent::setDefaultObtainersInfo();
    $ct = new ObtainerInfo('content_type');
    $ct->addMethod("findPRClassBreadcrumbPressRelease");
    $ct->addMethod("findPRImmediateRelease");
    $this->addObtainerInfo($ct);

    $date = new ObtainerInfo('date');
    $date->addMethod("findDivPAlignLeft");
    $date->addMethod('findSelector', array(".newsRight", 1));
    $date->addMethod("findDivClassContentSubDiv3");
    $date->addMethod("findProbableDate");
    $this->addObtainerInfo($date);

    $title = new ObtainerInfo('title');
    $title->addMethod('findH1Any');
    $title->addMethod('findSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('findSelector', array("h2", 1));
    $title->addMethod('findSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('findSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('findSelector', array("p > strong > em", 1));
    $title->addMethod('findSelector', array("#contentstart > div > h2", 1));
    $title->addMethod('findSelector', array("div > p[align='center']", 1));
    $this->addObtainerInfo($title);
  }
}
