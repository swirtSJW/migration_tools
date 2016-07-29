<?php

/**
 * @file
 * Parses static HTML files via queryPath to a non-row.
 */

namespace CustomMigrate\SourceParser;

/**
 * Class SourceParser\SecondaryParser
 *
 * Used for parsing secondary content (an additional page) for adding to the
 * primary page.
 *
 * @package migrate_fcc
 */
class SecondaryParser extends \MigrationTools\SourceParser\HtmlBase {
  /**
   * {@inheritdoc}
   */
  public function __construct($file_id, $html) {
    // We are making a fake row.
    $row = new \stdClass();
    parent::__construct($file_id, $html, $row);

    // Override the base modifier (optional).
    $this->modifier = new \CustomMigrate\Modifier\ModifyHtml($this->queryPath);
  }

  /**
   * Validate basic requirements and alert if needed.
   */
  protected function validateParse() {
    // A body is not required, but should be cause for alarm.
    if (empty($this->row->body)) {
      \MigrationTools\Message::make("The body for @fileid is empty.", array("@fileid" => $this->fileId), \WATCHDOG_ALERT);
    }
  }


  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainerJobs() {
    // For now we are just going to grab a body.
    $body = new \MigrationTools\Obtainer\Job('body', 'ObtainBody', TRUE);
    $this->addObtainerJob($body);
  }

  /**
   * Clean and alter the html within $this->queryPath.
   */
  protected function cleanQueryPathHtml() {
    parent::cleanQueryPathHtml();
  }
}
