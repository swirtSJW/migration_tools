<?php

/**
 * @file
 * Defines migration classes for resource content.
 *
 * @package doj_migration
 * @subpackage resource
 */

/**
 * Class IsoResoucreMigration.
 *
 * @package doj_migration
 */
class ResourceIsoFormMigration extends JusticeBaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->dependencies = array('Organization');

    global $databases;
    $iso_db_name = $databases['iso']['default']['database'];
    // Grab all items that have no Parent.
    $query = db_select("$iso_db_name.dojforms", 'c')
      ->fields('c', array(
        'id',
        'FormNo',
        'Title',
        'Location',
        'Agency',
        'SubTitle',
        'Parent',
      ))
      ->isNull('c.Parent')
      ->orderBy('id', 'ASC');

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
    $this->destination = new MigrateDestinationNode('resource');

    // Field Mappings.
    $this->addFieldMapping('title', 'Title');
    // These are going to be all placed in the DOJ (sitewide) group.
    $doj_nid = 752;
    $this->addFieldMapping('og_group_ref')->defaultValue($doj_nid);

    $this->addFieldMapping('uid')->defaultValue(1);

    $this->addFieldMapping('field_resource_body')->defaultValue('');
    $this->addFieldMapping('field_resource_body:format')->defaultValue('wysiwyg');

    $this->addFieldMapping('field_resource_link:title', 'LinkTitle')->separator('|');
    $this->addFieldMapping('field_resource_link', 'Location')->separator('|');

    $this->addFieldMapping('field_resource_id', 'FormNo');

    $this->addFieldMapping('field_resource_component', 'Agency');

    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');
    $this->addPathDnmMapping();

  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    // Always include this fragment at the beginning of every prepareRow()
    // implementation, so parent classes can ignore rows.
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // If the term does not exist in topic or component vocabularies, create
    // a new topic term.
    module_load_include('inc', 'doj_migration', 'includes/doj_migration');

    // Convert the Agency to a matching component.
    $row->Agency = ResourceIsoFormMigration::ResourceComponentNormalize($row->Agency);
    if (empty($row->Agency)) {
      // There is no agency so abort the migration.
      return FALSE;
    }

    // Build the links.
    // Populate the first link Title from the database entry.
    $row->LinkTitle = $row->Title;

    // Need a new query to look for any children that list the current row as
    // the parent.  There are no grandparents or grandchildren to process.
    global $databases;
    $iso_db_name = $databases['iso']['default']['database'];

    $query = db_select("$iso_db_name.dojforms", 'c')
      ->fields('c', array(
        'id',
        'Location',
        'SubTitle',
        'Parent',
      ))
      ->condition('c.Parent', $row->id, '=')
      ->orderBy('id', 'ASC');
    $results = $query->execute();
    // Loop through children results and add them as links.
    foreach ($results as $link) {
      // Add the Location as the link url and the subtitle as the link title.
      $row->Location .= '|' . $link->Location;
      $row->LinkTitle .= '|' . $link->SubTitle;
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


  /**
   * Converts an agency to a component term, or to NULL if no match found.
   *
   * @param string $agency
   *   The agency to look up and translated to a component.
   *
   * @return string
   *   The normalized component term or NULL if not found.
   */
  public function ResourceComponentNormalize($agency = '') {
    $normalized_term = NULL;
    $agency = trim($agency);

    $conversion_map = array(
      // NULL translations need not be here but are for record of existence.
      'ATF'	=>	'Bureau of Alcohol, Tobacco, Firearms and Explosives',
      'ATR'	=>	'Antitrust Division',
      'CIV'	=>	'Civil Division',
      'COPS'	=>	'Community Oriented Policing Services (COPS)',
      'CRM'	=>	'Criminal Division',
      'CRT'	=>	'Civil Rights Division',
      'DEA'	=>	'Drug Enforcement Administration (DEA)',
      'EOIR'	=>	'Executive Office for Immigration Review',
      'EOUSA'	=>	'Executive Office for U.S. Attorneys',
      'EOUST'	=>	'Executive Office for U.S. Trustees',
      'FBI' =>	'Federal Bureau of Investigation (FBI)',
      'FCSC'	=>	'Foreign Claims Settlement Commission of the United States',
      'FTC'	=>	NULL,
      'JMD'	=>	'Justice Management Division',
      'NDIC'	=>	NULL,
      'NSD'	=>	'National Security Division',
      'OJP'	=>	'Office of Justice Programs',
      'OMB'	=>	NULL,
      'OPM'	=>	NULL,
      'Pardon'	=>	'Office of the Pardon Attorney',
      'USCIS'	=>	NULL,
      'USCourts'	=>	NULL,
      'USMS'	=>	'U.S. Marshals Service',
      'USPC'	=>	'U.S. Parole Commission',
    );

    if (isset($conversion_map[$agency])) {
      $normalized_term = $conversion_map[$agency];
    }

    return $normalized_term;
  }
}
