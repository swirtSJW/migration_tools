<?php

/**
 * @file
 * Defines migration classes for the Careers section of justice.gov.
 */

/**
 * Migrates .html files from /careers to page nodes.
 *
 * @package doj_migration
 * @subpackage careers
 */
class CareersPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'careers',
      'careers/veterans',
      'careers/legal',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Careers section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('careers')->sourceMigration('Organization');
  }
}

/**
 * Class JusticeVacancyAnnouncementMigration.
 *
 * @package doj_migration
 * @subpackage careers
 */
class JusticeVacancyAnnouncementMigration extends JusticeFileSourceMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $source_dirs, $regex, $options = array(), MigrateContentParser $parser = NULL) {
    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'field_va_body' => t('Body'),
      'field_qualifications' => t('Qualifications'),
      'field_va_travel' => t('Travel'),
      'field_va_salary' => t('Salary'),
      'field_va_responsibilities' => t('Responsibilities'),
      'field_va_job_id' => t('Job ID'),
    );

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options, $parser);

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

    // The destination is the vacancy_announcement content type.
    $this->destination = new MigrateDestinationNode('vacancy_announcement');

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'uid',
      'title',
      'field_va_salary',
      'field_va_job_id',
    ));

    // Define non-simple field mappings.
    $this->addFieldMapping('field_va_email', 'field_va_email')->separator('|');
    $this->addFieldMapping('field_va_body', 'field_va_body');
    $this->addFieldMapping('field_va_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_va_qualifications', 'field_va_qualifications');
    $this->addFieldMapping('field_va_qualifications:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_va_travel', 'field_va_travel');
    $this->addFieldMapping('field_va_travel:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_va_responsibilities', 'field_va_responsibilities');
    $this->addFieldMapping('field_va_responsibilities:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_va_practice_area', 'field_va_practice_area');
    $this->addFieldMapping('field_va_hiring_component', 'field_va_hiring_component');
    $this->addFieldMapping('field_va_vacancy_type')->defaultValue('employment');
    $this->addFieldMapping('field_va_state' ,'state');

    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');
    $this->addFieldMapping('og_group_ref')->defaultValue('careers')->sourceMigration('Organization');
  }
}

/**
 * Migrates .html files from careers/legal/jobs to vacancy_announcement nodes.
 *
 * @package doj_migration
 * @subpackage careers
 */
class CareersJobMigration extends JusticeVacancyAnnouncementMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define the relative directories containing files to be migrated.
    $source_dirs = array('careers/legal/jobs');

    // Match .htm, .html files only.
    $regex = '/.*\.htm(l)?/';

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_dirs, $regex);
    $this->description = t('Migrates jobs from Careers legal subsection.');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Create a new SourceParser to handle HTML content.
    $source_parser = new CareersSourceParser($row->legacy_path, $row->filedata);

    $row->title = $source_parser->getTitle();
    $row->field_va_travel = $source_parser->extractInlineTitle('Travel');
    $row->field_va_qualifications = $source_parser->extractInlineTitle(array('Required qualifications', 'Required and Preferred Qualifications'));
    $row->field_va_salary = $source_parser->extractInlineTitle('Salary Information');
    $row->field_va_responsibilities = $source_parser->extractInlineTitle('Responsibilities and Opportunity Offered');
    $row->field_va_email = $source_parser->getEmailAddresses();
    $row->state = $source_parser->getUsState();
    $row->field_va_practice_area = $this->getPracticeArea($source_parser);
    $row->field_va_hiring_component = $this->getHiringComponent($source_parser);

    // Extract Job ID from title.
    if (preg_match('|[A-Z0-9]{1,4}(-[A-Z0-9()\/]+){2,}|', $row->title, $matches)) {
      if (isset($matches[0])) {
        $row->field_va_job_id = $matches[0];
      }
    }
    else {
      $this->queueMessage(t('No Job ID was found for @fileid.', array('@fileid' => $row->fileid)));
    }

    $row->field_va_body = $source_parser->getBody();
  }

  /**
   * Gets value for practice area field from HTML.
   */
  public function getPracticeArea($parser) {
    $practice_areas_blob = file_get_contents(drupal_get_path('module', 'doj_migration') . '/sources/careers-practice-areas.txt');
    $practice_areas = explode("\n", $practice_areas_blob);
    $page_text = $parser->queryPath->top()->html();
    foreach ($practice_areas as $practice_area) {
      list($key, $label) = explode('|', $practice_area);
      if (strpos($page_text, $label) !== FALSE) {
        return $key;
      }
    }
    return NULL;
  }

  /**
   * Gets value for hiring component field from HTML.
   */
  public function getHiringComponent($parser) {
    $hiring_comp_blob = file_get_contents(drupal_get_path('module', 'doj_migration') . '/sources/careers-hiring-components.txt');
    $hiring_comps = explode("\n", $hiring_comp_blob);
    $page_text = $parser->queryPath->top()->html();
    foreach ($hiring_comps as $hiring_comp) {
      list($key, $label) = explode('|', $hiring_comp);
      if (strpos($page_text, $label) !== FALSE) {
        return $key;
      }
    }
    return NULL;
  }
}

