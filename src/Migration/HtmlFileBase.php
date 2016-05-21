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
      watchdog('migration_tools', $message, array('@fileid' => $row->fileId), WATCHDOG_WARNING);
      return FALSE;
    }

    if (\MigrationTools\CheckFor::isSkipFile($row->fileId, $this->skipFiles)) {
      // This file is within a directory to be skipped.
      return FALSE;
    }

    // Build pathing properties.
    $row->pathing = new \MigrationTools\Url($row->fileId, $this->pathingLegacyDirectory, $this->pathingLegacyHost, $this->pathingRedirectCorral, $this->pathingSectionSwap, $this->pathingSourceLocalBasePath);
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
