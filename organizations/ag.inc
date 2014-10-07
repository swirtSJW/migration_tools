<?php

/**
 * @file
 * Defines migration classes for AG group.
 *
 * @package doj_migration
 * @subpackage ag
 */

/**
 * Migrates .html files from /ag to page nodes.
 *
 * @package doj_migration
 * @subpackage ag
 */
class AgPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    $json_src = '["ag", "ag\/youngstown", "ag\/testimony\/2002", "ag\/testimony\/2004", "ag\/testimony\/2010", "ag\/testimony\/2001", "ag\/testimony\/2009", "ag\/testimony\/2008", "ag\/testimony\/2003", "ag\/testimony\/2005", "ag\/readingroom", "ag\/annualreports\/pr2010", "ag\/annualreports\/pr2012", "ag\/annualreports\/afr2013", "ag\/annualreports\/annualreports\/pr2010", "ag\/annualreports\/annualreports\/afr2013", "ag\/annualreports\/annualreports\/pr2006", "ag\/annualreports\/annualreports\/pr2006\/P3", "ag\/annualreports\/annualreports\/pr2005", "ag\/annualreports\/annualreports\/pr2005\/P3", "ag\/annualreports\/annualreports\/pr2008\/sect3", "ag\/annualreports\/annualreports\/pr2008", "ag\/annualreports\/annualreports\/pr2007\/sect3", "ag\/annualreports\/annualreports\/pr2007", "ag\/annualreports\/annualreports\/pr2011", "ag\/annualreports\/annualreports\/pr2009\/sect3", "ag\/annualreports\/annualreports\/pr2009", "ag\/annualreports\/annualreports\/pr2004", "ag\/annualreports\/annualreports\/pr2004\/P4", "ag\/annualreports\/annualreports\/pr2004\/P1", "ag\/annualreports\/annualreports\/pr2004\/P3", "ag\/annualreports\/pr2011", "ag\/annualreports\/pr2009\/sect3", "ag\/annualreports\/pr2009", "ag\/annualreports\/apr2013", "ag\/aghistory\/tclark", "ag\/aghistory\/smith", "ag\/aghistory\/kleindienst", "ag\/aghistory\/mitchell", "ag\/aghistory\/civiletti", "ag\/aghistory\/richardson", "ag\/aghistory\/murphy", "ag\/aghistory\/mcgranery", "ag\/aghistory\/meese", "ag\/aghistory\/thornburgh", "ag\/aghistory\/jackson", "ag\/aghistory\/mcgrath", "ag\/aghistory\/gonzales", "ag\/aghistory\/clark", "ag\/aghistory", "ag\/aghistory\/mukasey", "ag\/aghistory\/bell", "ag\/aghistory\/ashcroft", "ag\/aghistory\/reno", "ag\/aghistory\/brownell", "ag\/aghistory\/cummings", "ag\/aghistory\/barr", "ag\/aghistory\/rogers", "ag\/aghistory\/levi", "ag\/aghistory\/saxbe", "ag\/aghistory\/biddle", "ag\/aghistory\/katzenbach", "ag\/events\/g82004", "ag\/defendingchildhood", "ag\/smart-on-crime"]';

    // Define source directories.
    $source_dirs = drupal_json_decode($json_src);

    $options = array(
      'recurse' => FALSE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from AG section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('ag')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $bool = parent::prepareRow($row);
    if (isset($bool)) {
      return $bool;
    }

    // Remove blank lines. This removes only an entire line that is blank,
    // not all \n or \r instances.
    // @see http://stackoverflow.com/a/709684
    $row->body = preg_replace("/(^[\r\n]*|[\r\n]+)[\s\t]*[\r\n]+/", "\n", $row->body);

    // Remove all \r and \n instances.
    $row->body = preg_replace("/\r|\n/", " ", $row->body);
  }
}

/**
 * Migrates binary files from /ag.
 *
 * @package doj_migration
 * @subpackage ag
 */
class AgFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates files (*.pdf) from the ag subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'ag',
    );

    $options = array(
      'recurse' => TRUE,
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///dag/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "ag";
  }
}

/**
 * Migrates  speeches .html files from /ag to speech nodes.
 *
 * @package doj_migration
 * @subpackage ag
 */
class AgSpeechMigration extends JusticeHtmlToSpeechMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'ag/speeches',
    );
    $options = array(
      'recurse' => TRUE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, '/[^spanish]+\.htm(l)?$/', $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates Speeches from the AG section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('ag')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    module_load_include("inc", "doj_migration", "includes/NewsHelper");
    if (NewsHelper::rejectNewsOlderThan(2009, $row->fileid) == TRUE) {
      return FALSE;
    }

    $bool = parent::prepareRow($row);
    if (isset($bool) && $bool == FALSE) {
      return FALSE;
    }

    $row->field_speech_component = "Office of the Attorney General";
  }
}

/**
 * Migrates  speeches .html files from /ag to speech nodes.
 *
 * @package doj_migration
 * @subpackage ag
 */
class AgSpeechSpanishMigration extends JusticeHtmlToSpeechSpanishMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'ag/speeches/2009',
      'ag/speeches/2010',
    );
    $options = array(
      'recurse' => TRUE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, '/.*-spanish\.htm(l)?$/', $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates Speeches from the AG section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('ag')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    module_load_include("inc", "doj_migration", "includes/NewsHelper");
    if (NewsHelper::rejectNewsOlderThan(2009, $row->fileid) == TRUE) {
      return FALSE;
    }

    $bool = parent::prepareRow($row);
    if (isset($bool) && $bool == FALSE) {
      return $bool;
    }

    $row->field_speech_component = "Office of the Attorney General";
  }
}

/**
 * Class AgHistoricalBioMigration.
 *
 * @package doj_migration
 */
class AgHistoricalBioMigration extends JusticeBaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->dependencies = array('Organization');

    global $databases;
    $iso_db_name = $databases['iso']['default']['database'];

    $query = db_select("$iso_db_name.attygeneral", 'c')
      ->fields('c', array(
        'id',
        'term',
        'wholename',
        'first',
        'last',
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
    $this->destination = new MigrateDestinationNode('historical_bio');

    // Field Mappings.
    $this->addFieldMapping('og_group_ref')->defaultValue("ag");

    $this->addFieldMapping('title', 'wholename');
    $this->addFieldMapping('field_bio_body', 'description');
    $this->addFieldMapping('field_bio_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_dates_service', 'term1');
    $this->addFieldMapping('field_dates_service:to', 'term2');
    $this->addSimpleMappings(array("uid", "status"));
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $bool = parent::prepareRow($row);
    if (isset($bool) && $bool == FALSE) {
      return $bool;
    }

    $term = $row->term;
    $terms = explode("-", $term);
    $row->term1 = trim($terms[0]);
    $row->term2 = trim($terms[1]);

    $row->uid = 1;
    $row->status = 1;
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
