<?php

/**
 * @file
 * Contains HtmlFileBase migration class for all  html file migrations.
 */

namespace MigrationTools\Migration;

/**
 * Abstract intermediate class holding common processes for html file migration.
 *
 * @package migration_tools
 */
abstract class HtmlFileBase extends Base {

  /**
   *  Array of full or partial paths to skip.
   *
   * @var array
   *   Ex:
   *   full - 'oldsite/section/bad-directory/'
   *   partial - 'bad-directory'
   */
  public $skipDirectories = array();

  /**
   * Array of specific fileIds to skip.
   *
   * @var array
   *   Ex: '/oldsite/section/subsection/skip-this.html'
   */
  public $skipFiles = array();
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->mergeArguments($arguments);

    // Add any arguments that are specific to this migration.
    $arguments = array(
      'source_type' => 'file',
      'default_files' => array('index', 'default'),
    );
    $this->mergeArguments($arguments);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    // This method is called and builds each row/item of the migration.
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    \MigrationTools\Message::makeSeparator();
    \MigrationTools\Message::make("Processing: @id", array('@id' => $row->fileId), FALSE, 0);

    if (\MigrationTools\CheckFor::isInPath($row->fileId, $this->skipDirectories)) {
      // This content is within a directory to be skipped.
      $message = '- @fileid  -> Skipped: within a skipped directory.';
      \MigrationTools\Message::make($message, array('@fileid' => $row->fileId), WATCHDOG_INFO, 1);
      return FALSE;
    }

    if (\MigrationTools\CheckFor::isSkipFile($row->fileId, $this->skipFiles)) {
      // This file is within a directory to be skipped.
      return FALSE;
    }

    // Build pathing properties.
    $row->pathing = new \MigrationTools\Url($row->fileId, $this->pathingLegacyDirectory, $this->pathingLegacyHost, $this->pathingRedirectCorral, $this->pathingSectionSwap, $this->pathingSourceLocalBasePath);

    // Check whether a redirect to this page already exists, if so, do not
    // migrate the "duplicate" content. Whatever migrated it first wins.
    if (\MigrationTools\CheckFor::isDuplicateByRedirect($row->pathing->corralledUri)) {
      // This record already exists so bail on the migration.
      return FALSE;
    }

    // Set the page location as one redirect source.
    $row->pathing->redirectSources[] = $row->pathing->corralledUri;

    // Set the directory as one source if the file is a default file.
    $default_filenames = $this->getArgument('default_files');
    $row->pathing->redirectSources[] = \MigrationTools\Url::getRedirectIfIndex($row->pathing->corralledUri, $default_filenames);

    // Set destination if this file explicitly redirected to some place else.
    $row->pathing->redirectDestination = \MigrationTools\CheckFor::isSkipAndRedirectFile($row->fileId, $this->skipFilesAndRedirect);

    if (empty($row->pathing->redirectDestination)) {
      // There is no defined redirect yet from outsite the page, so it is time
      // to instantiate the querypath source parser.
      $source_parser = $this->getArgument('source_parser');

      $this->sourceParser = new $source_parser($row->fileId, $row->filedata, $row);
    }
    else {
      // There is a redirect destination for this page so build it.
      \MigrationTools\Url::createRedirectsMultiple($row->pathing->redirectSources, $row->pathing->redirectDestination);
      $message = 'Migration:' . $this->getArgument('machine_name') . '- @fileid  -> Skipped and Redirected to: @destination';
      $variables = array(
        '@fileid' => $row->fileId,
        '@destination' => $row->pathing->redirectDestination,
      );

      \MigrationTools\Message::make($message, $variables, WATCHDOG_INFO, 1);

      return FALSE;
    }
  }


  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    // This method is called after the entity has been saved.

    // Build any redirects.
    $destination = $this->buildDestinationUri($entity, $row);
    if (!empty($row->pathing->redirectSources)) {
      \MigrationTools\Url::createRedirectsMultiple($row->pathing->redirectSources, $row->pathing->destinationUriRaw);
    }
    \MigrationTools\Message::make("Path: @path", array('@path' => $row->pathing->destinationUriRaw), FALSE, 1);

    // Report on the pathing.
    if (!empty($entity->path)) {
      if (empty($entity->path['pathauto']) && !empty($entity->path['alias'])) {
        \MigrationTools\Message::make("Alias (custom): @alias", array('@alias' => $entity->path['alias']), FALSE, 1);
      }
      elseif (!empty($entity->path['pathauto']) && !empty($entity->path['alias'])) {
        \MigrationTools\Message::make("Alias (pathauto): @alias", array('@alias' => $entity->path['alias']), FALSE, 1);
      }
      else {
        \MigrationTools\Message::make("Alias: none created", array(), FALSE, 1);
      }
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
  abstract public function buildDestinationUri($entity, $row);
}
