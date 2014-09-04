<?php

/**
 * @file
 * tribal.
 *
 * Defines migration classes for the Espanol section of justice.gov.
 */

/**
 * Migrates .html files from /spanish to page nodes.
 *
 * @package doj_migration
 * @subpackage espanol
 */
class EspanolPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'spanish',
      'spanish/topics',
      'spanish/testimony/2011',
      // Press release: The root of pr contains navigation pages.
      'spanish/pr',
      // Speeches: The root of speeches contain navigation pages.
      'spanish/speeches',
    );
    $options = array(
      'recurse' => FALSE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Spanish section into Espanol.');
    $this->addFieldMapping('og_group_ref')->defaultValue('espanol')->sourceMigration('Organization');
    $this->addFieldMapping('language')->defaultValue('es');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    module_load_include('inc', 'doj_migration', 'includes/HtmlCleanUp');
    // Cleanup the body from some spanish image banners.
    $row->body = HtmlCleanUp::removeElementsFromHtml($row->body, array(
      'div.bar-hdr',
      'div.banner_spanish',
    ));
  }
}


/**
 * Migrates  press_release .html files from /spanish to press_release nodes.
 *
 * @package doj_migration
 * @subpackage espanol
 */
class EspanolPressReleaseMigration extends JusticeHtmlToPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'spanish/pr/2010',
      'spanish/pr/2011',
      'spanish/pr/2012',
      'spanish/pr/2013',
      'spanish/pr/2014',
    );
    $options = array(
      'recurse' => TRUE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates Press Releases from the Spanish section into Espanol.');
    $this->addFieldMapping('og_group_ref')->defaultValue('espanol')->sourceMigration('Organization');
    $this->addFieldMapping('language')->defaultValue('es');

  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Get title from .presscontenttitle.
    $row->title = $this->sourceParser->queryPath->find('.presscontenttitle')->first()->text();
    $row->title = mb_strimwidth($row->title, 0, 255, "...");
    $row->title = html_entity_decode($row->title, ENT_QUOTES, 'UTF-8');

    // Get body from .presscontenttext.
    $row->body = $this->sourceParser->queryPath->find('.presscontenttext')->first()->html();
    $row->body = StringCleanUp::stripCmsLegacyMarkup($row->body);
    // Get pr number from .prnumber.
    $row->field_pr_number = $this->sourceParser->queryPath->find('.prnumber')->first()->text();
    // Get pr component from .prcomponent are these spanish translations?
    $row->field_pr_component = $this->sourceParser->queryPath->find('.prcomponent')->first()->text();
    // Build the terms for component vocabulary if they don't exist.
    module_load_include('inc', 'doj_migration', 'includes/doj_migration');
    $row->field_pr_component = doj_migrate_prepare_vocab_terms($row->field_pr_component, 'component');
    // Perhaps get .presscontentsubhdr
    $row->field_pr_subtitle = $this->sourceParser->queryPath->find('.presscontentsubhdr')->first()->text();

    // Process release_date from spanish, into numeric.
    $date_string = $this->sourceParser->queryPath->find('.presscontentdate .date')->first()->text();
    // Date_string looks like   miércoles, 28 de febrero de 2014.
    $row->field_pr_date = parent::dojMigrationESDateConvertWDMY($date_string);
  }


}


/**
 * Migrates binary files from /spanish.
 *
 * @package doj_migration
 * @subpackage espanol
 */
class EspanolFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates binary files from the spanish subdirectory.');
    $this->dependencies = array('Organization');

    // Match binary files only.
    $source_dirs = array(
      'spanish',
      'spanish/pr/2012/December',
      'spanish/pr/2014',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    // Sending them to spanish, the old location, instead of espanol so
    // redirects have a better chance of working.
    $dest_dir = 'public:///espanol/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "espanol";
  }
}


/**
 * Migrates  speeches .html files from /spanish to speech nodes.
 *
 * @package doj_migration
 * @subpackage espanol
 */
class EspanolSpeechMigration extends JusticeHtmlToSpeechSpanishMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'spanish/speeches/2010',
      'spanish/speeches/2011',
      'spanish/speeches/2012',
      'spanish/speeches/2013',
      'spanish/speeches/2014',
    );
    $options = array(
      'recurse' => TRUE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates Speeches from the Spanish section into Espanol.');
    $this->addFieldMapping('og_group_ref')->defaultValue('espanol')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Get title from .presscontenttitle.
    $row->title = $this->sourceParser->queryPath->find('.presscontenttitle')->first()->text();
    $row->title = mb_strimwidth($row->title, 0, 255, "...");
    $row->title = html_entity_decode($row->title, ENT_QUOTES, 'UTF-8');

    // Get body from .presscontenttext.
    $row->body = $this->sourceParser->queryPath->find('.presscontenttext')->first()->html();
    $row->body = StringCleanUp::stripCmsLegacyMarkup($row->body);

    // Remove common mso style string.
    $bad_string1 = 'style="margin: 0in 0in 0pt; text-indent: 0in; line-height: normal; mso-layout-grid-align: none; mso-outline-level: 1;"';
    $row->body = str_replace($bad_string1, '', $row->body);
  }
}
