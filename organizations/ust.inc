<?php

/**
 * @file
 * Defines migration classes for U.S. Trustee Program.
 */

/**
 * Migrates .html files from /ust to page nodes.
 *
 * @package doj_migration
 * @subpackage ust
 */
class UstPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'ust',
      33 => 'ust/eo/public_affairs/articles/docs',
      34 => 'ust/eo/public_affairs/articles/docs/2007',
      35 => 'ust/eo/public_affairs/articles',
      43 => 'ust/eo/public_affairs/press',
      44 => 'ust/eo/public_affairs/sig_guidance',
      45 => 'ust/eo/public_affairs/highlights',
      46 => 'ust/eo/public_affairs/reports_studies',
      47 => 'ust/eo/public_affairs',
      48 => 'ust/eo/public_affairs/debtor_audit',
      49 => 'ust/eo/public_affairs/lep',
      50 => 'ust/eo/public_affairs/outreach/docs',
      51 => 'ust/eo/public_affairs/outreach',
      52 => 'ust/eo/public_affairs/testimony',
      53 => 'ust/eo/public_affairs/factsheet/docs',
      54 => 'ust/eo/public_affairs/factsheet',
      55 => 'ust/eo/public_affairs/annualreport',
      56 => 'ust/eo/public_affairs/data_files/enf_act',
      57 => 'ust/eo/public_affairs/data_files/lap',
      58 => 'ust/eo/public_affairs/data_files/lap/lap_statistics',
      59 => 'ust/eo/public_affairs/data_files',
      60 => 'ust/eo/public_affairs/data_files/ch7_asset',
      61 => 'ust/eo/public_affairs/statistics',
      62 => 'ust/eo/public_affairs/statistics/docs/state/13',
      63 => 'ust/eo/public_affairs/consumer_info',
      64 => 'ust/eo/public_affairs/consumer_info/nms',
      65 => 'ust/eo/public_affairs/consumer_info/hamp',
      66 => 'ust/eo',
      67 => 'ust/eo/test',
      68 => 'ust/eo/test/cbc13',
      69 => 'ust/eo/test/cbc12',
      70 => 'ust/eo/foia',
      71 => 'ust/eo/foia/docs',
      72 => 'ust/eo/foia/mis',
      73 => 'ust/eo/ust_org',
      74 => 'ust/eo/ust_org/ustp_manual',
      75 => 'ust/eo/ust_org/bky-info',
      76 => 'ust/eo/bapcpa/20141101',
      77 => 'ust/eo/bapcpa/20120501',
      78 => 'ust/eo/bapcpa/20061001',
      79 => 'ust/eo/bapcpa',
      80 => 'ust/eo/bapcpa/20080201',
      81 => 'ust/eo/bapcpa/20111101',
      82 => 'ust/eo/bapcpa/ccde',
      83 => 'ust/eo/bapcpa/ccde/docs',
      84 => 'ust/eo/bapcpa/20121101',
      85 => 'ust/eo/bapcpa/20131115',
      86 => 'ust/eo/bapcpa/20081001',
      87 => 'ust/eo/bapcpa/20101101',
      88 => 'ust/eo/bapcpa/debtor_audit',
      89 => 'ust/eo/bapcpa/20130401',
      90 => 'ust/eo/bapcpa/20071015',
      91 => 'ust/eo/bapcpa/20110315',
      92 => 'ust/eo/bapcpa/20060213',
      93 => 'ust/eo/bapcpa/20140501',
      94 => 'ust/eo/bapcpa/20100315',
      95 => 'ust/eo/bapcpa/20080101',
      96 => 'ust/eo/bapcpa/uniform_final_rpts/20090401',
      97 => 'ust/eo/bapcpa/uniform_final_rpts',
      98 => 'ust/eo/bapcpa/uniform_final_rpts/20090901',
      99 => 'ust/eo/bapcpa/uniform_final_rpts/20101001',
      100 => 'ust/eo/bapcpa/20051017',
      101 => 'ust/eo/bapcpa/20080317',
      102 => 'ust/eo/bapcpa/20130501',
      103 => 'ust/eo/bapcpa/tfrgs/20090401',
      104 => 'ust/eo/bapcpa/tfrgs',
      105 => 'ust/eo/bapcpa/tfrgs/20090901',
      106 => 'ust/eo/bapcpa/tfrgs/20101001',
      107 => 'ust/eo/bapcpa/20090315',
      108 => 'ust/eo/bapcpa/20140401',
      109 => 'ust/eo/bapcpa/20091101',
      110 => 'ust/eo/bapcpa/20070201',
      111 => 'ust/eo/legal_matters',
      112 => 'ust/eo/private_trustee/locator',
      113 => 'ust/eo/private_trustee/data_statistics',
      114 => 'ust/eo/private_trustee/vacancies',
      115 => 'ust/eo/private_trustee',
      116 => 'ust/eo/private_trustee/ds',
      117 => 'ust/eo/private_trustee/library/chapter11',
      118 => 'ust/eo/private_trustee/library/chapter13/docs',
      119 => 'ust/eo/private_trustee/library/chapter13',
      120 => 'ust/eo/private_trustee/library',
      121 => 'ust/eo/private_trustee/library/chapter12/docs',
      122 => 'ust/eo/private_trustee/library/chapter12',
      123 => 'ust/eo/private_trustee/library/chapter07/docs/form4',
      124 => 'ust/eo/private_trustee/library/chapter07/docs/check21',
      125 => 'ust/eo/private_trustee/library/chapter07/docs/7handbook0301',
      126 => 'ust/eo/private_trustee/library/chapter07/docs/7handbook0301/appendices0301',
      127 => 'ust/eo/private_trustee/library/chapter07/docs/7handbook0301/backups',
      128 => 'ust/eo/private_trustee/library/chapter07/docs/7handbook0301/forms0301',
      129 => 'ust/eo/private_trustee/library/chapter07/docs/forms',
      130 => 'ust/eo/private_trustee/library/chapter07/docs/utc',
      131 => 'ust/eo/private_trustee/library/chapter07/docs/7handbook0702',
      132 => 'ust/eo/private_trustee/library/chapter07/docs/field_exams',
      133 => 'ust/eo/private_trustee/library/chapter07/docs/ch7-QA',
      134 => 'ust/eo/private_trustee/library/chapter07/docs/7handbook',
      135 => 'ust/eo/private_trustee/library/chapter07',
      136 => 'ust/eo/fraud',
      137 => 'ust/eo/rules_regulations/mou99',
      138 => 'ust/eo/rules_regulations/guidelines/docs',
      139 => 'ust/eo/rules_regulations/guidelines',
      140 => 'ust/eo/rules_regulations/docs',
      141 => 'ust/eo/rules_regulations',
      142 => 'ust/eo/rules_regulations/admin_decisions',
      143 => 'ust/eo/rules_regulations/admin_decisions/docs',
    );

    $options = array('recurse' => FALSE);

    $arguments['source_parser_class'] = "UstPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UstObtainTitlePage");
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('pluckSelector', array("title", 1));
    $title->addMethod('pluckSelector', array("h1", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the U.S. Trustee Program.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('ust')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array();
    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /ust.
 *
 * @package doj_migration
 * @subpackage ust
 */
class UstFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the ust subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      29 => 'ust/eo/public_affairs/articles/docs',
      30 => 'ust/eo/public_affairs/articles/docs/2007',
      31 => 'ust/eo/public_affairs/articles/docs/2011',
      32 => 'ust/eo/public_affairs/articles/docs/2010',
      33 => 'ust/eo/public_affairs/articles/docs/2009',
      34 => 'ust/eo/public_affairs/articles/docs/2008',
      35 => 'ust/eo/public_affairs/articles/docs/2014',
      36 => 'ust/eo/public_affairs/articles/docs/2013',
      37 => 'ust/eo/public_affairs/articles/docs/2012',
      45 => 'ust/eo/public_affairs/sig_guidance/docs',
      46 => 'ust/eo/public_affairs/reports_studies/docs',
      47 => 'ust/eo/public_affairs/debtor_audit/docs',
      48 => 'ust/eo/public_affairs/lep/docs',
      49 => 'ust/eo/public_affairs/outreach/docs',
      50 => 'ust/eo/public_affairs/testimony/docs',
      51 => 'ust/eo/public_affairs/testimony/docs/speeches',
      52 => 'ust/eo/public_affairs/annualreport/docs',
      53 => 'ust/eo/public_affairs/data_files/enf_act/docs/2011',
      54 => 'ust/eo/public_affairs/data_files/enf_act/docs',
      55 => 'ust/eo/public_affairs/data_files/enf_act/docs/2013',
      56 => 'ust/eo/public_affairs/data_files/enf_act/docs/2012',
      57 => 'ust/eo/public_affairs/data_files/lap/docs',
      58 => 'ust/eo/public_affairs/data_files/ch7_asset/docs',
      59 => 'ust/eo/public_affairs/statistics/docs/state/13',
      60 => 'ust/eo/public_affairs/consumer_info/docs',
      61 => 'ust/eo/public_affairs/consumer_info/nms/docs',
      62 => 'ust/eo/foia/docs',
      63 => 'ust/eo/ust_org/ustp_manual/docs',
      64 => 'ust/eo/ust_org/ustp_manual/docs/appendices',
      65 => 'ust/eo/ust_org/ustp_manual/docs/appendices/samptrusads',
      66 => 'ust/eo/ust_org/bky-info/docs',
      67 => 'ust/eo/ust_org',
      68 => 'ust/eo/bapcpa/ccde/docs',
      69 => 'ust/eo/bapcpa/ccde/cgs',
      70 => 'ust/eo/bapcpa/docs',
      71 => 'ust/eo/bapcpa/debtor_audit/docs',
      72 => 'ust/eo/bapcpa/proposed_rulemaking/docs',
      73 => 'ust/eo/bapcpa/uniform_final_rpts/20090401/docs',
      74 => 'ust/eo/bapcpa/uniform_final_rpts/20090901/docs',
      75 => 'ust/eo/bapcpa/uniform_final_rpts/20101001/docs',
      76 => 'ust/eo/bapcpa/tfrgs/20090401/docs',
      77 => 'ust/eo/bapcpa/tfrgs/docs',
      78 => 'ust/eo/bapcpa/tfrgs/20090901/docs',
      79 => 'ust/eo/bapcpa/tfrgs/20101001/docs',
      80 => 'ust/eo/legal_matters/docs',
      81 => 'ust/eo/private_trustee/library/chapter11/docs',
      82 => 'ust/eo/private_trustee/library/chapter13/docs',
      83 => 'ust/eo/private_trustee/library/chapter13/docs/audit_process',
      84 => 'ust/eo/private_trustee/library/chapter13/docs/handbook2010',
      85 => 'ust/eo/private_trustee/library/chapter13/docs/handbook2012',
      86 => 'ust/eo/private_trustee/library/chapter13/docs/handbook2012_supp',
      87 => 'ust/eo/private_trustee/library/chapter13/docs/Ch13handbook2008',
      88 => 'ust/eo/private_trustee/library/chapter12/docs/ch12hbk_2013',
      89 => 'ust/eo/private_trustee/library/chapter12/docs',
      90 => 'ust/eo/private_trustee/library/chapter07/docs/form4',
      91 => 'ust/eo/private_trustee/library/chapter07/docs/other_poli',
      92 => 'ust/eo/private_trustee/library/chapter07/docs/check21',
      93 => 'ust/eo/private_trustee/library/chapter07/docs/7handbook0301',
      94 => 'ust/eo/private_trustee/library/chapter07/docs/7handbook0301/backups',
      95 => 'ust/eo/private_trustee/library/chapter07/docs/7handbook0301/forms0301',
      96 => 'ust/eo/private_trustee/library/chapter07/docs/7handbook1008',
      97 => 'ust/eo/private_trustee/library/chapter07/docs/chapter07_0301/7handbook0301',
      98 => 'ust/eo/private_trustee/library/chapter07/docs/forms',
      99 => 'ust/eo/private_trustee/library/chapter07/docs/utc',
      100 => 'ust/eo/private_trustee/library/chapter07/docs/assetcases',
      101 => 'ust/eo/private_trustee/library/chapter07/docs/ch7hb2012',
      102 => 'ust/eo/private_trustee/library/chapter07/docs',
      103 => 'ust/eo/private_trustee/library/chapter07/docs/field_exams',
      104 => 'ust/eo/private_trustee/library/chapter07/docs/ch7_handbook',
      105 => 'ust/eo/private_trustee/library/chapter07/docs/ch7-QA',
      106 => 'ust/eo/fraud/docs',
      107 => 'ust/eo/rules_regulations/mou99',
      108 => 'ust/eo/rules_regulations/guidelines/docs',
      109 => 'ust/eo/rules_regulations/guidelines/docs/appxB',
      110 => 'ust/eo/rules_regulations/guidelines/docs/proposed',
      111 => 'ust/eo/rules_regulations/docs',
      112 => 'ust/eo/rules_regulations/admin_decisions/docs',
      113 => 'ust/eo/rules_regulations/admin_decisions/docs/2007',
      114 => 'ust/eo/rules_regulations/admin_decisions/docs/CC/2007',
      115 => 'ust/eo/rules_regulations/admin_decisions/docs/CC/2006',
      116 => 'ust/eo/rules_regulations/admin_decisions/docs/CC/2011',
      117 => 'ust/eo/rules_regulations/admin_decisions/docs/CC/2008',
      118 => 'ust/eo/rules_regulations/admin_decisions/docs/2011',
      119 => 'ust/eo/rules_regulations/admin_decisions/docs/2010',
      120 => 'ust/eo/rules_regulations/admin_decisions/docs/2009',
      121 => 'ust/eo/rules_regulations/admin_decisions/docs/2008',
      122 => 'ust/eo/rules_regulations/admin_decisions/docs/DE/2007',
      123 => 'ust/eo/rules_regulations/admin_decisions/docs/DE/2008',
      124 => 'ust/eo/rules_regulations/admin_decisions/docs/2013',
      125 => 'ust/eo/rules_regulations/admin_decisions/docs/2012',
      393 => 'ust/eo/private_trustee/ds/docs',
      398 => 'ust/eo/bapcpa/ccde/docs/credit_card_act',
      412 => 'ust/eo/private_trustee/data_statistics/docs/chapter12',
      413 => 'ust/eo/private_trustee/library/chapter13/docs/ch13stat',
      417 => 'ust/eo/public_affairs/statistics/docs/state/filings',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx|zip)/i';
    $dest_dir = 'public:///ust/docs';

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
    return "ust";
  }
}

