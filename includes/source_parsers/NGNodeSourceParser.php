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
  // @codingStandardsIgnoreStart
  protected $content_type;
  // @codingStandardsIgnoreEnd

  /**
   * Getter.
   */
  public function getTitle() {
    $title = $this->getProperty('title');
    if (empty($title)) {
      $this->sourceParserMessage("The title for @fileid is empty.", array("@fileid" => $this->fileId), WATCHDOG_ALERT);
    }
    return $title;
  }

  /**
   * Getter.
   */
  public function getBody() {
    $this->cleanHtml();
    $body = $this->getProperty('body');
    if (empty($body)) {
      $this->sourceParserMessage("The body for @fileid is empty.", array("@fileid" => $this->fileId), WATCHDOG_ALERT);
    }
    return $body;
  }

  /**
   * Getter.
   */
  public function getContentType() {
    return $this->getProperty('content_type');
  }

  /**
   * Set the html var after some cleaning.
   *
   * @todo this is specific to justice so it should not be here.
   */
  protected function cleanHtml() {
    try {
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
    $type = new ObtainerInfo("content_type");
    $this->addObtainerInfo($type);

    $title = new ObtainerInfo("title");
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('pluckSelector', array("title", 1));
    $title->addMethod('pluckSelector', array("h1", 1));
    $this->addObtainerInfo($title);

    $body = new ObtainerInfo("body");
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');
    $this->addObtainerInfo($body);
  }

}
