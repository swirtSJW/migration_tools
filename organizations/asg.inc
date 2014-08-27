<?php

/**
 * @file
 * Office of Associate Attorney General ASG.
 *
 * Defines migration classes for the ASG section of justice.gov.
 */

/**
 * Migrates .html files from /asg to page nodes.
 *
 * @package doj_migration
 * @subpackage asg
 */
class AsgPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'asg',
      'asg/testimony/2009',
      'asg/testimony/2010',
      'asg/testimony',
      'asg/readingroom',

    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Office of the Associate Attorney General.');
    $this->addFieldMapping('og_group_ref')->defaultValue('aag')->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /asg.
 *
 * @package doj_migration
 * @subpackage asg
 */
class AsgFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the asg subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'asg',
      'asg/readingroom',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///asg/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "asg";
  }
}

/**
 * Migrates speeches from /asg.
 *
 * @package doj_migration
 * @subpackage asg
 */
class AsgSpeechMigration extends JusticeHtmlToSpeechMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'asg/speeches/2008',
      'asg/speeches/2009',
      'asg/speeches/2010',
    );

    $options = array(
      'recurse' => FALSE,
      'nomask' => '/(\.\.?|CVS|.*-spanish.html?)$/',
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Office of the Associate Attorney General.');
    $this->addFieldMapping('og_group_ref')->defaultValue('aag')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Get title from .presscontenttitle.
    $title = '';
    $title = $this->sourceParser->queryPath->find('.presscontenttitle')->first()->text();
    $title = mb_strimwidth($title, 0, 255, "...");
    $row->title = html_entity_decode($title, ENT_QUOTES, 'UTF-8');

    $body = '';
    $body = $this->sourceParser->queryPath->find('em')->remove();
    $body = $this->sourceParser->queryPath->find('.presscontenttext')->first()->html();

    $body = parent::dojMigrateStripCmsLegacyMarkup($body);

    $row->body = $body;

    $row->field_speech_date = $this->sourceParser->queryPath->find('.speechdate')->text();

    $row->field_speech_component = "Office of the Associate Attorney General";

    module_load_include('inc', 'doj_migration', 'includes/doj_migration');
    $location = $this->sourceParser->queryPath->find('.speechlocation')->text();

    if (module_exists('geocoder') && $location !== '') {
      // Note that calling this too many times (as in very large migrations)
      // may exceed the API request limit for geocoder's source data.
      $point = geocoder('google', $location);
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
}

/**
 * Migrates spanish language speeches from /asg.
 *
 * @package doj_migration
 * @subpackage asg
 */
class AsgSpeechEspanolMigration extends JusticeHtmlToSpeechMigration {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'field_speech_body' => t('Body'),
    );

    // Define source directories.
    $source_dirs = array(
      'asg/speeches/2008',
      'asg/speeches/2009',
      'asg/speeches/2010',
    );

    $options = array('recurse' => FALSE);

    // Match spanish .htm, .html files only.
    $regex = '/.*spanish\.html?$/';

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_dirs, $regex, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Office of the Associate Attorney General.');
    $this->addFieldMapping('language')->defaultValue('es');
    $this->addFieldMapping('og_group_ref')->defaultValue('aag')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Get title from .presscontenttitle.
    $title = '';
    $title = $this->sourceParser->queryPath->find('.presscontenttitle')->first()->text();
    $title = mb_strimwidth($title, 0, 255, "...");
    $row->title = html_entity_decode($title, ENT_QUOTES, 'UTF-8');

    $body = '';
    $body = $this->sourceParser->queryPath->find('em')->remove();
    $body = $this->sourceParser->queryPath->find('.presscontenttext')->first()->html();

    $body = parent::dojMigrateStripCmsLegacyMarkup($body);

    $row->body = $body;

    $date = '';
    $date = $this->sourceParser->queryPath->find('.speechdate')->text();
    $row->field_speech_date = parent::dojMigrationESDateConvertWDMY($date);

    $row->field_speech_component = "Office of the Associate Attorney General";

    module_load_include('inc', 'doj_migration', 'includes/doj_migration');
    $location = $this->sourceParser->queryPath->find('.speechlocation')->text();

    if (module_exists('geocoder') && $location !== '') {
      // Note that calling this too many times (as in very large migrations)
      // may exceed the API request limit for geocoder's source data.
      $point = geocoder('google', $location);
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
}
