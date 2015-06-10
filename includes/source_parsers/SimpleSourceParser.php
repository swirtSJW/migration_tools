<?php

/**
 * @file
 * Includes SourceParser class, which parses static HTML files via queryPath.
 */

/**
 * Class SimpleSourceParser.
 *
 * @package doj_migration
 */
class SimpleSourceParser extends NGSourceParser {

  /**
   * {@inheritdoc}
   */
  public function __construct($file_id, $html) {
    parent::__construct($file_id, $html);

    $this->cleanHtml();
  }

  /**
   * Set the html var after some cleaning.
   *
   * @todo this is specific to justice so it should not be here.
   */
  protected function cleanHtml() {
    try {
      $this->initQueryPath();
      HtmlCleanUp::convertRelativeSrcsToAbsolute($this->queryPath, $this->fileId);
      HtmlCleanUp::removeFaultyImgLongdesc($this->queryPath);

      // Clean up specific to the Justice site.
      HtmlCleanUp::stripOrFixLegacyElements($this->queryPath);
    }
    catch (Exception $e) {
      $this->sourceParserMessage('@file_id Failed to clean the html, Exception: @error_message', array('@file_id' => $this->fileId, '@error_message' => $e->getMessage()), WATCHDOG_ERROR);
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
  }

}
