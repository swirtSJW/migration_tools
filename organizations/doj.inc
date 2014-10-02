<?php

/**
 * @file
 * Department of Justice.
 *
 * Defines migration classes for the DOJ section of justice.gov.
 */

/**
 * Migrates .html files from /crs to page nodes.
 *
 * @package doj_migration
 * @subpackage doj
 */
class DojPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'about',
      'accessibility',
      'accomplishments',
      'actioncenter',
      'agencies',
      'business',
      'cjs',
      'data',
      'dec',
      'developer',
      'digitalstrategy',
      'employees',
      'healthcare',
      'iqpr',
      'joiningforces',
      'publications',
      'sitemap',
      'about.html',
      'a-z-index.html',
      'component.htm',
      'contact-us.html',
      'dojofficials.htm',
      'legalpolicies.htm',
      'opa-in-memoriam.htm',
      'privacy-file.htm',
      'rss.htm',
    );
    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from main Department of Justice section and maintains their root level structure.');
    // Hard coding in the nid of the DOJ group.
    $doj_nid = 752;
    $this->addFieldMapping('og_group_ref')->defaultValue($doj_nid);

    // Since this content belongs to DOJ but is not going into /doj/ then
    // pathauto must be overridden.  See prepareRow().
    $this->addFieldMapping('path', 'url_alias');

    $this->addFieldMapping('pathauto', 'pathauto');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);
    $row->body = StringCleanUp::stripCmsLegacyMarkup($row->body);

    // Check for title alterations needed if title is "Publications".
    if ($row->title == 'Publications') {
      // Grab a more meaningful title from the <title>.
      $row->title = $this->sourceParser->queryPath->find('title')->first()->text();
    }

    // Since the doj group is the root of the site, the group path is ''.
    // Generate the full overridden alias.
    $row->group_path_override = '';
    $sub_sub_path = preg_replace('/([\/]?[0-9a-z\-\_]+)?\.(htm(l?))/ixm', '', $row->url_path);
    $row->url_path_overridden = (empty($row->group_path_override)) ? $sub_sub_path : $row->group_path_override . '/' . $sub_sub_path;

    if ((!empty($row->isIndex)) && module_load_include('inc', 'pathauto', 'pathauto')) {
      // It is an index, so the alias does not need the title in it.
      $row->url_alias = $row->url_path_overridden;
      pathauto_alias_uniquify($row->url_alias, '', LANGUAGE_NONE);
    }
    else {
      // It is not an index so it needs the title in the alias too.
      $row->url_alias = $row->url_path_overridden . '/' . pathauto_cleanstring($row->title);
      pathauto_alias_uniquify($row->url_alias, '', LANGUAGE_NONE);
    }

    // Disable pathauto.
    $row->pathauto = FALSE;
  }
}


/**
 * Migrates non-image files from the doj content.
 *
 * @package doj_migration
 * @subpackage doj
 */
class DojFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the doj (remaining sections of version4 tarball).');
    $this->dependencies = array('Organization');

    // Match non-image files.
    $source_dirs = array(
      'accomplishments',
      'business/itss',
      'cjs/docs',
      'data',
      'employees',
      'healthcare/docs',
      'publications',
      'publications/waco',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx|swf|flv)/';
    $dest_dir = 'public:///doj/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return 'doj';
  }
}

/**
 * Class DojCaseMigration.
 *
 * @package doj_migration
 */
class DojCaseMigration extends JusticeBaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->dependencies = array('Organization');

    global $databases;
    $iso_db_name = $databases['iso']['default']['database'];

    $query = db_select("$iso_db_name.largecases", 'c')
      ->fields('c', array(
        'id',
        'case_link',
        'names',
        'district',
        'overview',
        'case',
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
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('case');

    // Field Mappings.
    // Hard coding in the nid of the DOJ group.
    $doj_nid = 752;
    $this->addFieldMapping('og_group_ref')->defaultValue($doj_nid);

    $this->addFieldMapping('title', 'case');
    $this->addFieldMapping('field_case_link:title', 'case');
    $this->addFieldMapping('field_case_overview', 'overview');
    $this->addFieldMapping('field_case_overview:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_case_link', 'case_link');
    $this->addFieldMapping('field_case_tags', 'names')->separator(";");
    $this->addFieldMapping('field_case_tags:create_term')->defaultValue(TRUE);
    $this->addFieldMapping('field_case_component', 'district');
    $this->addFieldMapping('field_case_component:create_term')->defaultValue(TRUE);
    $this->addFieldMapping('field_case_topic')->defaultValue("Information for Victims in Large Cases");
    $this->addFieldMapping('field_case_topic:create_term')->defaultValue(TRUE);
    $this->addSimpleMappings(array("uid", "status"));
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);
    $row->uid = 1;
    $row->status = 1;
    $row->case_link = "http://www.justice.gov{$row->case_link}";
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
 * Class DojResourceIsoFormMigration.
 *
 * @package doj_migration
 */
class DojResourceIsoFormMigration extends JusticeBaseMigration {

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
    $row->Agency = DojResourceIsoFormMigration::ResourceComponentNormalize($row->Agency);
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
