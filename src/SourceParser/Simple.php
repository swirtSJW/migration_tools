<?php

/**
 * @file
 * Includes Simple source parser class, parses static HTML files via queryPath.
 */

namespace MigrationTools\SourceParser;

/**
 * Class Simple source parser.
 *
 * @package migration_tools
 */
class Simple extends Base {

  /**
   * {@inheritdoc}
   */
  public function __construct($file_id, $html) {
    parent::__construct($file_id, $html);

    $this->cleanHtml();
  }

  /**
   * Set the html var after some cleaning.
   */
  protected function cleanHtml() {
    try {
      $this->initQueryPath();
      HtmlCleanUp::convertRelativeSrcsToAbsolute($this->queryPath, $this->fileId);

      // Clean up specific to this site.
      HtmlCleanUp::stripOrFixLegacyElements($this->queryPath);
    }
    catch (Exception $e) {
      MigrationMessage::makeMessage('@file_id Failed to clean the html, Exception: @error_message', array('@file_id' => $this->fileId, '@error_message' => $e->getMessage()), WATCHDOG_ERROR);
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
  }

}
