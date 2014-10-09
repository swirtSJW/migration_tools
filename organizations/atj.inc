<?php

/**
 * @file
 * Access to Justice Initiative ATJ.
 *
 * Defines migration classes for the ATJ section of justice.gov.
 */

/**
 * Migrates .html files from /atj to page nodes.
 *
 * @package doj_migration
 * @subpackage atj
 */
class AtjPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'atj',
    );
    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Access to Justice Initiative ATJ section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('atj')->sourceMigration('Organization');
  }
}


/**
 * Migrates non-image files from the /atj.
 *
 * @package doj_migration
 * @subpackage atj
 */
class AtjFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the atj subdirectory.');
    $this->dependencies = array('Organization');

    // Match non-image files.
    $source_dirs = array(
      'atj',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx|swf|flv)/';
    $dest_dir = 'public:///atj/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return 'atj';
  }
}


/**
 * Class AtjPublicationMigration.
 *
 * @package doj_migration
 */
class AtjPublicationMigration extends JusticeBaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->dependencies = array('Organization');

    global $databases;
    $iso_db_name = $databases['iso']['default']['database'];

    $query = db_select("$iso_db_name.atj_idp", 'c')
      ->fields('c', array(
        'id',
        'title',
        'url',
        'author',
        'year',
        'funding_component',
        'description',
      ));

    // A map of source MySQL row -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'int',
          'length' => 11,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceSQL($query);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('publication');

    // Field Mappings.
    $this->addFieldMapping('og_group_ref')->defaultValue('atj')->sourceMigration('Organization');

    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('field_publication_description', 'description');
    $this->addFieldMapping('field_publication_description:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_publication_url', 'url');
    $this->addFieldMapping('field_publication_author', 'author');

    // The source funding_component contains some values that are present in
    // the vocabulary 'Component' and some that in the vocabulary
    // 'Non DOJ Organization'. So we are going to pass it in twice and prevent
    // new terms from getting created.
    // The components and non_doj_organization terms should already exist.
    $this->addFieldMapping('field_publication_component', 'funding_component')->separator(";");
    $this->addFieldMapping('field_publication_component:create_term')->defaultValue(FALSE);
    $this->addFieldMapping('field_publication_non_doj_org', 'funding_component')->separator(";");
    $this->addFieldMapping('field_publication_non_doj_org:create_term')->defaultValue(FALSE);

    $this->addFieldMapping('field_publication_date', 'year');

    $this->addSimpleMappings(array("uid", "status"));
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);
    $row->uid = 1;
    $row->status = 1;
    $row->year = "{$row->year}/1/1";

    // There are some multi-values on funding_component separated by '/' so will
    // need to replace the / with a ; to make sure to get them all parsed.
    $row->funding_component = str_replace('/', ';', $row->funding_component);

    // Remove html tags from title.
    $row->title = strip_tags($row->title);

    // Normalize the funding component names.
    $funding_norms = array(
      'Access to Justice Initiative' => 'Access to Justice',
      'Bureau of Justice Assistance' => 'Bureau of Justice Assistance (OJP)',
      'Bureau of Justice Statistics' => 'Bureau of Justice Statistics (OJP)',
      'The Bureau of Justice Statistics' => 'Bureau of Justice Statistics (OJP)',
      'The Government Accountability Office' => 'Government Accountability Office',
      'National Institute of Corrections (Federal Bureau of Prisons)' => 'National Institute of Corrections (FBOP)',
      'National Institute of Justice' => 'National Institute of Justice (OJP)',
      'National Institute Justice' => 'National Institute of Justice (OJP)',
      'Office of Juvenile Justice and Delinquency Prevention' => 'Office of Juvenile Justice and Delinquency Prevention (OJJDP)',
      'The Office of Justice Programs' => 'Office of Justice Programs',
      'The Law Enforcement Assistance Administration' => 'Law Enforcement Assistance Administration',
    );

    // Loop through each funding component.
    $components = explode(';', $row->funding_component);
    foreach ($components as &$component) {
      $component = trim($component);
      // Normalize.
      $component = (!empty($funding_norms[$component])) ? $funding_norms[$component] : $component;
    }
    unset($component);
    // Reassemble.
    $row->funding_component = implode(';', $components);
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    doj_migration_prepare_workbench_settings($entity, $row);
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    if (isset($entity->nid) && $entity->nid) {
      doj_migration_complete_workbench_settings($entity, $row);
    }
  }
}
