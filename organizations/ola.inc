<?php
/**
 * @file
 * Office of Legislative Affairs OLA.
 *
 * Defines migration classes for the OLA section of justice.gov.
 */

class OlaTestimonyMigration extends JusticeBaseMigration {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->dependencies = array('Organization');

    global $databases;
    $cmsopa_db_name = $databases['cmsopa']['default']['database'];

    $query = db_select("$cmsopa_db_name.cms_ola_testimony", 'c')
      ->fields('c', array(
        'id',
        'creationDate',
        'modificationDate',
        'sensitiveData',
        'documentDate',
        'documentTitle',
        'testimonyCommitte',
        'dateExtra',
        'congressTerm',
        'congressSession',
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
    $this->destination = new MigrateDestinationNode('testimony');

    // Field Mappings.
    $this->addFieldMapping('og_group_ref')->defaultValue('ola');

    $this->addFieldMapping('title', 'documentTitle');
    $this->addFieldMapping('create', 'creationDate');
    $this->addFieldMapping('changed', 'modificationDate');
    $this->addFieldMapping('field_testimony_subject', 'documentTitle');
    $this->addFieldMapping('field_testimony_date', 'documentDate');
    $this->addFieldMapping('field_testimony_committee', 'testimonyCommitte')->separator(",");
    $this->addFieldMapping('field_testimony_committee:create_term')->defaultValue(TRUE);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $bool = parent::prepareRow($row);
    if (isset($bool) && $bool == FALSE) {
      return $bool;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    doj_migration_prepare_workbench_settings($entity, $row);
    $entity->status = 1;
    $entity->uid = 1;
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

/**
 * Migrates .html files from /ola to page nodes.
 *
 * @package doj_migration
 * @subpackage ola
 */
class OlaPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $dir_json = '["ola","ola\/views-letters"]';
    // Define source directories.
    $source_dirs = drupal_json_decode($dir_json);

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    $arguments['source_parser_class'] = "OlaPageSourceParser";
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Legislative Affairs section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('ola')->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /ola.
 *
 * @package doj_migration
 * @subpackage ola
 */
class OlaFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the ola subdirectory.');
    $this->dependencies = array('Organization');

    $dir_json = '["ola\/views-letters\/113","ola\/views-letters\/111-1","ola\/views-letters\/111-2","ola\/views-letters\/112"]';

    // Match .pdf files only.
    $source_dirs = drupal_json_decode($dir_json);

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///ola/docs';

    $options = array('recurse' => FALSE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "ola";
  }
}
