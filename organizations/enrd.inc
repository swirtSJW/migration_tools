<?php

/**
 * @file
 * Defines migration classes for Environment and Natural Resources Division.
 */

/**
 * Migrates .html files from /enrd to page nodes.
 *
 * @package doj_migration
 * @subpackage enrd
 */
class EnrdPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      'enrd',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "EnrdPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "EnrdObtainTitlePage");
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('pluckSelector', array("title", 1));
    $title->addMethod('pluckSelector', array("h1", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Environment and Natural Resources Division.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('enrd')
    ->sourceMigration('Organization');
    $this->addFieldMapping('context')->defaultValue('no_right_sidebar');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/enrd/timeline_eo.html',
      '/enrd/timeline_IR.html',
      '/enrd/timeline_LP.html',
    );
    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }

    // All content with a "all numeric" title are teasers, and can be
    // ignored.
    $title = trim(str_replace("-", "", $row->title));
    if (is_numeric($title)) {
      return FALSE;
    }

    // All content with a month as the title only have a link to a pdf.
    // We can ignore these content. Also, there are a few other useless
    // pieces of content that I would like to clean up.
    $months = array("January", "February", "March", "April", "May", "June",
      "July", "August", "September", "October", "November", "December",
      "Redirect Page");
    if (in_array($title, $months)) {
      return FALSE;
    }

    // Content with months as titles are not the only pages with only a link
    // so we need to check for some other pattern. All those links have
    // text within font tags saying how big the file is (MB).
    foreach ($this->sourceParser->queryPath->find("font[color='black']") as $font) {
      $text = $font->text();
      if (substr_count($text, "MB") > 0) {
        drush_print_r("We got one!!!");
        return FALSE;
      }
    }

    // All content with a specific banner is from the "100th Anniversary" of
    // ENRD, we can ignore this content.
    $body = $row->body;
    if (substr_count($body, "masthead_home.gif") > 0) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /enrd.
 *
 * @package doj_migration
 * @subpackage enrd
 */
class EnrdFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the enrd subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'enrd',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///enrd/docs';

    $options = array('recurse' => TRUE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "enrd";
  }
}

/**
 * Migrates press releases from /enrd.
 *
 * @package doj_migration
 * @subpackage enrd
 */
class EnrdPressReleaseMigration extends NGJusticeHtmlToPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['component'] = "Environment and Natural Resources Division";
    $arguments['source_parser_class'] = "EnrdPressSourceParser";
    $arguments['component_tid'] = 471;

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "EnrdObtainTitlePress");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array("h2", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));
    $title->addMethod('pluckSelector', array("p > strong > em", 1));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 1));

    $date = new ObtainerInfo('date', "EnrdObtainDate");
    $date->addMethod('pluckTableRow1Col2');
    $date->addMethod('pluckTableRow1Col1');
    $date->addMethod('pluckTable2Row2Col2');
    $date->addMethod('pluckSelector', array("p[align='center']", 1));
    $date->addMethod('pluckSelector', array("#contentstart > p", 1));
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckSelector', array(".BottomLeftContent", 1));
    $date->addMethod('pluckProbableDate');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;

    $source_dirs = array();
    parent::__construct($arguments, $source_dirs);
  }
}

class EnrdPageSourceParser  extends NGNodeSourceParser {

  /**
   * {@inheritdoc}
   */
  public function getTitle() {
    $title = parent::getTitle();
    $title = str_replace("USDOJ: Environment And Natural Resources Division 100th Anniversary :", "", $title);
    $title = str_replace("USDOJ: Environment And Natural Resources Division :", "", $title);
    $title = str_replace("USDOJ:", "", $title);
    $this->sourceParserMessage('title cleaned --> @text', array('@text' => $title), WATCHDOG_DEBUG, 2);
    return $title;
  }

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    $this->queryPath->find("font[color='#999999']")->remove();
    $this->queryPath->find(".headline")->first()->remove();
    $this->queryPath->find(".right-column")->remove();
    HtmlCleanUp::rewrapElements($this->queryPath, array('.headline'), '<h2 />');

    // Remove the first 2 uls they are menus.
    $uls = $this->queryPath->find("ul");
    $counter = 0;
    foreach ($uls as $ul) {
      $ul->remove();
      $counter++;
      if ($counter == 2) {
        break;
      }
    }
  }
}

class EnrdPressSourceParser extends NGPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

class EnrdObtainTitlePage extends ObtainTitle {
}

class EnrdObtainTitlePress extends ObtainTitlePressRelease {
}

class EnrdObtainDate extends ObtainDate {
}

class EnrdTimelineMigration extends NGJusticeHtmlToNodeMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      'enrd',
    );

    $options = array('recurse' => TRUE);

    // Expected timeline nodes:
    // http://www.justice.gov/enrd/timeline_eo.html
    // http://www.justice.gov/enrd/timeline_IR.html
    // http://www.justice.gov/enrd/timeline_LP.html
    $regex = '/timeline_.+\.htm(l)?/';

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
    );

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('timeline');

    $this->addFieldMapping('og_group_ref')->defaultValue('enrd')->sourceMigration('Organization');
  }
}

