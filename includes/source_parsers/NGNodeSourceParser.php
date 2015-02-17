<?php

/**
 * @file
 * Includes SourceParser class, which parses static HTML files via queryPath.
 */

/**
 * Class NGNodeSourceParser.
 *
 * @package doj_migration
 */
class NGNodeSourceParser extends NGSourceParser {
  protected $body;
  protected $title;

  /**
   * Getter.
   */
  public function getTitle() {
    return $this->getProperty('title');
  }

  /**
   * Getter.
   */
  public function getBody() {
    $this->cleanHtml();
    return $this->getProperty('body');
  }

  /**
   * Set the html var after some cleaning.
   *
   * @todo this is specific to justice so it should not be here.
   */
  protected function cleanHtml() {
    try {
      HtmlCleanUp::convertRelativeSrcsToAbsolute($this->queryPath, $this->fileId);

      // Clean up specific to the Justice site.
      HtmlCleanUp::stripOrFixLegacyElements($this->queryPath);
    }
    catch (Exception $e) {
      $this->sourceParserMessage('Failed to clean the html, Exception: @error_message', array('@error_message' => $e->getMessage()), WATCHDOG_ERROR);
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObatinersInfo() {
    $title = new ObtainerInfo("title");
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findTitleTag');
    $title->addMethod('findH1First');
    $this->addObtainerInfo($title);

    $body = new ObtainerInfo("body");
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');
    $this->addObtainerInfo($body);
  }
}
