<?php

/**
 * @file
 * Migration class for Press Releases stored in OPA CMS.
 */

/**
 * Class JusticePressReleaseMigration.
 */
abstract class JusticePressReleaseMigration extends JusticeBaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $org_abbreviation) {
    parent::__construct($arguments);

    $query = db_select('cmsopa.cms_opa_prs', 'c')
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
    $query->leftJoin('cmsopa.mainComponents', 'mc', 'c.componentID = mc.cid');
    $query->condition('mc.compAbbrev', $org_abbreviation)
      ->orderBy('id', 'ASC');

    // A map of source HTML filename -> destination node id.
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
    $this->addFieldMapping('title', 'documentTitle');
    $this->addFieldMapping('field_pr_tags', 'tag');
    $this->addFieldMapping('field_pr_number', 'prNumber');
    $this->addFieldMapping('status', 'publishStatus');
    $this->addFieldMapping('created', 'creationDate');
    $this->addFieldMapping('updated', 'modificationDate');
    $this->addFieldMapping('promote', 'onHomepage');
    $this->addFieldMapping('migrate_redirects', 'legacy_url');

    $this->addFieldMapping('field_pr_body', 'documentBody');
    $this->addFieldMapping('field_pr_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_pr_teaser', 'documentTeaser');
    $this->addFieldMapping('field_pr_teaser:format')->defaultValue('wysiwyg');

    $this->addFieldMapping('og_group_ref')->defaultValue($org_abbreviation)->sourceMigration('Organization');
    $this->addPathDnmMapping();

    $this->addUnmigratedSources(array(
      'spanishContent',
      'sensitiveData',
      'documentDate',
      'imageID',
      'prSubtitle',
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
    parent::prepareRow($row);
    $row->tag = trim($row->tagSubject, ' ,');
    $row->legacy_path = trim($row->documentLocation, '/');
  }
}
