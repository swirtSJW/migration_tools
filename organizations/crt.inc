<?php

/**
 * @file
 * Defines migration classes for Civil Rights Division.
 */

/**
 * Migrates .html files from /crt to page nodes.
 *
 * @package doj_migration
 * @subpackage crt
 */
class CrtPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // @codingStandardsIgnoreStart
    // Define source directories.
    $source_dirs = array(
      1 => 'crt/spec_topics/religiousdiscrimination',
      2 => 'crt/spec_topics/religiousdiscrimination/newsletter',
      3 => 'crt/about/osc/htm',
      4 => 'crt/about/osc/pdf',
      5 => 'crt/about/osc/pdf/publications',
      6 => 'crt/about/osc/ref',
      7 => 'crt/about/osc/press',
      8 => 'crt/about/drs',
      9 => 'crt/about/vot/hava',
      10 => 'crt/about/vot/notices',
      11 => 'crt/about/vot/42usc',
      12 => 'crt/about/vot',
      13 => 'crt/about/vot/sec_2',
      14 => 'crt/about/vot/misc',
      15 => 'crt/about/vot/sec_203',
      16 => 'crt/about/vot/sec_203/documents',
      17 => 'crt/about/vot/litigation',
      18 => 'crt/about/vot/28cfr/51',
      19 => 'crt/about/vot/nvra',
      20 => 'crt/about/vot/sec_5',
      21 => 'crt/about/edu',
      22 => 'crt/about/edu/documents',
      23 => 'crt/about/cor',
      24 => 'crt/about/cor/lep',
      25 => 'crt/about/cor/Pubs',
      26 => 'crt/about/cor/Pubs/forum',
      27 => 'crt/about/cor/coord',
      28 => 'crt/about/cor/byagency',
      29 => 'crt/about/emp/documents',
      30 => 'crt/about/emp',
      31 => 'crt/about/app',
      32 => 'crt/about/app/briefs',
      33 => 'crt/about/hce',
      34 => 'crt/about/hce/housing_esp',
      35 => 'crt/about/hce/documents',
      36 => 'crt/about/hce/documents/_trash_can',
      37 => 'crt/about/hce/documents/weaversettle_files',
      38 => 'crt/about/hce/fairhousing',
      39 => 'crt/about/spl',
      40 => 'crt/about/spl/documents',
      41 => 'crt/about/spl/documents/pgpd',
      42 => 'crt/about/spl/documents/us-v-lu',
      43 => 'crt/about/spl/documents/dpd',
      44 => 'crt/about/spl/documents/georgia',
      45 => 'crt/about/spl/Spanish Final docs',
      46 => 'crt/about/crm/trafficking_newsletter',
      47 => 'crt/housing/fairhousing/for_translation',
      51 => 'crt/foia/readingroom/frequent_requests/ada_tal',
      52 => 'crt/foia/readingroom/frequent_requests/ada_coreletter',
      53 => 'crt/foia/readingroom/frequent_requests/ada_settlements/dc',
      54 => 'crt/foia/readingroom/bostonjfk/text',
      55 => 'crt/split/Spanish Final docs',
      56 => 'crt/pubs',
      57 => 'crt/records/vot/obj_letters/letters/new_ltr/2013_0895ltr_130408_files',
      58 => 'crt/records/vot/obj_letters/letters/new_ltr',
      59 => 'crt/records/vot/obj_letters/letters/new_ltr/old_names',
      60 => 'crt/508',
      61 => 'crt/508/report',
      62 => 'crt/legalinfo',
      67 => 'crt/about/osc',
      68 => 'crt/about/cor/Pubs/manuals',
      69 => 'crt/about/spl/documents/usvmcmillian',
      70 => 'crt/about/crm/wetf',
      71 => 'crt/about/crm',
      72 => 'crt/publications/accomplishments',
      75 => 'crt',
      // 79 => 'crt/records/vot/obj_letters/letters/zz-list of non accessible pdfs',
      81 => 'crt/contact/election',
      83 => 'crt/508/archive',
      88 => 'crt/spec_topics/wellsfargo',
      89 => 'crt/spec_topics/mdoc',
      95 => 'crt/church_arson',
      96 => 'crt/about/osc/htm/telephone_interventions',
      97 => 'crt/about/vot/examine',
      98 => 'crt/about/vot/intro',
      99 => 'crt/about',
      100 => 'crt/about/wg',
      101 => 'crt/about/wg/iwg',
      102 => 'crt/about/wg/lgbti',
      103 => 'crt/about/hce/cases',
      104 => 'crt/about/pol',
      105 => 'crt/about/crm/mlk',
      109 => 'crt/publications',
      110 => 'crt/publications/ebola',
      115 => 'crt/aag',
      117 => 'crt/grants_statutes',
      118 => 'crt/grants_statutes/forum',
      119 => 'crt/pressroom',
      125 => 'crt/complaint',
      126 => 'crt/foia',
      127 => 'crt/foia/readingroom',
      128 => 'crt/foia/readingroom/mississippi',
      129 => 'crt/foia/readingroom/frequent_requests',
      130 => 'crt/foia/readingroom/frequent_requests/ada_lof',
      131 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ut',
      132 => 'crt/foia/readingroom/frequent_requests/ada_settlements/pa',
      133 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ma',
      134 => 'crt/foia/readingroom/frequent_requests/ada_settlements/wa',
      135 => 'crt/foia/readingroom/frequent_requests/ada_settlements/in',
      136 => 'crt/foia/readingroom/frequent_requests/ada_settlements/mo',
      77 => 'crt/foia/readingroom/frequent_requests/ada_settlements/oh',
      137 => 'crt/foia/readingroom/frequent_requests/ada_settlements/wy',
      138 => 'crt/foia/readingroom/frequent_requests/ada_settlements/nm',
      139 => 'crt/foia/readingroom/frequent_requests/ada_settlements/kn',
      140 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ak',
      141 => 'crt/foia/readingroom/frequent_requests/ada_settlements/va',
      142 => 'crt/foia/readingroom/frequent_requests/ada_settlements',
      143 => 'crt/foia/readingroom/frequent_requests/ada_settlements/mn',
      144 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ok',
      145 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ia',
      146 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ne',
      147 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ws',
      148 => 'crt/foia/readingroom/frequent_requests/ada_settlements/wv',
      149 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ks',
      150 => 'crt/foia/readingroom/frequent_requests/ada_settlements/nh',
      151 => 'crt/foia/readingroom/frequent_requests/ada_settlements/id',
      152 => 'crt/foia/readingroom/frequent_requests/ada_settlements/al',
      153 => 'crt/foia/readingroom/frequent_requests/ada_settlements/md',
      154 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ny',
      155 => 'crt/foia/readingroom/frequent_requests/ada_settlements/nd',
      156 => 'crt/foia/readingroom/frequent_requests/ada_settlements/sc',
      157 => 'crt/foia/readingroom/frequent_requests/ada_settlements/nc',
      158 => 'crt/foia/readingroom/frequent_requests/ada_settlements/az',
      159 => 'crt/foia/readingroom/frequent_requests/ada_settlements/co',
      160 => 'crt/foia/readingroom/frequent_requests/ada_settlements/mas',
      161 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ca',
      162 => 'crt/foia/readingroom/frequent_requests/ada_settlements/mi',
      163 => 'crt/foia/readingroom/frequent_requests/ada_settlements/nv',
      164 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ga',
      165 => 'crt/foia/readingroom/frequent_requests/ada_settlements/sd',
      166 => 'crt/foia/readingroom/frequent_requests/ada_settlements/tn',
      167 => 'crt/foia/readingroom/frequent_requests/ada_settlements/dl',
      168 => 'crt/foia/readingroom/frequent_requests/ada_settlements/nj',
      169 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ha',
      170 => 'crt/foia/readingroom/frequent_requests/ada_settlements/vt',
      171 => 'crt/foia/readingroom/frequent_requests/ada_settlements/or',
      172 => 'crt/foia/readingroom/frequent_requests/ada_settlements/mt',
      173 => 'crt/foia/readingroom/frequent_requests/ada_settlements/tx',
      174 => 'crt/foia/readingroom/frequent_requests/ada_settlements/ri',
      175 => 'crt/foia/readingroom/frequent_requests/ada_settlements/la',
      176 => 'crt/foia/readingroom/frequent_requests/ada_settlements/mis',
      177 => 'crt/foia/readingroom/frequent_requests/ada_settlements/fl',
      178 => 'crt/foia/readingroom/frequent_requests/ada_settlements/il',
      179 => 'crt/foia/readingroom/frequent_requests/ada_settlements/cn',
      180 => 'crt/foia/readingroom/frequent_requests/title_i_settlements',
      181 => 'crt/foia/readingroom/frequent_requests/days_inn',
      182 => 'crt/foia/readingroom/bostonjfk',
      183 => 'crt/foia/readingroom/wilmington',
      185 => 'crt/records/vot/obj_letters',

      // Temporarilly removed until they figure out what to do with letters
      // that have no titles.
      // 186 => 'crt/records/vot/obj_letters/letters/NC',
      // 187 => 'crt/records/vot/obj_letters/letters/NY',
      // 188 => 'crt/records/vot/obj_letters/letters/AZ',
      // 189 => 'crt/records/vot/obj_letters/letters/VA',
      // 190 => 'crt/records/vot/obj_letters/letters/SC',
      // 191 => 'crt/records/vot/obj_letters/letters/GA',
      // 192 => 'crt/records/vot/obj_letters/letters/AL',
      // 193 => 'crt/records/vot/obj_letters/letters/TX',
      // 194 => 'crt/records/vot/obj_letters/letters/MS',
      // 195 => 'crt/records/vot/obj_letters/letters/MI',
      // 196 => 'crt/records/vot/obj_letters/letters/CA',
      // 197 => 'crt/records/vot/obj_letters/letters/SD',
      // 198 => 'crt/records/vot/obj_letters/letters/FL',
      // 199 => 'crt/records/vot/obj_letters/letters/LA',
      202 => 'crt/records',
      203 => 'crt/employment',
      204 => 'crt/contact',
      206 => 'crt/508/report2',

      // Only contain php header redirects.
      // 122 => 'crt/app',
      // 114 => 'crt/cor/coord',
      // 112 => 'crt/cor/lep',
      // 113 => 'crt/cor/Pubs',
      // 111 => 'crt/cor',
      // 124 => 'crt/crim',
      // 106 => 'crt/drs',
      // 121 => 'crt/edo',
      // 116 => 'crt/emp',
      // 94 => 'crt/fcs',
      // 108 => 'crt/housing/fairhousing',
      // 107 => 'crt/housing',
      // 48 => 'crt/lep',
      // 73 => 'crt/lep/guidance',
      // 84 => 'crt/osc',
      // 120 => 'crt/pressroom/videos',
      // 200 => 'crt/records/vot/PubSubmSearch',
      // 201 => 'crt/records/vot/PubSubmSearch/include',
      // 80 => 'crt/records/vot/PubSubmSearch/backup',
      // 123 => 'crt/recruit_employ',
      // 82 => 'crt/religdisc',
      // 78 => 'crt/religiousdiscrimination',
      // 85 => 'crt/spec_topics',
      // 184 => 'crt/split',
      // 205 => 'crt/voting',

      // Forms that can't be migrated.
      // 76 => 'crt/complaint/votintake',
      // 66 => 'crt/spec_topics/osc_macys',
      // 65 => 'crt/spec_topics/votintake.dev',

      // Removed because they are covered by other crt-* migrations.
      // 0 => 'crt/spec_topics/military',
      // 63 => 'crt/spec_topics/military/statutes',
      // 64 => 'crt/spec_topics/military/includes',
      // 90 => 'crt/spec_topics/military/news',
      // 300 => 'crt/spec_topics/fdny',
      // 350 => 'crt/spec_topics/njcsc',

    );
    // @codingStandardsIgnoreEnd

    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(htm|html|php)$/i';

    $arguments['source_parser_class'] = "CrtPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CrtObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('findSelectorAttribute', array("h1 img", 'title'));
    $title->addMethod('findSubBannerTitle');
    $title->addMethod('findSubBannerAlt');
    $title->addMethod('pluckAnySelectorUntilValid', array("h1"));
    $title->addMethod('pluckAnySelectorUntilValid', array("h2"));
    $title->addMethod('pluckAnySelectorUntilValid', array("h3"));
    $title->addMethod('pluckAnySelectorUntilValid', array("h4"));
    $title->addMethod('findSelectorAttribute', array("div > img", 'title'));
    $title->addMethod('findSelectorAttribute', array("div > img", 'alt'));
    $title->addMethod('pluckSelector', array("td > center > b", 2));

    // These are all high risk because they are so generic.
    $title->addMethod('pluckSelector', array("p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array("p[align='CENTER'] > strong", 1));
    $title->addMethod('pluckSelector', array("div[align='center'] > b", 1));
    $title->addMethod(pluckSelector, array('p > center > strong', 1));
    $title->addMethod('pluckSelector', array("p[align='center']", 1));
    $title->addMethod('pluckSelector', array("div[align='center']", 1));
    $title->addMethod('findSelectorAttribute', array("img", 'title'));
    $title->addMethod('pluckSelector', array("title", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Civil Rights Division.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('crt')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      // Header redirect pages.
      '/crt/contact/election/index.php',
      '/crt/about/crm/wetf/wetf2.html',
      '/crt/about/crm/wetf/victimsbrochure.html',
      '/crt/about/crm/wetf/trafficbrochure.html',
      '/crt/about/crm/wetf/wetf4.html',
      '/crt/about/crm/wetf/wetf3.html',
      '/crt/about/crm/wetf/traffic.html',
      '/crt/about/crm/wetf/wetfpolicy.html',
      '/crt/about/crm/wetf/wetfletter.html',
      '/crt/about/spl/documents/usvmcmillian/us_v_mcmillan_coverpage_4-29-08.html',
      '/crt/about/cor/Pubs/manuals/tab1.html',
      '/crt/about/cor/Pubs/manuals/tab2.html',
      '/crt/about/cor/Pubs/manuals/tab3.html',
      '/crt/about/cor/Pubs/manuals/tab4.html',
      '/crt/about/cor/Pubs/manuals/tab5.html',
      '/crt/about/cor/Pubs/manuals/tab6.html',
      '/crt/about/cor/Pubs/manuals/tab7.html',
      '/crt/about/cor/Pubs/manuals/tab8.html',
      '/crt/about/cor/Pubs/manuals/tab9.html',
      '/crt/about/cor/Pubs/manuals/tab10.html',
      '/crt/about/cor/Pubs/manuals/tab11.html',
      '/crt/about/cor/Pubs/manuals/tab12.html',
      '/crt/about/cor/Pubs/manuals/tab13.html',
      '/crt/about/cor/Pubs/manuals/tab14.html',
      '/crt/about/cor/Pubs/manuals/tab15.html',
      '/crt/about/cor/Pubs/manuals/tab16.html',
      '/crt/about/cor/Pubs/manuals/tab17.html',
      '/crt/about/cor/Pubs/manuals/tab18.html',
      '/crt/about/cor/Pubs/manuals/tab19.html',
      '/crt/about/cor/Pubs/manuals/tab20.html',
      '/crt/about/cor/Pubs/manuals/tab21.html',
      '/crt/about/cor/Pubs/manuals/tab22.html',
      '/crt/about/cor/Pubs/manuals/tab23.html',
      '/crt/about/cor/Pubs/manuals/tab24.html',
      '/crt/about/cor/Pubs/manuals/tab25.html',
      '/crt/about/cor/Pubs/manuals/tab26.html',
      '/crt/about/cor/Pubs/manuals/tab27.html',
      '/crt/about/cor/Pubs/manuals/tab28.html',

      // Pages that have no real content once php is removed.
      '/crt/records/vot/obj_letters/index.php',
      '/crt/pressroom/index.php',
      '/crt/pressroom/more_news.php',

      // Miscellaneous cruft.
      '/crt/contact/election/snippet.html',
      '/crt/pressroom/rss.php',
      '/crt/pressroom/_get_news.php',
      '/crt/pressroom/videos.php',
      '/crt/records/vot/obj_letters/states.php',
      '/crt/spec_topics/mdoc/_page_mods.php',
      '/crt/spec_topics/wellsfargo/_include.php',
      '/crt/about/hce/cases/race.php',
      '/crt/index_features.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Logic could go here to check the path using isInPath() to apply titles
    // to all the letters that have no titles.

  }
}

/**
 * Migrates binary files from /crt.
 *
 * @package doj_migration
 * @subpackage crt
 */
class CrtFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the crt subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'crt',
    );

    $dest_dir = 'public:///crt/docs';
    $regex = NULL;

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
    return "crt";
  }
}

