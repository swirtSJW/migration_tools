<?php

/**
 * @file
 * Defines migration classes for the Careers section of justice.gov.
 */

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
 * Migrates jobs from careers/legal/jobs -> vacancy_announcement.
 */
class CareersJobsMigration extends CareersMigration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Migrates jobs from Careers legal subsection.');

    // Define the directories containing files to be migrated.
    $this->base_dir = $this->base_dir;
    $relative_dirs = array('careers/legal/jobs');
    $absolute_dirs = $this->prependJusticeFilepath($relative_dirs);

    // Match .htm, .html files only.
    $regex = '/.*\.htm(l)?/';

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'body' => t('Body')
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

    // Field mappings.
    $this->addFieldMapping('uid', 'uid');
    $this->addFieldMapping('title', 'title');

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'field_qualifications',
      'field_travel',
      'field_salary',
      'field_responsibilities',
    ));

    $this->addFieldMapping('field_email', 'field_email')->separator('|');

    $this->addFieldMapping('body', 'body')
      ->arguments(array('format' => 'wysiwyg'));
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

    $row->body = $source_parser->getBody();
  }
}
