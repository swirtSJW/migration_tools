<?php

/**
 * @file
 * Defines JusticeBinaryFileMigration class.
 */

/**
 * Class JusticeFileSourceMigration.
 *
 * Parent class for all migrations with source data stored in files.
 *
 * @package doj_migration
 */
abstract class JusticeFileSourceMigration extends JusticeBaseMigration {

  // Stores base directory for legacy files.
  public $baseDir;

  /**
   * Prepare the file source.
   *
   * This will setup $this->source.
   * It will also add field mappings for redirects.
   *
   * All derived classes should define 'fileid' as the source key in
   * MigrateSQLMap(), as it is used to create redirects.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   * @param array $source_fields
   *   An associative aray of fields contained in the source data, in the form
   *   of array('machine_name' => t('Human Readable')).
   * @param array $source_dirs
   *   An array of source directories, relative to $this->baseDir.
   * @param string $regex
   *   The file mask. Only filenames matching this regex will be migrated.
   * @param array $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   * @param MigrateContentParser $parser
   *   A parser class that will provide file chunk ids. Defaults to
   *   MigrateSimpleContentParser in parent constructor.
   */
  public function __construct($arguments, $source_fields, $source_dirs, $regex, $options = array(), MigrateContentParser $parser = NULL) {
    parent::__construct($arguments);

    $this->baseDir = variable_get('doj_migration_base_dir', DOJ_LEGACY_WEBROOT);

    // Define the directories containing files to be migrated.
    $absolute_dirs = $this->prependJusticeFilepath($source_dirs);

    // $list_files will provide migrate with a list of all files to be migrated.
    // $item_file provides methods for retrieving a file given an identifier.
    // $this->source define the essential data source from which to migrate.
    $list_files = new MigrateListFiles($absolute_dirs, $this->baseDir, $regex, $options, $parser);
    $item_file = new MigrateItemFile($this->baseDir, TRUE, $parser);
    $this->source = new MigrateSourceList($list_files, $item_file, $source_fields);
  }

  /**
   * Makes relative filepaths absolute based on $this->baseDir.
   *
   * @param array $relative_paths
   *   A flat array of relative directory paths.
   *
   * @return array
   *   An array of absolute directory paths, based on $this->base_path.
   */
  public function prependJusticeFilepath(array $relative_paths) {

    $absolute_paths = array();
    foreach ($relative_paths as $key => $relative_path) {
      $absolute_paths[$key] = $this->baseDir . '/' . $relative_path;
    }

    return $absolute_paths;
  }

  /**
   * Generates a legacy file path based on a row's file id.
   *
   * @param object $row
   *   The row being imported.
   */
  public function generateLegacyPath($row) {
    // $row->url_path can be used as an identifer, whereas $row->legacy_path
    // may have multiple values.
    $row->url_path = substr($row->fileid, 1);
    $row->legacy_path = $row->url_path;

    $this->handleIndexRedirect($row);
  }

  /**
   * Handles generation of redirects for index.php or index.html pages.
   */
  public function handleIndexRedirect($row) {
    // Assume not an index until proven otherwise.
    $row->isIndex = FALSE;
    // Check to see if $row->fileid == 'index.html' or 'index.php'.
    if (preg_match('/index\.(htm(l?)|php)/', $row->fileid, $matches)) {
      $row->isIndex = TRUE;
      // Determine root path. E.g., oip/index.html would have the root path
      // /oip.
      $root_path = ltrim(rtrim(str_replace($matches[0], '', $row->fileid), '/'), '/');

      // Check to see if this alias already exists. E.g., if we are migrating
      // oip/index.html, see if an alias exists for /oip.
      if ($system_path = drupal_lookup_path('source', $root_path)) {
        // Continuing /oip example: if an alias does exist for /oip, see if
        // the system path points to an organization node.
        $node = menu_get_object($type = 'node', $position = 1, $system_path);
        if ($node->type == 'organization') {
          // Check to see if a redirect already exists for this path.
          $redirect = redirect_load_by_source($row->legacy_path);
          if (!$redirect) {
            // Create a redirect from /oip/index.html to the organization node.
            $redirect = new stdClass();
            redirect_object_prepare($redirect);
            $redirect->source = $row->legacy_path;
            $redirect->redirect = $system_path;

            redirect_save($redirect);

            // Remove legacy_patch from this node.
            unset($row->legacy_path);
          }
        }
      }
      else {
        $row->legacy_path .= '|' . $root_path;
      }
    }
  }


  /**
   * {@inheritdoc}
   *
   * @param array $report_rows
   *   Additional rows of information to be added to the migration report.
   */
  public function postImport($report_rows = array()) {
    parent::postImport();

    // @todo Move this to NGJusticeHtmlToNodeMigration after
    // JusticeHtmlToNodeMigration is completely deprecated.
    if (is_subclass_of($this, 'JusticeHtmlToNodeMigration') || (is_subclass_of($this, 'NGJusticeHtmlToNodeMigration'))) {
      $this->printNodeMigrationReport($report_rows);
    }
  }

  /**
   * Prints a migration report via drush.
   *
   * @param array $report_rows
   *   Additional rows of information to be added to the migration report.
   */
  public function printNodeMigrationReport($report_rows = array()) {
    $duplicates = $this->countDuplicateTitles();
    $output_rows = array(
      array(t('Total records'), $this->sourceCount()),
      array(t('Migrate messages'), $this->messageCount()),
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

    return (int) $query->fetchField();
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
