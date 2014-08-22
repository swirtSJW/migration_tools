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
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Office of the Associate Attorney General.');
    $this->addFieldMapping('og_group_ref')->defaultValue('asg')->sourceMigration('Organization');
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
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Office of the Associate Attorney General.');
    $this->addFieldMapping('og_group_ref')->defaultValue('asg')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row, SourceParser $source_parser = NULL) {
    // Create a new SourceParser to handle HTML content.
    if (!$source_parser) {
      $source_parser = new SourceParser($row->url_path, $row->filedata);
    }

    // Get title from .presscontenttitle.
    $row->title = $source_parser->queryPath->find('.presscontenttitle')->first()->text();
    $row->title = mb_strimwidth($row->title, 0, 255, "...");
    $row->title = html_entity_decode($row->title, ENT_QUOTES, 'UTF-8');

    $body = '';
    $body = $source_parser->queryPath->find('em')->remove();
    $body = $source_parser->queryPath->find('.presscontenttext')->first()->html();

    $body = doj_migrate_strip_cmsopa_legacy_markup($body);

    $row->field_speech_body = $body;

    $row->field_speech_date = $source_parser->queryPath->find('.speechdate')->text();

    // This data is a mess.
    $location = '';
    $city = '';
    $state = '';

    $location = strtolower($source_parser->queryPath->find('.speechlocation')->text());

    list($city, $state) = explode(', ', $location);

    $state = strtoupper(strtr($state, array('.' => '')));

    if (strlen($state) > 2) {
      switch ($state) {
        case 'MASS':
          $state = 'MA';
          break;

        case 'WASH':
          $state = 'WA';
          break;

        case 'MICH':
          $state = 'MI';
          break;

        case 'MINN':
          $state = 'MN';
          break;

      }
    }

    $row->state = $state;
    $row->city = ucwords($city);
    parent::prepareRow($row, $source_parser);
  }
}

/**
 * Migrates spanish language speeches from /asg.
 *
 * @package doj_migration
 * @subpackage asg
 */
class AsgSpeechEspanolMigration extends JusticeFileSourceMigration {
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
    $regex = '/.*spanish\.htm(l)?$/';

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('speech');

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'uid',
      'title',
    ));

    // A map of source HTML filename -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'fileid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Define non-simple field mappings.
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');

    // Create redirects using relative, legacy path of static file.
    $this->addFieldMapping('migrate_redirects', 'legacy_path')->separator('|');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_speech_date', 'field_speech_date');
    $this->addFieldMapping('field_speech_body', 'field_speech_body');
    $this->addFieldMapping('field_speech_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_speech_location', 'country')->defaultValue('US');
    $this->addFieldMapping('field_speech_location:locality', 'city');
    $this->addFieldMapping('field_speech_location:administrative_area', 'state');
    $this->addFieldMapping('field_speech_image', 'field_speech_image');
    $this->addFieldMapping('field_speech_attachment', 'field_speech_attachment');
    $this->addFieldMapping('field_speech_topic', 'field_speech_topic');
    $this->addFieldMapping('field_speech_component', 'field_speech_component');
    $this->addFieldMapping('language')->defaultValue('es');
    $this->addFieldMapping('og_group_ref')->defaultValue('asg')->sourceMigration('Organization');

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row, SourceParser $source_parser = NULL) {
    // Create a new SourceParser to handle HTML content.
    if (!$source_parser) {
      $source_parser = new SourceParser($row->url_path, $row->filedata);
    }

    // Get title from .presscontenttitle.
    $row->title = $source_parser->queryPath->find('.presscontenttitle')->first()->text();
    $row->title = mb_strimwidth($row->title, 0, 255, "...");
    $row->title = html_entity_decode($row->title, ENT_QUOTES, 'UTF-8');

    $body = '';
    $body = $source_parser->queryPath->find('em')->remove();
    $body = $source_parser->queryPath->find('.presscontenttext')->first()->html();

    $body = doj_migrate_strip_cmsopa_legacy_markup($body);

    $row->field_speech_body = $body;

    $date = '';
    $date = $source_parser->queryPath->find('.speechdate')->text();
    $date = str_ireplace('de', '', $date);

    $fmt = new IntlDateFormatter(
      'es_ES',
      IntlDateFormatter::FULL,
      IntlDateFormatter::FULL,
      'America/New_York',
      IntlDateFormatter::GREGORIAN,
      'eeee, dd MMMM y'
    );

    $row->field_speech_date = $fmt->parse($date);

    // This data is a mess.
    $location = '';
    $city = '';
    $state = '';

    $location = strtolower($source_parser->queryPath->find('.speechlocation')->text());

    list($city, $state) = explode(', ', $location);

    $state = strtoupper(strtr($state, array('.' => '')));

    $row->state = $state;
    $row->city = ucwords($city);
    JusticeHtmlToSpeechMigration::prepareRow($row, $source_parser);
    JusticeHtmlToNodeMigration::generateLegacyPath($row);
    parent::prepareRow($row, $source_parser);
  }

  /**
   * {@inheritdoc}
   */
  public function isDuplicateByRedirect($row) {
    JusticeHtmlToSpeechMigration::isDuplicateByRedirect($row);
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    JusticeHtmlToSpeechMigration::prepare($entity, $row);
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    JusticeHtmlToSpeechMigration::complete($entity, $row);
  }
}