/**
 * Migrates press releases from /ust.
 *
 * @package doj_migration
 * @subpackage ust
 */
class UstPressReleaseMigration extends NGJusticeHtmlToPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['component'] = "U.S. Trustee Program";
    $arguments['source_parser_class'] = "UstPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UstObtainTitlePress");
    $title->addMethod("findPressContentTitle");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array("h2", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));
    $title->addMethod('pluckSelector', array("p > strong > em", 1));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 1));

    $date = new ObtainerInfo('date', "UstObtainDate");
    $date->addMethod("findPressContentDateContainer");
    $date->addMethod("findPressContentDate");
    $date->addMethod('pluckTableRow1Col2');
    $date->addMethod('pluckTableRow1Col1');
    $date->addMethod('pluckTable2Row2Col2');
    $date->addMethod('pluckSelector', array("p[align='center']", 1));
    $date->addMethod('pluckSelector', array("#contentstart > p", 1));
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckSelector', array(".BottomLeftContent", 1));
    $date->addMethod('pluckProbableDate');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['component_tid'] = 666;

    $source_dirs = array(
      41 => 'ust/eo/public_affairs/press/docs',
    );
    parent::__construct($arguments, $source_dirs);
    $this->addFieldMapping('og_group_ref')->defaultValue('ust')->sourceMigration('Organization');
    $this->addFieldMapping('context')->defaultValue('no_right_sidebar');
    $this->addFieldMapping('field_pr_attachment', 'attachment_filename');
    $this->addFieldMapping('field_pr_attachment:file_class')
      ->defaultValue('MigrateFileUri');
    $this->addFieldMapping('field_pr_attachment:source_dir', 'source_dir');
    $this->addFieldMapping('field_pr_attachment:file_replace')->defaultValue(FILE_EXISTS_REUSE);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);
    $full_path = $row->fileid;
    $filename = basename($row->fileid);
    $source_dir = $this->baseDir . str_replace("/" . $filename, "", $full_path);
    $filename = str_replace(".htm", ".pdf", $filename);
    $row->source_dir = $source_dir;
    $row->attachment_filename = $filename;
  }
}

class UstPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    // Remove banners from pages.
    $images = $this->queryPath->find("img");
    foreach ($images as $image) {
      $src = $image->attr('src');
      if (substr_count($src, "-banner")) {
        $image->remove();
      }
    }
    parent::cleanHtml();
  }
}

class UstPressSourceParser extends NGPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    $this->queryPath->find('.justicenews-header')->first()->remove();
    $this->queryPath->find('.presscontenthdr')->first()->remove();
    $this->queryPath->find('.presscontentsubhdr')->first()->remove();
  }
}

class UstObtainTitlePage extends ObtainTitle {
}

class UstObtainTitlePress extends ObtainTitlePressRelease {
  /**
   * Get text from the presscontenttitle class.
   */
  protected function findPressContentTitle() {
    $title = $this->queryPath->find(".presscontenttitle")->first();
    $this->setElementToRemove($title);
    return $title->text();
  }
}

class UstObtainDate extends ObtainDate {
  /**
   * Get text from the presscontentdatecontainer class.
   */
  protected function findPressContentDateContainer() {
    $date = $this->queryPath->find(".presscontentdatecontainer")->first();
    $this->setElementToRemove($date);
    return $date->text();
  }

  /**
   * Get text from the presscontentdate class.
   */
  protected function findPressContentDate() {
    $date = $this->queryPath->find(".presscontentdate")->first();
    $this->setElementToRemove($date);
    return $date->text();
  }
}
