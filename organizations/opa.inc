<?php

/**
 * @file
 * Defines migration classes for OPA group.
 *
 * @package doj_migration
 * @subpackage opa
 */

/**
 * This file should reside in wordpress://, which is generally
 * sites/default/private/wordpress.
 */
define('OPA_WORDPRESS_SOURCE', 'officeofpublicaffairs.wordpress.2014-01-30.xml');

/**
 * Class OpaBlogEntryMigration.
 *
 * @package doj_migration
 * @subpackage opa
 */
class OpaBlogEntryMigration extends JusticeWordpressBlogEntry {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OPA_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Class OpaAuthorMigration
 *
 * @package doj_migration
 * @subpackage opa
 */
class OpaAuthorMigration extends JusticeWordpressAuthor {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OPA_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Class OpaAttachmentMigration.
 *
 * @package doj_migration
 * @subpackage opa
 */
class OpaAttachmentMigration extends JusticeWordpressAttachment {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OPA_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Class OpaPressReleaseMigration.
 *
 * @package doj_migration
 */
class OpaPressReleaseMigration extends JusticeBaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->dependencies = array('Organization');

    global $databases;
    $cmsopa_db_name = $databases['cmsopa']['default']['database'];

    $query = db_select("$cmsopa_db_name.cms_opa_prs", 'c')
      ->fields('c', array(
        'id',
        'creationDate',
        'modificationDate',
        'publishStatus',
        'onHomepage',
        'documentLocation',
        'imageID',
        'componentID',
        'spanishContent',
        'sensitiveData',
        'documentDate',
        'documentTitle',
        'documentTeaser',
        'documentBody',
        'tagSubject',
        'prNumber',
        'prSubtitle',
        'prMediaAdvisory',
        'prURL',
        'prPurpose',
        'prPhone',
        'prTDD',
        'prLeftExtra',
        'prRightExtra',
      ));
    $query->leftJoin("$cmsopa_db_name.mainComponents", 'mc', 'c.componentID = mc.cid');
    $query->fields('mc', array(
      'compLongName',
      'compAbbrev',
    ));
    $query->orderBy('id', 'ASC');

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
    $this->destination = new MigrateDestinationNode('press_release');

    // Field Mappings.
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('status', 'publishStatus');
    $this->addFieldMapping('created', 'creationDate');
    $this->addFieldMapping('updated', 'modificationDate');
    $this->addFieldMapping('uid', 'uid');
    $this->addFieldMapping('migrate_redirects', 'legacy_url');

    $this->addFieldMapping('field_pr_body', 'documentBody');
    $this->addFieldMapping('field_pr_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_pr_teaser', 'documentTeaser');
    $this->addFieldMapping('field_pr_teaser:format')->defaultValue('wysiwyg');

    $this->addFieldMapping('field_pr_promote', 'onHomepage');
    $this->addFieldMapping('field_pr_date', 'documentDate');
    $this->addFieldMapping('field_pr_topic', 'topic')->separator('|');
    $this->addFieldMapping('field_pr_component', 'component')->separator('|');
    $this->addFieldMapping('field_pr_number', 'prNumber');
    $this->addFieldMapping('field_pr_subtitle', 'prSubtitle');

    $this->addFieldMapping('og_group_ref')->defaultValue('opa')->sourceMigration('Organization');
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');
    $this->addPathDnmMapping();

    $this->addUnmigratedSources(array(
      'spanishContent',
      'sensitiveData',
      'imageID',
      'prMediaAdvisory',
      'prURL',
      'prPurpose',
      'prPhone',
      'prTDD',
      'prLeftExtra',
      'prRightExtra',
    ));
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (!$row->publishStatus) {
      return FALSE;
    }

    $row->title = $row->documentTitle;

    parent::prepareRow($row);
    $row->tag = trim($row->tagSubject, ',');
    $row->legacy_url = trim($row->documentLocation, '/');
    $row->uid = 1;

    // If the term does not exist in topic or component vocabularies, create
    // a new topic term.
    module_load_include('inc', 'doj_migration', 'includes/doj_migration');
    $row->topic = doj_migrate_prepare_topic_term($row->tagSubject);

