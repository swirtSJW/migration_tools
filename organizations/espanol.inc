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
  public function prepareRow($row, SourceParser $source_parser = NULL) {
    parent::prepareRow($row);
    // Create a new SourceParser to handle HTML content.
    if (!$source_parser) {
      $source_parser = new SourceParser($row->url_path, $row->filedata);
    }

    // Get title from .presscontenttitle.
    $row->title = $source_parser->queryPath->find('.presscontenttitle')->first()->text();
    $row->title = mb_strimwidth($row->title, 0, 255, "...");
    $row->title = html_entity_decode($row->title, ENT_QUOTES, 'UTF-8');

    // Get body from .presscontenttext.
    $row->body = $source_parser->queryPath->find('.presscontenttext')->first()->html();
    // Get pr number from .prnumber.
    $row->field_pr_number = $source_parser->queryPath->find('.prnumber')->first()->text();
    // Get pr component from .prcomponent are these spanish translations?
    $row->field_pr_component = $source_parser->queryPath->find('.prcomponent')->first()->text();
    // Build the terms for component vocabulary if they don't exist.
    module_load_include('inc', 'doj_migration', 'includes/doj_migration');
    $row->field_pr_component = doj_migrate_prepare_vocab_terms($row->field_pr_component, 'component');
    // Perhaps get .presscontentsubhdr
    $row->field_pr_subtitle = $source_parser->queryPath->find('.presscontentsubhdr')->first()->text();

    // Process release_date from spanish, into numeric.
    $date_string = $source_parser->queryPath->find('.presscontentdate .date')->first()->text();
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
class EspanolSpeechMigration extends JusticeHtmlToSpeechMigration {

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
    $this->addFieldMapping('language')->defaultValue('es');

    $this->addFieldMapping('field_speech_location', 'country')->defaultValue('US');
    $this->addFieldMapping('field_speech_location:locality', 'city');
    $this->addFieldMapping('field_speech_location:administrative_area', 'state');
  }


  /**
   * {@inheritdoc}
   */
  public function prepareRow($row, SourceParser $source_parser = NULL) {
    parent::prepareRow($row);
    // Process the date.
    // Format: < div class="date">jueves, 19 de junio de 2014</div>
    // Create a new SourceParser to handle HTML content.
    if (!$source_parser) {
      $source_parser = new SourceParser($row->url_path, $row->filedata);
    }

    // Get title from .presscontenttitle.
    $row->title = $source_parser->queryPath->find('.presscontenttitle')->first()->text();
    $row->title = mb_strimwidth($row->title, 0, 255, "...");
    $row->title = html_entity_decode($row->title, ENT_QUOTES, 'UTF-8');

    // Get body from .presscontenttext.
    $row->body = $source_parser->queryPath->find('.presscontenttext')->first()->html();
    // Get the location from .speechlocation.
    $row->field_speech_location = trim($source_parser->queryPath->find('.speechlocation')->first()->text());

    // Geocode the location and parse into structured data for migration.
    // Geocoder module is not an explicit dependency because most migrations
    // do not rely on it. It should be disabled after use.
    if (!empty($row->field_speech_location)) {
      if ($row->field_speech_location == 'Washington, D.C.') {
        // The most common entry, so skip geocoding.
        $row->city = 'Washington';
        $row->state = 'DC';
      }
      elseif (module_exists('geocoder')) {
        // Note that calling this too many times (as in very large migrations)
        // may exceed the API request limit for geocoder's source data.
        $point = geocoder('google', $row->field_speech_location);
        module_load_include('inc', 'doj_migration', 'includes/doj_migration');
        $address = doj_migrate_convert_geocoded_point_to_address($point);

        if ($address) {
          $row->city = $address['locality'];
          $row->state = $address['administrative_area_level_1'];
          $row->country = $address['country'];
        }
        else {
          $this->queueMessage(t('Could not look up speech location because geocoder returned nothing The API request limit may have been exceeded.'));
        }
      }
      else {
        $this->queueMessage(t('Could not look up speech location because geocoder is not installed.'));
      }
    }

    // Process release_date from spanish, into numeric.
    $date_string = $source_parser->queryPath->find('.speechdate')->first()->text();
    // Date_string looks like   miércoles, 28 de febrero de 2014.
    $row->field_speech_date = parent::dojMigrationESDateConvertWDMY($date_string);
  }
}