class EnrdEventMigration extends XMLMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->setHardDependencies(array('EnrdTimeline'));

    // Expected files:
    // timeline_EO.xml
    // timeline_IR_events.xml
    // timeline_LP_LPevents.xml
    // timeline_LP_Worldevents.xml

    $this->timelineMap = array(
      '/enrd/timeline_EO.xml' => '/enrd/timeline_eo.html',
      '/enrd/timeline_IR_events.xml' => '/enrd/timeline_IR.html',
      '/enrd/timeline_LP_LPevents.xml' => '/enrd/timeline_LP.html',
      '/enrd/timeline_LP_Worldevents.xml' => '/enrd/timeline_LP.html',
    );

    $this->baseDir = variable_get('doj_migration_base_dir', DOJ_LEGACY_WEBROOT);

    $xml_folder = $this->baseDir . '/enrd';
    $mask = '/timeline_.+\.xml/i';
    $options['recurse'] = TRUE;
    $files = file_scan_directory($xml_folder, $mask, $options);
    $items_url = array_keys($files);

    // Relative to document.
    $item_xpath = '/data/event';
    // Relative to item_xpath.
    $item_id_xpath = '@title';

    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_id_xpath);

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'field_timeline_event_body' => t('Body'),
      'field_timeline_event_date' => t('Date'),
    );

    // The source ID here is the one retrieved from each data item in the XML
    // file, and used to identify specific items.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'title' => array(
          'type' => 'varchar',
          'length' => 255,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );
    $this->destination = new MigrateDestinationNode('timeline_event');

    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('field_timeline_event_body', 'body');
    $this->addFieldMapping('field_timeline_event_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_timeline_event_date', 'datetime');

    $this->addFieldMapping('field_timeline_event_image:file_class')->defaultValue('MigrateFileUri');
    $this->addFieldMapping('field_timeline_event_image', 'image_filename');
    $this->addFieldMapping('field_timeline_event_image:source_dir', 'image_dir');
    $this->addFieldMapping('field_timeline_event_image:file_replace')->defaultValue(FILE_EXISTS_REUSE);
    // $this->addFieldMapping('field_timeline_event_image:alt', 'alt');
    // $this->addFieldMapping('field_timeline_event_image:title', 'title');

    $this->addFieldMapping('og_group_ref')->defaultValue('enrd')->sourceMigration('Organization');
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');
    $this->addFieldMapping('uid')->defaultValue(1);
  }


  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $base_path = variable_get('doj_migration_base_dir', DOJ_LEGACY_WEBROOT);
    $row->file_path = str_replace($base_path, '', $this->source->getReader()->url);

    $row->title = (string) $row->xml['title'];
    $row->datetime = strtotime((string) ($row->xml['start']));
    if (empty($row->datetime)) {
      throw new Exception('Date is invalid for ' . $row->title . ' in ' . $row->file_path);
    }

    $image_relative_path = (string) $row->xml['image'];
    if ($image_relative_path) {
      $image_path = $this->baseDir . '/enrd/' . $image_relative_path;
      $row->image_filename = basename($image_path);
      $row->image_dir = dirname($image_path);
    }

    // We need to run the string through a SourceParser to that it gets
    // the benefit of encoding and cleanHtml() methods.
    $body = '<html><body>' . (string) $row->xml[0] . '</body></html>';
    $source_parser = new SourceParser($row->file_path, $body);
    $row->body = $source_parser->getBody();
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    doj_migration_prepare_workbench_settings($entity, $row);
  }

  /**
   * {@inheritdoc}
   *
   * The primary responsibility of this method is to associate the incoming
   * event with its parent timeline node.
   */
  public function complete($entity, stdClass $row) {
    if (isset($entity->nid) && $entity->nid) {
      doj_migration_complete_workbench_settings($entity, $row);
    }

    $source_id = $this->timelineMap[$row->file_path];
    $timeline_nid = $this->handleSourceMigration('EnrdTimeline', $source_id);
    if (empty($timeline_nid)) {
      throw new Exception('Parent timeline not found for ' . $row->title . ' in ' . $row->file_path);
    }
    $timeline_node = node_load($timeline_nid);
    $wrapper = entity_metadata_wrapper('node', $timeline_node);
    $wrapper->field_timeline_events[] = $entity->nid;
    $wrapper->save();
  }

  /**
   * {@inheritdoc}
   *
   * The primary responsibility of this method is to remove the association
   * between the current event and the parent timeline node.
   */
  public function completeRollback($entity_ids) {
    // Remove reference to entity on parent timeline node.
    foreach ($this->timelineMap as $source_id) {
      $timeline_nid = $this->handleSourceMigration('EnrdTimeline', $source_id);
      $timeline_node = node_load($timeline_nid);
      $wrapper = entity_metadata_wrapper('node', $timeline_node);
      $events = $wrapper->field_timeline_events->raw();
      foreach ($events as $delta => $event_nid) {
        if (in_array($event_nid, $entity_ids)) {
          $wrapper->field_timeline_events[$delta]->set(NULL);
        }
      }
      $wrapper->save();
    }
  }
}