    // The values of 1 and 2 for onHomepage can be considered synonymous.
    if ($row->onHomepage == 2) {
      $row->onHomepage = 1;
    }

    $row->component = doj_migrate_standardize_component_terms($row->compLongName);
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


/**
 * Class OpaSpeechMigration
 *
 * Migrates speeches from {cms_opa_speeches} in cmsopa table.
 */
class OpaSpeechMigration extends JusticeBaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->dependencies = array('Organization');

    global $databases;
    $cmsopa_db_name = $databases['cmsopa']['default']['database'];

    $query = db_select("$cmsopa_db_name.cms_opa_speeches", 'c')
      ->fields('c', array(
        'id',
        'creationDate',
        'modificationDate',
        'publishStatus',
        'onHomepage',
        'documentLocation',
        'imageID',
        'componentID',
        'spanishContent',
        'sensitiveData',
        'documentDate',
        'documentTitle',
        'documentTeaser',
        'documentBody',
        'tagSubject',
        'speechLocation',
        'speechTime',
      ));
    $query->leftJoin("$cmsopa_db_name.mainComponents", 'mc', 'c.componentID = mc.cid');
    $query->fields('mc', array(
      'compLongName',
      'compAbbrev',
    ));
    $query->orderBy('id', 'ASC');

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
    $this->destination = new MigrateDestinationNode('speech');

    // Field Mappings.
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('status', 'publishStatus');
    $this->addFieldMapping('created', 'creationDate');
    $this->addFieldMapping('updated', 'modificationDate');
    $this->addFieldMapping('promote', 'onHomepage');
    $this->addFieldMapping('uid', 'uid');
    $this->addFieldMapping('migrate_redirects', 'legacy_url');
    $this->addFieldMapping('og_group_ref')->defaultValue('opa')->sourceMigration('Organization');
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');

    $this->addFieldMapping('field_speech_teaser', 'teaser');
    $this->addFieldMapping('field_speech_teaser:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_speech_body', 'body');
    $this->addFieldMapping('field_speech_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_speech_date', 'documentDate');
    $this->addFieldMapping('field_speech_component', 'component')->separator('|');
    $this->addFieldMapping('field_speech_topic', 'topic')->separator('|');

    $this->addFieldMapping('field_speech_location', 'country')->defaultValue('US');
    $this->addFieldMapping('field_speech_location:locality', 'city');
    $this->addFieldMapping('field_speech_location:administrative_area', 'state');

    $this->addPathDnmMapping();
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (!$row->publishStatus) {
      return FALSE;
    }

    // Decode html entities in body.
    $row->body = html_entity_decode($row->documentBody, ENT_QUOTES, 'UTF-8');
    $row->teaser = html_entity_decode($row->documentTeaser, ENT_QUOTES, 'UTF-8');

    // Define title and pass to parent for trimming.
    $row->title = $row->documentTitle;
    parent::prepareRow($row);

    $row->tag = trim($row->tagSubject, ',');
    $row->legacy_url = trim($row->documentLocation, '/');
    $row->uid = 1;

    // If the term does not exist in topic or component vocabularies, create
    // a new topic term.
    module_load_include('inc', 'doj_migration', 'includes/doj_migration');
    $row->topic = doj_migrate_prepare_topic_term($row->tagSubject);

    // The values of 1 and 2 for onHomepage can be considered synonymous.
    if ($row->onHomepage == 2) {
      $row->onHomepage = 1;
    }

    $row->component = doj_migrate_standardize_component_terms($row->compLongName);

    // Geocode the location and parse into structured data for migration.
    // The geocoder module is not an explicit dependency because most migrations
    // do not rely on it. It should be disabled after use.
    if (!empty($row->speechLocation)) {
      if (module_exists('geocoder')) {
        // Note that calling this too many times (as in very large migrations)
        // may exceed the API request limit for geocoder's source data.
        $point = geocoder('google', $row->speechLocation);
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

/**
 * Class OpaEventMigration
 *
 * @package doj_migration
 */
class OpaEventMigration extends JusticeFileSourceMigration {
  // Migrate events from {cms_doj_calendar} in cmsopa table.
}
