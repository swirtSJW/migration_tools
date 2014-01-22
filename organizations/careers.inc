<?php

/**
 * @file
 * Defines migration classes for the Careers section of justice.gov.
 */

/**
 * The Universally Unique Identifier for the Careers Organic Group.
 */
const CAREERS_OG_UUID = 'b6437e3d-02de-4d2f-b4e9-d5002e9a4d12';


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
    $this->addGroupReferenceMapping(CAREERS_OG_UUID);
  }
}

/**
 * Migrates .html files from careers/legal/jobs to vacancy_announcement nodes.
 */
class CareersJobMigration extends JusticeFileSourceMigration {

  public function __construct() {
    // Add this migration to the 'careers' group.
    $group = MigrateGroup::getInstance('careers', array());

    $this->description = t('Migrates jobs from Careers legal subsection.');

    // Define the relative directories containing files to be migrated.
    $source_dirs = array('careers/legal/jobs');

    // Match .htm, .html files only.
    $regex = '/.*\.htm(l)?/';

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
    parent::__construct($group, $source_fields, $source_dirs, $regex);

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

    $this->addGroupReferenceMapping(CAREERS_OG_UUID);
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
