<?php

/**
 * @file
 * Defines JusticeHtmlToNodeMigration class.
 */

/**
 * Class JusticeHtmlToNodeMigration.
 *
 * Parent class for all migrations from static HTML to any content type.
 *
 * @package doj_migration
 */
abstract class JusticeHtmlToNodeMigration extends JusticeFileSourceMigration {

  protected $sourceParserClass;

  /**
   * @var SourceParser $source_parser
   *   The source parser object for a given row.
   */
  protected $sourceParser;

  /**
   * Define $this->source, $this->map, and simple mappings.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   * @param array $source_fields
   *   An associative array of fields contained in the source data, in the form
   *   of array('machine_name' => t('Human Readable')).
   * @param array $source_dirs
   *   A flat array of the source directories containing html files.
   * @param string $regex
   *   The file mask. Only file names matching this regex will be migrated.
   *   This will default to '/.*\.htm(l)?$/' if NULL is passed in.
   * @param array $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   * @param MigrateContentParser $parser
   *   A parser class that will provide file chunk ids. Defaults to
   *   MigrateSimpleContentParser in parent constructor.
   */
  public function __construct($arguments, $source_fields, $source_dirs, $regex = NULL, $options = array(), MigrateContentParser $parser = NULL) {
    if ($regex === NULL) {
      $regex = '/.*\.htm(l)?$/';
    }

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options, $parser);

    // A child class might provide it's own source parser that offer specific
    // cleanup.
    if (array_key_exists('source_parser_class', $arguments)) {
      $this->sourceParserClass = $arguments['source_parser_class'];
    }
    else {
      $this->sourceParserClass = "SourceParser";
    }

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('page');

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'uid',
      'title',
    ));

    // A map of source HTML filename -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'fileid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Define non-simple field mappings.
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');

    // Create redirects using relative, legacy path of static file.
    $this->addFieldMapping('migrate_redirects', 'legacy_path')->separator('|');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Sets $row->legacy_path and $row->url_path.
    if (empty($row->legacy_path) || empty($row->url_path)) {
      $this->generateLegacyPath($row);
    }

    // Check whether a redirect to this page already exists, if so, do not
    // migrate the "duplicate" content.
    if ($this->isDuplicateByRedirect($row)) {
      return FALSE;
    }

    // Set to admin for now.
    $row->uid = 1;

    // Create a new SourceParser to handle HTML content.
    $this->sourceParser = new $this->sourceParserClass($row->url_path, $row->filedata, array(), $this->getArguments());

    // We check that everything has been initialized correctly to keep children
    // classes from having to do all this checks.
    if (!is_object($this->sourceParser->queryPath)) {
      $this->queueMessage(t("@fileid failed to initialize QueryPath", array('@fileid' => $row->fileid)));
      return FALSE;
    }

    $row->title = $this->sourceParser->getTitle();
    $row->body = $this->sourceParser->getBody();
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    doj_migration_prepare_workbench_settings($entity, $row);

    // Sidestep language to get redirects and aliases working for all users.
    parent::languageSideStep($entity);
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    // Change the language back if it was sidestepped in prepare().
    // Must come before workbench settings or revision ends up unpublished.
    parent::languageReturnStep($entity);

    if (isset($entity->nid) && $entity->nid) {
      doj_migration_complete_workbench_settings($entity, $row);
    }
  }

  /**
   * Is this document a duplicate? Determine this by using redirect objects.
   *
   * Legacy paths from justice, should not be pointing to more than one node,
   * If this is happening, it is a good sign that we are bringing in duplicate
   * content.
   *
   * @param object $row
   *   A row object as delivered by migrate.
   *
   * @return bool
   *   Whether this row is a duplicate or not.
   */
  protected function isDuplicateByRedirect($row) {
    $parsed = redirect_parse_url($row->legacy_path);
    $source = isset($parsed['path']) ? ltrim($parsed['path'], '/') : '';
    $redirect = redirect_load_by_source($source);

    if ($redirect) {
      $message = "- @source  -> Skipped: Already redirected to '@redirect'.";
      watchdog('doj_migration', $message, array('@source' => $source, '@redirect' => $redirect->redirect), WATCHDOG_WARNING);
      return TRUE;
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   *
   * @param array $report_rows
   *   Additional rows of information to be added to the migration report.
   */
  public function postImport($report_rows = array()) {
    parent::postImport();
    $this->printMigrationReport($report_rows);
  }

  /**
   * Prints a migration report via drush.
   *
   * @param array $report_rows
   *   Additional rows of information to be added to the migration report.
   */
  public function printMigrationReport($report_rows = array()) {
    $duplicates = $this->countDuplicateTitles();
    $output_rows = array(
      array(t('Total records'), $this->sourceCount()),
      array(t('Missing titles'), $this->countEmptyTitles()),
      array(t('Duplicate titles'), $duplicates),
      array(t('Unpublished'), $this->countUnpublishedNodes()),
    );
    $output_rows = array_merge($output_rows, $report_rows);
    drush_print(t('Migration report:'));
    drush_print_table($output_rows);

    if (!empty($duplicates)) {
      $this->printDuplicateTitleReport();
    }
  }

  /**
   * Counts the number of unpublished nodes in current migration.
   *
   * @return int
   *   The number of  unpublished nodes.
   */
  public function countUnpublishedNodes() {
    // Count migrated, unpublished nodes.
    $map_table = $this->map->getMapTable();
    $query = db_query("SELECT COUNT(n.nid) FROM {node} n RIGHT JOIN {$map_table} m ON m.destid1 = n.nid WHERE n.status = 0");

    return $query->fetchField();
  }

  /**
   * Counts the number of nodes with empty titles in current migration.
   *
   * @return int
   *   The number of nodes with empty titles.
   */
  public function countEmptyTitles() {
    // Count migrated nodes with empty titles.
    $map_table = $this->map->getMapTable();
    $query = db_query("SELECT COUNT(n.nid) FROM {node} n RIGHT JOIN {$map_table} m ON m.destid1 = n.nid WHERE TRIM(n.title) = ''");

    return $query->fetchField();
  }

  /**
   * Counts the number of nodes with shared titles in current migration.
   *
   * @return int
   *   The number of nodes with shared titles.
   */
  public function countDuplicateTitles() {
    $map_table = $this->map->getMapTable();

    // Count many migrated nodes that share titles with each other.
    $query = db_query("SELECT SUM(title_group_count) as duplicates
                        FROM (SELECT COUNT(n.nid) as title_group_count
                        FROM node n
                        RIGHT JOIN {$map_table} m
                        ON m.destid1 = n.nid
                        WHERE TRIM(n.title) != ''
                        GROUP BY n.title
                        HAVING title_group_count > 1
                        ) src");

    return $query->fetchField();
  }

  /**
   * Prints a report of all nodes with duplicate titles, via drush.
   */
  public function printDuplicateTitleReport() {
    $map_table = $this->map->getMapTable();
    $query = db_query("SELECT n.title, COUNT(n.nid) as title_group_count
                        FROM node n
                        RIGHT JOIN {$map_table} m
                        ON m.destid1 = n.nid
                        WHERE TRIM(n.title) != ''
                        GROUP BY n.title
                        HAVING title_group_count > 1");

    $duplicate_titles = array(array(t('Title'), t('Count')));
    while ($row = $query->fetchAssoc()) {
      $duplicate_titles[] = array($row['title'], $row['title_group_count']);
    }
    drush_print(t('Duplicate title report:'));
    drush_print_table($duplicate_titles, TRUE);
  }
}
