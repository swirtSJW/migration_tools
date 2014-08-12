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
    $this->addFieldMapping('field_pr_topic', 'topic');
    $this->addFieldMapping('field_pr_component', 'component');
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
    $row->title = $row->documentTitle;

    parent::prepareRow($row);
    $row->tag = trim($row->tagSubject, ' ,');
    $row->legacy_url = trim($row->documentLocation, '/');
    $row->uid = 1;

    $row->topic = trim($row->tagSubject, ',');
    if (!empty($row->topic)
        && !taxonomy_get_term_by_name($row->topic, 'topic')
        && !taxonomy_get_term_by_name($row->topic, 'component')) {
      // print $row->topic . "\n";
      $this->queueMessage(t('Creating new topic term "@topic".', array('@topic' => $row->topic)));
      $vocab = taxonomy_vocabulary_machine_name_load('topic');
      $term = new stdClass();
      $term->name = $row->topic;
      $term->vid = $vocab->vid;
      taxonomy_term_save($term);
    }

    // The values of 1 and 2 for onHomepage can be considered synonymous.
    if ($row->onHomepage == 2) {
      $row->onHomepage = 1;
    }

    $row->component = $row->compLongName;
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
  // Soin to cms_opa_doclinks, tagsubjectindex, and cms_opa_homepage ?

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
    $this->addFieldMapping('title', 'documentTitle');
        $this->addFieldMapping('status', 'publishStatus');
    $this->addFieldMapping('created', 'creationDate');
    $this->addFieldMapping('updated', 'modificationDate');
    $this->addFieldMapping('promote', 'onHomepage');
    $this->addFieldMapping('migrate_redirects', 'legacy_url');
    $this->addFieldMapping('og_group_ref')->defaultValue('opa')->sourceMigration('Organization');
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');
    $this->addPathDnmMapping();

    $this->addUnmigratedSources(array(

    ));
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
