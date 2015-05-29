<?php

/**
 * @file
 * Defines migration classes for Fraud Section.
 */

/**
 * Migrates .html files from /criminal-fraud to page nodes.
 *
 * @package doj_migration
 * @subpackage criminal-fraud
 */
class CriminalFraudPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'criminal/fraud/career',
      1 => 'criminal/fraud/contact',
      2 => 'criminal/fraud/documents',
      3 => 'criminal/fraud/fcpa',
      4 => 'criminal/fraud/hcf',
      5 => 'criminal/fraud/internet',
      6 => 'criminal/fraud/sff',
      61 => 'criminal/fraud/sff/cases-accounting',
      62 => 'criminal/fraud/sff/cases-commodities-fraud',
      63 => 'criminal/fraud/sff/cases-insider-trading',
      64 => 'criminal/fraud/sff/cases-investment-fraud',
      65 => 'criminal/fraud/sff/cases-libor-fx',
      66 => 'criminal/fraud/sff/cases-market-manipulation',
      67 => 'criminal/fraud/sff/cases-mortgage-fraud',
      68 => 'criminal/fraud/sff/cases-procurement',
      69 => 'criminal/fraud/sff/cases-telemarketing-fraud',
      70 => 'criminal/fraud/sff/recent-substantial-sentences',
      71 => 'criminal/fraud/sff/working-groups',
      7 => 'criminal/fraud/websites',
      8 => 'criminal/fraud/working-grps',
      9 => 'criminal/fraud/fcpa/docs',
      10 => 'criminal/fraud/fcpa/guidance',
      11 => 'criminal/fraud/fcpa/history',
      12 => 'criminal/fraud/fcpa/index',
      13 => 'criminal/fraud/fcpa/intlagree',
      14 => 'criminal/fraud/fcpa/opinion',
      15 => 'criminal/fraud/fcpa/othersites',
      16 => 'criminal/fraud/fcpa/review',
      17 => 'criminal/fraud/fcpa/statutes',
      18 => 'criminal/fraud/fcpa/summaries',
      19 => 'criminal/fraud/hcf/facts',
      20 => 'criminal/fraud/hcf/hcfac',
      21 => 'criminal/fraud/hcf/index',
      22 => 'criminal/fraud/hcf/statutes',
      23 => 'criminal/fraud/hcf/strike-force-operations',
    );

    $options = array('recurse' => FALSE);

    $arguments['source_parser_class'] = "CriminalFraudPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalFraudObtainTitlePage");
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('pluckSelector', array('div.headline', 1));
    $title->addMethod('pluckSelector', array('div.headline', 2));
    $title->addMethod('findSelectorAttribute', array("div.headline > img", "alt"));
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("title", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Fraud Section.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('criminal-fraud')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/criminal/fraud/sff/cases-accounting/arthrocare.html',
      '/criminal/fraud/sff/cases-accounting/christian-milton.html',
      '/criminal/fraud/sff/cases-accounting/christopher-p-garand.html',
      '/criminal/fraud/sff/cases-accounting/david-applegate.html',
      '/criminal/fraud/sff/cases-accounting/don-a-langford.html',
      '/criminal/fraud/sff/cases-accounting/elizabeth-monrad.html',
      '/criminal/fraud/sff/cases-accounting/gilbert-lundstrom.html',
      '/criminal/fraud/sff/cases-accounting/james-laphen.html',
      '/criminal/fraud/sff/cases-accounting/john-houldsworth.html',
      '/criminal/fraud/sff/cases-accounting/john-raffle.html',
      '/criminal/fraud/sff/cases-accounting/michael-baker.html',
      '/criminal/fraud/sff/cases-accounting/michael-gluk.html',
      '/criminal/fraud/sff/cases-accounting/richard-napier.html',
      '/criminal/fraud/sff/cases-accounting/robert-graham.html',
      '/criminal/fraud/sff/cases-accounting/ronald-e-ferguson.html',
      '/criminal/fraud/sff/cases-telemarketing-fraud/ian-thomas-mcculloch.html',
      '/criminal/fraud/sff/cases-telemarketing-fraud/osman-ban.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates .html files from /criminal-fraud to case nodes.
 *
 * @package doj_migration
 * @subpackage criminal/fraud
 */
class CriminalFraudCaseMigration extends NGJusticeHtmlToCaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    $arguments['organization'] = "criminal-fraud";

    // Define source directories.
    $source_dirs = array(
      'criminal/fraud/fcpa/cases',
      'criminal/fraud/sff',
      'criminal/fraud/sff/cases-accounting',
      'criminal/fraud/sff/cases-commodities-fraud',
      'criminal/fraud/sff/cases-insider-trading',
      'criminal/fraud/sff/cases-investment-fraud',
      'criminal/fraud/sff/cases-libor-fx',
      'criminal/fraud/sff/cases-market-manipulation',
      'criminal/fraud/sff/cases-mortgage-fraud',
      'criminal/fraud/sff/cases-procurement',
      'criminal/fraud/sff/cases-telemarketing-fraud',
    );

    $options = array('recurse' => TRUE);

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "CriminalFraudObtainTitleCase");
    $title->addMethod('pluckSelector', array('p[style="margin-bottom:10px;"] > b', 1));
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("title", 1));

    $arguments['obtainers_info'][] = $title;
    $arguments['source_parser_class'] = "CriminalFraudCaseSourceParser";
    $arguments['component_tid'] = 4271;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates cases from the Fraud Section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('criminal-fraud')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/criminal/fraud/fcpa/cases/york-int-corp.html',
      '/criminal/fraud/fcpa/cases/novon.html',
      '/criminal/fraud/sff/cases-accounting/index.html',
      '/criminal/fraud/sff/cases-commodities-fraud/index.html',
      '/criminal/fraud/sff/cases-insider-trading/index.html',
      '/criminal/fraud/sff/cases-investment-fraud/index.html',
      '/criminal/fraud/sff/cases-libor-fx/index.html',
      '/criminal/fraud/sff/cases-market-manipulation/index.html',
      '/criminal/fraud/sff/cases-mortgage-fraud/index.html',
      '/criminal/fraud/sff/cases-procurement/index.html',
      '/criminal/fraud/sff/cases-telemarketing-fraud/index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates binary files from /criminal-fraud.
 *
 * @package doj_migration
 * @subpackage criminal-fraud
 */
class CriminalFraudFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the criminal/fraud subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'criminal/fraud',
    );

    $dest_dir = 'public:///criminal-fraud/docs';
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
    return "criminal-fraud";
  }
}

/**
 * SourceParser for /criminal-fraud pages.
 *
 * @package doj_migration
 * @subpackage criminal-fraud
 */
class CriminalFraudPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-fraud pages.
 *
 * @package doj_migration
 * @subpackage criminal-fraud
 * @see Obtainer.api.php
 */
class CriminalFraudObtainTitlePage extends ObtainTitle {

}

/**
 * SourceParser for /criminal-fraud cases.
 *
 * @package doj_migration
 * @subpackage criminal-ceos
 */
class CriminalFraudCaseSourceParser  extends NGCaseSourceParser {

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {

    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /criminal-fraud cases.
 *
 * @package doj_migration
 * @subpackage criminal-fraud
 * @see Obtainer.api.php
 */
class CriminalFraudObtainTitleCase extends ObtainTitle {

}
