<?php
/**
 * @file
 * Base class to add support for all html migrations.
 */

namespace CustomMigrate\Html;

/**
 * Base handling for migrating into nodes from html files.
 */
abstract class NodeMigration extends \MigrationTools\Migration\HtmlFileBase {
  private $rowAggregator;
  private $relevantSourceFields = array();

  /**
   * {@inheritdoc}
   */
  public function __construct(array $arguments) {
    $this->mergeArguments($arguments);
    // Add any arguments that are specific to this migration.
    $arguments = array(
      'redirect_detect_texts' => array(
        'This page has been moved to',
        'from the redesigned version of the website',
      ),
      // Rewrite any relative links in the content.
      'rewrite_link_base' => 'https://old-site.com/',
      'rewrite_link_base_alters' => array(
        // Change internal links to be https (optional).
        'http://old-site.com/' => 'https://old-site.com/',
        // Changing domain name for this site (optional).
        'https://old-site.com/' => 'https://new-site.com/',
      ),
      // Rewrite any image src URLs in the content.
      'rewrite_image_base' => 'https://old-site.com/',
      'rewrite_image_base_alters' => array(
        'http://old-site.com/' => 'https://old-site.com/',
      ),
      // Rewrite any script src URLs in the content.
      'rewrite_script_base' => 'https://old-site.com/',
      'rewrite_script_base_alters' => array(
        'http://old-site.com/' => 'https://old-site.com/',
      ),
      // Rewrite any non-image file links in the content.
      'rewrite_file_base' => 'https://old-site.com/',
      'rewrite_file_base_alters' => array(
        'http://old-site.com/' => 'https://old-site.com/',
      ),

    );
    $this->mergeArguments($arguments);
    parent::__construct($this->getArguments());

    // The destination is the page content type.
    $this->destination = new \MigrateDestinationNode($this->getArgument('destination_type'));
    $this->map = new \MigrateSQLMap($this->machineName,
        array(
          'fileId' => array(
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
          ),
        ),
        \MigrateDestinationNode::getKeySchema()
      );

    $fields = array(
      // Destination field => Source field, or just destination field.
      'title',
      'body',
    );
    $this->addFieldMappings($fields);

    $this->addFieldMapping('uid')
      ->defaultValue($this->getArgument('uid'));
    $this->addFieldMapping('language')
      ->defaultValue($this->getArgument('default_language'))
      ->callbacks(array($this, 'forceLanguageEnglish'));

    $this->addFieldMapping('status')
      ->defaultValue($this->getArgument('published'));

    $this->addFieldMapping('body:language')
      ->defaultValue($this->getArgument('default_language'))
      ->callbacks(array($this, 'forceLanguageEnglish'));
    $this->addFieldMapping('body:summary', 'body_summary');

    // Define non-simple field mappings.
    $this->addFieldMapping('workbench_moderation_state_new')
        ->defaultValue($this->getArgument('worbench_state_default'));
    $this->addFieldMapping('pathauto', NULL, FALSE)
      ->description('Forcing content on the new site to use pathauto.')
      ->defaultValue($this->getArgument('pathauto'));

  }

  /**
   * Add source fields that we are not migrating to irrelevantSourceFields.
   *
   * This is useful for analysis and reporting, and this function also
   * automatically adds those source fields to unmigrated sources.
   *
   * @param array $source_fields
   *   An array of strings where each string maps to a source field.
   */
  protected function addIrrelevantSourceFields($source_fields) {
    $this->addUnmigratedSources($source_fields, t('DNM'));
    $this->irrelevantSourceFields = array_merge($this->irrelevantSourceFields, $source_fields);
  }

