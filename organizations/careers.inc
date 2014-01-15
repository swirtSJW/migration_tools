<?php

/**
 * @file
 * Defines migration classes for the Careers section of justice.gov.
 */

/**
 * The Universally Unique Identifier for the Careers Organic Group.
 */
const CAREERS_OG_UUID = '8a7892d3-a273-4736-acb6-dd0246c51180';

/**
 * Abstract intermediate class holding common settings.
 */
abstract class CareersMigration extends JusticeMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct() {

    // We group all sub classes of this super class under the 'careers' group,
    // and indicate that it should appear after the 'default' group in UI.
    parent::__construct(MigrateGroup::getInstance('careers', array()));
  }
}


/**
 * Migrates .html files from careers/legal -> page nodes.
 */
class CareersPageMigration extends CareersMigration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Migrates pages from Careers section.');

    // Match .htm, .html files only.
    $regex = '/.*\.htm(l)?/';

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'page_body' => t('Body')
    );

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('page');

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'uid',
      'title',
    ));

    // Define non-simple field mappings.
    $this->addFieldMapping('page_body', 'body')
      ->arguments(array('format' => 'wysiwyg'));

  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    // Extract the relative legacy path.
    $row->legacy_path = substr($row->sourceid, 1);

    // Set to admin for now.
    $row->uid = 1;
  }
}

/**
 * Migrates jobs from careers/legal/jobs -> vacancy_announcement.
 */
class CareersJobMigration extends CareersMigration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Migrates jobs from Careers legal subsection.');

    // Define the directories containing files to be migrated.
    $relative_dirs = array('careers/legal/jobs');
    $absolute_dirs = $this->prependJusticeFilepath($relative_dirs);

    // Match .htm, .html files only.
    $regex = '/.*\.htm(l)?/';

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'body' => t('Body'),
      'field_qualifications' => t('Qualifications'),
      'field_travel' => t('Travel'),
      'field_salary' => t('Salary'),
      'field_responsibilities' => t('Responsibilities'),
      'field_job_id' => t('Job ID'),
    );

    // $list_files will provide migrate with a list of alls file to be migrated.
    // $item_file provides methods for deriving a unique file identifier.
    // $this->source define the essential data source from which to migrate.
    $list_files = new MigrateListFiles($absolute_dirs, $this->base_dir, $regex);
    $item_file = new MigrateItemFile($this->base_dir);
    $this->source = new MigrateSourceList($list_files, $item_file, $source_fields);

    // A map of source HTML filename -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
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
      'field_qualifications',
      'field_travel',
      'field_salary',
      'field_responsibilities',
      'field_job_id',
    ));

    // Define non-simple field mappings.
    $this->addFieldMapping('field_email', 'field_email')->separator('|');
    $this->addFieldMapping('body', 'body')
      ->arguments(array('format' => 'wysiwyg'));

    // Set the Organic Group to which this content belongs. Get the data
    // here so that it isn't fetched per $row in prepare().
    list($careers_og_eid) = array_values(entity_get_id_by_uuid('node', array(CAREERS_OG_UUID)));
    $this->addFieldMapping('og_group_ref')->defaultValue($careers_og_eid);

    // Create redirects using relative, legacy path of static file.
    $this->addFieldMapping('migrate_redirects', 'legacy_path')
      ->defaultValue('MigrateRedirectEntity');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    // Extract the relative legacy path.
    $row->legacy_path = substr($row->sourceid, 1);

    // Set to admin for now.
    $row->uid = 1;

    // Create a new SourceParser to handle HTML content.
    $source_parser = new SourceParser($row->legacy_path, $row->filedata);

    $row->title = $source_parser->getTitle();
    $row->field_travel = $source_parser->extractInlineTitle('Travel');
    $row->field_qualifications = $source_parser->extractInlineTitle(array('Required qualifications','Required and Preferred Qualifications'));
    $row->field_salary = $source_parser->extractInlineTitle('Salary Information');
    $row->field_responsibilities = $source_parser->extractInlineTitle('Responsibilities and Opportunity Offered');
    $row->field_email = $source_parser->getEmailAddresses();

    // Extract Job ID from title.
    if (preg_match('|[A-Z0-9]{1,4}(-[A-Z0-9()\/]+){2,}|', $row->title, $matches)) {
      if (isset($matches[0])) {
        $row->field_job_id =$matches[0];
      }
    }
    else {
      $this->queueMessage(t('No Job ID was found for @sourceid.', array('@sourceid' => $row->sourceid)));
    }

    $row->body = $source_parser->getBody();
  }
}

/**
 * Migrates files (*.pdf) from the careers subdirectory.
 */
class CareersFileMigration extends CareersMigration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Migrates files (*.pdf) from the careers subdirectory.');

    // Match .pdf files only.
    $source_dirs = array('careers');
    $regex = '/.*\.pdf/';
    $dest_dir = 'public://careers/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    $this->prepareFileMigration($source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $this->prepareFileRow($row);
  }

}
