<?php

/**
 * @file
 * Includes Node class, parses static HTML files via queryPath.
 */

namespace MigrationTools\SourceParser;

/**
 * Class SourceParser\Node
 *
 * @package migration_tools
 */
class Node extends HtmlBase {
  /**
   * {@inheritdoc}
   */
  public function __construct($file_id, $html, &$row) {
    parent::__construct($file_id, $html, $row);

  }

  /**
   * Validate basic requirements and alert if needed.
   */
  protected function validateParse() {
    // An empty title should throw an error.
    if (empty($this->row->title)) {
      \MigrationTools\Message::make("The title for @fileid is empty.", array("@fileid" => $this->fileId), \WATCHDOG_ALERT);
    }

    // A body is not required, but should be cause for alarm.
    if (empty($this->row->body)) {
      \MigrationTools\Message::make("The body for @fileid is empty.", array("@fileid" => $this->fileId), \WATCHDOG_ALERT);
    }
  }


  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainerJobs() {
    // Basic nodes will only have a title and a body.  Other SourceParsers can
    // extend this and additional Searches can be added in prepareRow.
    $title = new \MigrationTools\Obtainer\Job('title', 'ObtainTitle');
    $title->addSearch('pluckSelector', array("h1", 1));
    $title->addSearch('pluckSelector', array("title", 1));
    $this->addObtainerJob($title);

    $body = new \MigrationTools\Obtainer\Job('body', 'ObtainBody', TRUE);
    $this->addObtainerJob($body);
  }
}
