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
    parent::prepareRow($row);
    $row->field_speech_component = "Office of the Attorney General";
    try {
      $date_string = $this->sourceParser->queryPath->find('.speechdate')->first()->text();
      $date = DateTime::createFromFormat("l, F j, Y", $date_string);
      if ($date) {
        $row->field_speech_date = $date->format('Y-m-d');
      }
      else {
        $this->queueMessage("{$row->fileId} date does not have the format l, F j, Y: {$date_string}");
      }
    }
    catch(Exception $e){
      $this->queueMessage("{$row->fileId} failed to acquire a date");
    }
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
    parent::prepareRow($row);
    $row->field_speech_component = "Office of the Attorney General";
  }
}
