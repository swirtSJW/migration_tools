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
    $this->addFieldMapping('field_testimony_session', 'term');
    $this->addFieldMapping('field_testimony_session:create_term')->defaultValue(TRUE);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $bool = parent::prepareRow($row);
    if (isset($bool) && $bool == FALSE) {
      return $bool;
    }

    $pieces = explode("-", $row->documentDate);
    $year = $pieces[0];

    $m = array(
      1 => "First",
      2 => "Second",
      3 => "Third",
      4 => "Fourth",
      5 => "Fifth",
    );

    $row->term = "{$row->congressTerm}th Congress {$m[$row->congressSession]} Session - {$year}";
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

class OlaWitnessMigration extends JusticeBaseMigration {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->dependencies = array('Organization', 'OlaTestimony');

    global $databases;
    $cmsopa_db_name = $databases['cmsopa']['default']['database'];

    $query = db_select("$cmsopa_db_name.cms_opa_doclinks", 'c')
      ->fields('c', array(
        'id',
        'docID',
        'documentType',
        'url',
        'text',
        'inDocument',
      ))->condition('documentType', 'olatestimony', "=");

    // A map of source MySQL row -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'int',
          'length' => 11,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationFieldCollection::getKeySchema()
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceSQL($query);

    $this->destination = new MigrateDestinationFieldCollection(
      'field_testimony_witness',
      array('host_entity_type' => 'node')
    );

    $this->addFieldMapping('host_entity_id', 'docID')->sourceMigration('OlaTestimony');
    $this->addFieldMapping('field_witness_name', 'name');
    $this->addFieldMapping('field_witness_position', 'position');
    $this->addFieldMapping('field_witness_component', 'component');

    $this->addFieldMapping('field_witness_attachment', 'attachment_filename');
    $this->addFieldMapping('field_witness_attachment:file_class')
      ->defaultValue('MigrateFileUri');
    $this->addFieldMapping('field_witness_attachment:destination_dir', 'destination_dir');
    $this->addFieldMapping('field_witness_attachment:source_dir', 'source_dir');
    $this->addFieldMapping('field_witness_attachment:file_replace')->defaultValue(FILE_EXISTS_REUSE);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $text = $row->text;
    $pieces = explode("--------", $text);

    // The second piece contains the position and the component.
    $pc = explode(",", $pieces[1]);

    $row->name = $pieces[0];
    $row->position = trim($pc[0]);

    $component = trim($pc[1]);

    if (!empty($component)) {
      $query = db_select("taxonomy_term_data", "t")->fields("t", array('name'))->condition('vid', 6, '=')
        ->condition('name', "%{$component}%", "LIKE");

      $results = $query->execute();
      foreach ($results as $result) {
        $row->component = $result->name;
        break;
      }
    }

    $url = $row->url;
    $row->attachmentRedirects = array(substr($url, 1));

    $pieces = explode("/", $url);

    $attachment_filename = array_pop($pieces);

    $file_pieces = explode("-", $attachment_filename);
    $year = "20{$file_pieces[2]}";
    $month = $file_pieces[0];
    $day = $file_pieces[1];

    $source_dir = implode("/", $pieces);
    $source_dir = variable_get("doj_migration_base_dir") . $source_dir . "/";

    // $source_dir = variable_get("doj_migration_base_dir") .
    // "/ola/testimony/{$info[0]}-{$info[1]}/";

    $destination_dir = "public:///testimonies/witnesses/attachments/{$year}/{$month}/{$day}/";

    $row->attachment_filename = $attachment_filename;
    $row->source_dir = $source_dir;
    $row->destination_dir = $destination_dir;
  }

  /**
   * Not being use right now. Might be helpful with testimony html migrations.
   */
  private function parseHostTerm($term) {
    $return = array();

    $ms = array(
      1 => "First",
      2 => "Second",
      3 => "Third",
      4 => "Fourth",
      5 => "Fifth",
    );

    $cs = array("110", "111", "112", "113", "114", "115");

    $name = $term->name;

    foreach ($cs as $c) {
      if (substr_count($name, $c) > 0) {
        $return[] = $c;
        break;
      }
    }

    foreach ($ms as $n => $m) {
      if (substr_count($name, $m) > 0) {
        $return[] = $n;
        break;
      }
    }

    return $return;
  }

  /**
   * Not being use right now. Might be helpful with testimony html migrations.
   */
  private function getHostEntity($doc_id) {
    // Arghh, need the host_entity_id here. Will ned to do db query directly.
    $query = db_select("migrate_map_olatestimony", "t");
    $query->fields("t", array('destid1'));
    $query->condition("sourceid1", $doc_id, "=");
    $results = $query->execute();
    $node = NULL;
    foreach ($results as $r) {
      $host_entity_id = $r->destid1;
      $node = node_load($host_entity_id);
      break;
    }
    return $node;
  }

  /**
   * Not being use right now. Might be helpful with testimony html migrations.
   */
  private function getSessionFromHost($host) {
    // Now we need a session.
    $items = field_get_items("node", $host, "field_testimony_session");
    $item = reset($items);
    $term = NULL;
    if ($item) {
      $term = taxonomy_term_load($item['tid']);
    }
    return $term;
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRollback($entity_id) {
    $entity = entity_load_single('field_collection_item', $entity_id);
    $this->rollbackAttachmentRedirect($entity, 'field_witness_attachment');
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    if (module_exists('redirect')) {
      $this->createAttachmentRedirect($entity, $row->attachmentRedirects, 'field_witness_attachment');
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