/**
 * SourceParser for /crt pages.
 *
 * @package doj_migration
 * @subpackage crt
 */
class CrtPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /crt pages.
 *
 * @package doj_migration
 * @subpackage crt
 * @see Obtainer.api.php
 */
class CrtObtainTitlePage extends ObtainTitle {

}


/**
 * Migrates Speeches from /crt.
 *
 * @package doj_migration
 * @subpackage crt
 */
class CrtSpeechMigration extends NGJusticeHtmlToSpeechMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['component'] = "Civil Rights Division";
    $arguments['component_tid'] = 436;
    $source_dirs = array(
      49 => 'crt/speeches',
      50 => 'crt/speeches/2009',
      74 => 'crt/speeches/2010',
    );
    $options = array('recurse' => TRUE);
    $extension_regex = '/.*\.(htm|html|php)$/i';
    $arguments['source_parser_class'] = "CrtSpeechSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CrtObtainTitleSpeech");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array("h2", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 1));

    $date = new ObtainerInfo('date', "CrtObtainDate");
    $date->addMethod('pluckSelector', array("#contentstart > p", 1));
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckSelector', array(".BottomLeftContent", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "CrtObtainPrNumber");

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    parent::__construct($arguments, $source_dirs, $options, $extension_regex);

    $this->addFieldMapping('og_group_ref')
    ->defaultValue('crt')
    ->sourceMigration('Organization');
  }
}

/**
 * SourceParser for /crt Speeches.
 *
 * @package doj_migration
 * @subpackage crt
 */
class CrtPressSourceParser extends NGSpeechSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /crt Speeches.
 *
 * @package doj_migration
 * @subpackage crt
 * @see Obtainer.api.php
 */
class CrtObtainTitleSpeech extends ObtainTitle {

}

/**
 * Obtainer for prNumber property for /crt Speeches.
 *
 * @package doj_migration
 * @subpackage crt
 * @see Obtainer.api.php
 */
class CrtObtainPrNumber extends ObtainId {
  /**
   * Finds the press release number from markup.
   */
  protected function findPrNumber() {
    $text = '';
    // $element = $this->queryPath->find('p')->last();
    // $this->setElementToRemove($element);
    // $text = $element->text();

    return $text;
  }
}

/**
 * Custom obtainer for date property for /crt.
 *
 * @package doj_migration
 * @subpackage crt
 * @see Obtainer.api.php
 */
class CrtObtainDate extends ObtainDate {
}