  /**
   * Add  the source fields that we are migrating to relevantSourceFields.
   *
   * This array (relevantSourceFields) is only used for migration analysis and
   * reporting.
   *
   * @param array $source_fields
   *   An array of strings where each string maps to a source field.
   */
  protected function addRelevantSourceFields($source_fields) {
    $this->relevantSourceFields = array_merge($this->relevantSourceFields, $source_fields);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    if (empty($row->pathing->redirectDestination)) {
      $row->pathing->redirectDestination = \MigrationTools\Url::hasValidRedirect($row, $this->sourceParser->queryPath, $this->getArgument('redirect_detect_texts'));
    }

    if ($row->pathing->redirectDestination === 'skip') {
      // There is a redirect but it is broken.  Skip migrating this page.
      return \MigrationTools\Message::makeSkip('broken redirect detected', $row->fileId, WATCHDOG_ERROR);
    }
    elseif ($row->pathing->redirectDestination) {
      // There is a redirect destination for this page so build it.
      \MigrationTools\Url::createRedirectsMultiple($row->pathing->redirectSources, $row->pathing->redirectDestination);
      $message = '- @fileid  -> Skipped and Redirected to: @destination';
      $variables = array(
        '@fileid' => $row->fileId,
        '@destination' => $row->pathing->redirectDestination,
      );
      watchdog("Migration: " . $this->getArgument('machine_name'), $message, $variables, WATCHDOG_INFO);

      return FALSE;
    }

    // Rewrite links and urls.
    \MigrationTools\Url::rewriteScriptSourcePaths($this->sourceParser->queryPath, $this->getArgument('rewrite_script_base_alters'), $row->fileId, $this->getArgument('rewrite_script_base'));
    \MigrationTools\Url::rewriteImageHrefsOnPage($this->sourceParser->queryPath, $this->getArgument('rewrite_image_base_alters'), $row->fileId, $this->getArgument('rewrite_image_base'));
    \MigrationTools\Url::rewriteAnchorHrefsToBinaryFiles($this->sourceParser->queryPath, $this->getArgument('rewrite_file_base_alters'), $row->fileId, $this->getArgument('rewrite_file_base'));
    \MigrationTools\Url::rewriteAnchorHrefsToPages($this->sourceParser->queryPath, $this->getArgument('rewrite_link_base_alters'), $row->fileId, $this->getArgument('rewrite_link_base'));

    // Note: The final migration class should be the one to call parse().
    // $this->sourceParser->parse();
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    // Set the format of the body field.
    \MigrationTools\Node::reassignBodyFilter($entity, 'full_html');

    // Check to see if pathauto is making the path, or not.
    if ($this->getArgument('use_pathauto')) {
      // Use Pathauto.
      $entity->path['pathauto'] = 1;
    }
    else {
      $entity->path['pathauto'] = 0;
      $entity->path['alias'] = $row->pathing->destinationUriAlias;
    }

  }

  /**
   * {@inheritdoc}
   */
  public function complete($node, $row) {
    parent::complete($node, $row);

    $this->rowAggregator->printEntityReport($this, $node);

    // Uncomment this to see what the pathing looks like.
    // $row->pathing->debug("In NodeMigration::complete()");
  }

  /**
   * {@inheritdoc}
   */
  public function preImport() {
    parent::preImport();
    \MigrationTools\Message::makeSeparator();
    $message = "Pre Import Processing";
    \MigrationTools\Message::make($message, array(), FALSE, 0);
  }


  /**
   * {@inheritdoc}
   */
  public function postImport() {
    parent::postImport();
    \MigrationTools\Message::makeSeparator();
    $message = "Post Import Processing";
    \MigrationTools\Message::make($message, array(), FALSE, 0);

    if (isset($this->rowAggregator)) {
      $this->rowAggregator->postImport($this->machineName);
    }
  }

  /**
   * Called from complete(), builds and returns destination Uri for an entity.
   *
   * @param object $entity
   *   The entity that was just saved.
   *
   * @param object $row
   *   The row that was just migrated.
   *
   * @return string
   *   Example node/123, user/123, taxonomy/term/123.
   */
  public function buildDestinationUri($entity, $row) {
    if (!empty($entity->nid)) {
      $destination = "node/{$entity->nid}";
      $row->pathing->destinationUriRaw = $destination;
      return $destination;
    }
    else {
      // Something went wrong and we have no node id for the saved entity.
      // Throw a migration exception.
      $message = $t("Unable to build DestinationUrl without a nid in @sourceid", array('@sourceid' => $row->fileId));
      throw new \MigrateException($message);
    }
  }


  /**
   * Gets the correct Workbench state based on status value.
   *
   * @param bool $value
   *   The value associated with the incoming legacy node status.
   *
   * @return string
   *   The correct workbench state.
   */
  public function determineWorkbenchState($value) {
    if (!empty($value)) {
      return 'published';
    }
    else {
      return '';
    }
  }

  /**
   * Row element callback to convert undefined languages to english.
   *
   * @param string $value
   *   The language value (und, es, en).
   *
   * @return string
   *   The mapped language to use.
   */
  public function forceLanguageEnglish($value) {
    if (empty($value) || ($value == \LANGUAGE_NONE)) {
      $language = 'en';
    }
    else {
      $language = $value;
    }
    return $language;
  }
}
