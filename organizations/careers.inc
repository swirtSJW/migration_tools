<?php

/**
 * @file
 * Defines migration classes for the Careers section of justice.gov.
 */

/**
 * Migrates .html files from /careers to page nodes.
 */
class CareersPageMigration extends JusticeHtmlToPageMigration {

  public function __construct() {
    // Add this migration to the 'careers' group.
    $group = MigrateGroup::getInstance('careers', array());

    // Define source directories.
    $source_dirs = array(
      'careers',
      'careers/veterans',
      'careers/legal',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($group, $source_dirs, $options);

    $this->description = t('Migrates pages from Careers section.');
    $this->addFieldMapping('og_group_ref', 'abbreviation')->sourceMigration('careers');
  }
}

/**
 *
 */
class JusticeVacancyAnnouncementMigration extends JusticeFileSourceMigration {

  public function __construct($source_dirs, $regex, $options = array(), MigrateContentParser $parser = NULL) {
    // Add this migration to the 'careers' group.
    $group = MigrateGroup::getInstance('careers', array());

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
    parent::__construct($group, $source_fields, $source_dirs, $regex, $options, $parser);

    // A map of source HTML filename -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'fileid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        )
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
    $this->addFieldMapping('field_va_body', 'field_va_body')->arguments(array('format' => 'wysiwyg'));
    $this->addFieldMapping('field_va_qualifications', 'field_va_qualifications')->arguments(array('format' => 'wysiwyg'));
    $this->addFieldMapping('field_va_travel', 'field_va_travel')->arguments(array('format' => 'wysiwyg'));
    $this->addFieldMapping('field_va_responsibilties', 'field_va_responsibilties')->arguments(array('format' => 'wysiwyg'));

    // @todo Add practice area.
    // @todo Add hiring component.
    // @todo Add state.

    $this->addFieldMapping('og_group_ref', 'abbreviation')->sourceMigration('careers');
  }
}

/**
 * Migrates .html files from careers/legal/jobs to vacancy_announcement nodes.
 */
class CareersJobMigration extends JusticeVacancyAnnouncementMigration {

  public function __construct() {

    // Define the relative directories containing files to be migrated.
    $source_dirs = array('careers/legal/jobs');

    // Match .htm, .html files only.
    $regex = '/.*\.htm(l)?/';

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($source_dirs, $regex);
    $this->description = t('Migrates jobs from Careers legal subsection.');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Create a new SourceParser to handle HTML content.
    $source_parser = new SourceParser($row->legacy_path, $row->filedata);

    $row->title = $source_parser->getTitle();
    $row->field_va_travel = $source_parser->extractInlineTitle('Travel');
    $row->field_va_qualifications = $source_parser->extractInlineTitle(array('Required qualifications','Required and Preferred Qualifications'));
    $row->field_va_salary = $source_parser->extractInlineTitle('Salary Information');
    $row->field_va_responsibilities = $source_parser->extractInlineTitle('Responsibilities and Opportunity Offered');
    $row->field_va_email = $source_parser->getEmailAddresses();

    // Extract Job ID from title.
    if (preg_match('|[A-Z0-9]{1,4}(-[A-Z0-9()\/]+){2,}|', $row->title, $matches)) {
      if (isset($matches[0])) {
        $row->field_va_job_id =$matches[0];
      }
    }
    else {
      $this->queueMessage(t('No Job ID was found for @fileid.', array('@fileid' => $row->fileid)));
    }

    $row->field_va_body = $source_parser->getBody();

    // @todo Add practice area.
    // @todo Add hiring component.
    // @todo Add state.
  }
}

/**
 * Migrates internships from [] to vacancy_announcement nodes.
 *
 * @see CareersInternshipParser
 */
class CareersInternshipMigration extends JusticeVacancyAnnouncementMigration {
  public function __construct() {

    // Define the relative directories containing files to be migrated.
    $source_dirs = array('careers/legal');

    // Expected file matches are:
    // careers/legal/volunteer-opp.html
    // careers/legal/volunteer-opp-usao.html
    // careers/legal/volunteer-opp-summer.html
    // careers/legal/volunteer-opp-usao-summer.html

    // Match .htm, .html files only.
    $regex = '/volunteer-opp.*\.htm(l)?/';
    $options = array();

    // Instantiate the parser object that will parse a single HTML file into
    // chunks that will be translated into internship nodes.
    $parser = new CareersInternshipParser();
    //$parser = NULL;

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($source_dirs, $regex, $options, $parser);
    $this->description = t('Migrates jobs from Careers legal subsection.');
  }
}

/**
 * Parses a single html file into a list of internship rows.
 *
 * @see CareersInternshipMigration
 */
class CareersInternshipParser extends JusticeChunkParser {

  public function setChunks() {
    // Each <table> in #rightColORAM after the first is an internship.
   $this->chunks = $this->sourceParser->qp->find('#rightColOARM table:gt(1)')->has('a');
  }

  public function getChunkIDs() {
    $chunks = $this->getChunks();

    $ids = array();
    foreach ($chunks as $chunk) {
      $ids[] = $chunk->find('a:first')->attr('name');
    }
    return $ids;
  }

  public function getChunk($id) {
    $chunks = $this->getChunks();
    $chunk = $chunks->find('a[name=" '. $id. '"')->parents('table');

    return $chunk;
  }
}

/**
 * Migrates files (*.pdf) from /careers.
 */
class CareersFileMigration extends JusticeBinaryFileMigration {

  public function __construct() {
    // Add this migration to the 'careers' group.
    $group = MigrateGroup::getInstance('careers', array());
    $this->description = t('Migrates files (*.pdf) from the careers subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array('careers');
    $regex = '/.*\.pdf/';
    $dest_dir = 'public://careers/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($group, $source_dirs, $dest_dir, $regex);
  }
}
