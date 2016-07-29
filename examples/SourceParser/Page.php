<?php

/**
 * @file
 * Includes Node class, parses static HTML files via queryPath.
 */

namespace CustomMigrate\SourceParser;

/**
 * Class SourceParser\Page
 *
 * Used for parsing content into the content type 'page' fields
 *
 * @package custom_migrate
 */
class Page extends \MigrationTools\SourceParser\Node {
  /**
   * {@inheritdoc}
   */
  public function __construct($file_id, $html, &$row) {
    parent::__construct($file_id, $html, $row);

    // Override the base modifier if needed.
    $this->modifier = new \CustomMigrate\Modifier\FccModifyHtml($this->queryPath);
  }

  /**
   * Validate basic requirements and alert if needed.
   *
   * Can also run any logic that requires that the entire parse is completed.
   */
  protected function validateParse() {
    // An empty title should throw an error.
    if (empty($this->row->title)) {
      \MigrationTools\Message::make("The title for @fileid is empty.", array("@fileid" => $this->fileId), \WATCHDOG_ALERT);
    }

    // A body is not required, but should be cause for alarm if it is empty.
    if (empty($this->row->body)) {
      \MigrationTools\Message::make("The body for @fileid is empty.", array("@fileid" => $this->fileId), \WATCHDOG_ALERT);
    }

    // Custom handling to come up with a date if the page content has none.
    // If no date, use the file info as the updated date.
    if (empty($this->row->field_date_updated_reviewed)) {
      $this->row->field_date_updated_reviewed = $this->row->fileDate;

    }

    // Set Drupal created date to the updated date from the page if available.
    if (!empty($row->field_date_updated_reviewed)) {
      $row->created = strtotime($row->field_date_updated_reviewed);
    }
  }


  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainerJobs() {
    // Content type 'page' nodes have a title , updated date, type and a body.
    // Other SourceParsers can extend this.

    $title = new \MigrationTools\Obtainer\Job('title', 'ObtainTitle');
    $this->addObtainerJob($title);
    // Searches can be added to a Job here but it makes them difficult to step
    // in front of if you need to change the order.  So it is better to set add
    // searches in prepareRow() of your migration class.

    $updated_date = new \MigrationTools\Obtainer\Job('field_date_updated_reviewed', 'ObtainDate');
    $this->addObtainerJob($updated_date);

    $page_type = new \MigrationTools\Obtainer\Job('field_basic_page_type', 'ObtainContentType');
    $this->addObtainerJob($page_type);

    $body = new \MigrationTools\Obtainer\Job('body', 'ObtainBody', TRUE);
    $this->addObtainerJob($body);
  }

  /**
   * Clean and alter the html within $this->queryPath.
   *
   * Any custom cleaning can be done here.  This runs after ObtainerJobs that
   * were instantiated with $after_clean = FALSE and before ObtainerJobs that
   * were instantiated with $after_clean = TRUE.
   */
  protected function cleanQueryPathHtml() {
    parent::cleanQueryPathHtml();
  }
}
