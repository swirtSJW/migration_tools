<?php
/**
 * @file
 * Class for migration of AAA local files to Page in EN.
 */

namespace CustomMigrate\Html;

/**
 * Migrating AAA into Page nodes from html files.
 */
class AaaToPageMigration extends PageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct(array $arguments) {
    // Merge in initial arguments so they establish precedent.
    $this->mergeArguments($arguments);
    // Add any arguments that are specific to this migration.
    $arguments = array(
      'description' => t('Migration of AAA html files to D7 Pages.'),
      'source_type' => 'file',
      'destination_type' => 'page',
      'default_language' => 'en',
      'source_parser' => '\CustomMigrate\SourceParser\Page',
      'default_files' => array('index', 'default', 'welcome'),
      // Make content author John Doe.
      'uid' => 1,
      // Specify the group to assign it to group nid = 0000 (change the value).
      'og_nid' => array(0000),
      'use_pathauto' => TRUE,
      // These two states need to agree.
      'published' => FALSE,
      'worbench_state_default' => 'needs_review',

    );
    $this->mergeArguments($arguments);

    parent::__construct($this->getArguments());

    // Location of the section of the site you are trying to migrate.
    $this->pathingLegacyDirectory = 'transition/bureaus/aaa';

    $this->pathingLegacyHost = 'https://old-site.com';
    $this->pathingRedirectCorral = 'redirect-old-site';
    $this->pathingSectionSwap = array();
    $this->pathingSourceLocalBasePath = variable_get('migration_tools_source_directory_base', NULL);

    // Identify the source.
    $regex = NULL;
    $scan_options = array(
      // Block content from directories named '_vti_cnf' and 'deleted'.
      'nomask' => '#^(_vti_cnf|deleted)$#',
    );

    // Any directories listed in this array will not be migrated or redirected.
    // Better to handle them in the nomask regex if possible.
    // 'nomask' only evaluates each directory name 1 at a time.  While
    // skipDirectories can limit by complex paths like 'subsection/common/a'.
    $this->skipDirectories = array();

    $source_directories = array($this->pathingLegacyDirectory);

    $source = new \MigrationTools\Source\HtmlFile($source_directories, $regex, $scan_options, $this->pathingSourceLocalBasePath);
    $this->source = $source->getSource();

    // Any fileIds listed in this array will not be migrated or redirected.
    $this->skipFiles = array(
      // Ordinary pages not being migrated.

      // Hand migrated items. @TODO build the redirect when we know the path.

    );

    // Any files listed in keys of this array not have to be migrated, but will
    // have a redirect built to the value of that array element.
    $this->skipFilesAndRedirect = array(
      // Pattern 'file_id' => 'destination URI'.
    );

  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Remove the following selectors from the page.
    $this->sourceParser->htmlElementsToRemove = array(
      '.pagedate',
      'head',
    );

    // Rewrap the following elements.
    $this->sourceParser->htmlElementsToReWrap = array(
      // Old wrapper => New wrapper.
    );

    $this->sourceParser->obtainerJobs['title']
      ->addSearch('pluckSelector', array('h1', 1))
      ->addSearch('pluckSelector', array('title', 1));

    $this->sourceParser->obtainerJobs['field_date_updated_reviewed']
      ->addSearch('pluckAndFilterForwardDate', array(".pagedate"));

    $this->sourceParser->obtainerJobs['body']
      ->addSearch('findTopBodyHtml');

    // Modifiers run inbetween obtainer jobs that run pre-clean
    // (like title) and obtainer jobs that run post-clean (like body).
    $this->sourceParser->modifier
      ->addModifier('removeSelectorAll', array('.topnavlinks'))
      ->addModifier('removeEmptyTables');

    // Add any Searches and Modifiers, before calling parse() because
    // it runs them.
    $this->sourceParser->parse();

    // Use this to alter any path for migrations not relying on pathauto.
    // $row->pathing->generateDestinationUriAlias(array(), $row->title);
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    parent::prepare($entity, $row);

  }

  /**
   * {@inheritdoc}
   */
  public function complete($node, $row) {
    parent::complete($node, $row);

  }

  /**
   * {@inheritdoc}
   */
  public function preImport() {
    parent::preImport();
    // Code to execute before the first row is processed.

    // Calling this will import a csv based list of redirects.
    // \HookUpdateDeployTools\Redirects::import('aaa-redirects');
  }

  /**
   * {@inheritdoc}
   */
  public function postImport() {
    parent::postImport();
    // Code to execute after the last row has been imported.
  }
}