/**
 * Migrates internships from [] to vacancy_announcement nodes.
 *
 * @package doj_migration
 * @subpackage careers
 * @see CareersInternshipParser
 */
class CareersInternshipMigration extends JusticeVacancyAnnouncementMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define the relative directories containing files to be migrated.
    $source_dirs = array('careers/legal');

    // Match .htm, .html files only.
    // Expected file matches are:
    // careers/legal/volunteer-opp.html.
    // careers/legal/volunteer-opp-usao.html.
    // careers/legal/volunteer-opp-summer.html.
    // careers/legal/volunteer-opp-usao-summer.html.
    $regex = '/volunteer-opp.*\.htm(l)?/';
    $options = array();

    // Instantiate the parser object that will parse a single HTML file into
    // chunks that will be translated into internship nodes.
    $parser = new CareersChunkParser();

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_dirs, $regex, $options, $parser);
    $this->description = t('Migrates jobs from Careers legal subsection.');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Create a new SourceParser to handle HTML content.
    $source_parser = new CareersSourceParser($row->legacy_path, $row->filedata);

    // Extract title.
    $title_element = $source_parser->queryPath->find('td:first a');
    $row->title = $title_element->text();
    $title_element->remove();

    $row->field_va_qualifications = $this->extractTableRow($source_parser, 'Qualifications:');
    $row->field_va_email = $this->extractTableRow($source_parser, 'Email:');

    // Assign leftovers to body field.
    $row->field_va_body = $source_parser->queryPath->find('table')->html();
  }

  /**
   * Extracts the value of a row and removes from a table.
   */
  public function extractTableRow($parser, $label) {
    $element = $parser->queryPath
      ->xpath("//td[contains(text(), '$label')]")
      ->next('td');
    $value = $element->text();
    $element->parents('tr')->remove();

    return $value;
  }
}

/**
 * Parses a single html file into a list of internship rows.
 *
 * @package doj_migration
 * @subpackage careers
 * @see CareersInternshipMigration
 */
class CareersChunkParser extends JusticeChunkParser {

  /**
   * Sets $this->chunks.
   */
  public function setChunks() {
    // Each <table> in #rightColORAM after the first is an internship.
    $this->chunks = $this->sourceParser->queryPath->find('#rightColOARM table:gt(1)')->has('a');
  }

  /**
   * {@inheritdoc}
   */
  public function getChunkIDs() {
    $chunks = $this->getChunks();

    $ids = array();
    foreach ($chunks as $chunk) {
      $ids[] = $chunk->find('a:first')->attr('name');
    }
    return $ids;
  }

  /**
   * {@inheritdoc}
   */
  public function getChunk($chunk_id) {
    $chunks = $this->getChunks();
    $chunk = $chunks->find('a[name=" ' . $chunk_id . '"')->parents('table');

    return $chunk;
  }
}

/**
 * Migrates files (*.pdf) from /careers.
 *
 * @package doj_migration
 * @subpackage careers
 */
class CareersFileMigration extends JusticeBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates files (*.pdf) from the careers subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array('careers');
    $regex = '/.*\.pdf/';
    $dest_dir = 'public://careers/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }
}
