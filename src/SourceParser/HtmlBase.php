<?php

/**
 * @file
 * Defines SourceParser\Base class, that parses static HTML files via queryPath.
 */

namespace MigrationTools\SourceParser;

/**
 * Class SourceParser\Base.
 *
 * @package migration_tools
 */
abstract class HtmlBase {

  public $obtainerJobs = array();
  public $fileId;
  protected $html;
  public $row;
  public $queryPath;

  /**
   * A source parser class should set a useful set of obtainer jobs.
   *
   * These should be the basics for a given content type.  More searches can be
   * added to the source parser before parse() is called.
   */
  abstract protected function setDefaultObtainerJobs();

  /**
   * A source parser class should perform any checks on required properties.
   *
   * Appropriate messages should be output.
   * This will be called at the end of parse().
   */
  abstract protected function validateParse();

  /**
   * Constructor.
   *
   * @param string $file_id
   *   The file id, e.g. careers/legal/pm7205.html
   * @param string $html
   *   The full HTML data as loaded from the file.
   * @param object $row
   *   Migrate row to be altered by reference.
   */
  public function __construct($file_id, $html, &$row) {
    $this->fileId = $file_id;
    $this->row = $row;

    $html = \MigrationTools\StringTools::fixEncoding($html);
    $html = \MigrationTools\StringTools::stripWindowsCRChars($html);
    $html = \MigrationTools\StringTools::fixWindowSpecificChars($html);
    $html = \MigrationTools\StringTools::removePhp($html);
    $this->html = $html;
    $this->initQueryPath();

    $this->setDefaultObtainerJobs();
  }

  /**
   * Add obtainer job for this source parser to run.
   */
  public function addObtainerJob(\MigrationTools\Obtainer\Job $job) {
    $this->obtainerJobs[$job->getProperty()] = $job;
  }

  /**
   * Getter.
   */
  public function getObtainerJobs() {
    return $this->obtainerJobs;
  }

  /**
   * Parse the page by running all the ObtainerJobs.
   *
   * This should be called after all obtainer jobs have been added.
   */
  public function parse() {
    // Run all Obtainer jobs.
    if (isset($this->obtainerJobs) && is_array($this->obtainerJobs)) {
      // Since all other items are picked out of the body, body must run last.
      // Run any jobs that do not require cleaning that are not body.
      foreach ($this->obtainerJobs as $job) {
        $property = $job->getProperty();
        if (!$job->afterClean && ($property !== 'body')) {
          $this->row->{$property} = $this->getProperty($property);
        }
      }
      // Clean the html for any obtainerJobs that require clean html.
      $this->cleanQueryPathHtml();

      // Run any jobs that do require cleaning.
      foreach ($this->obtainerJobs as $job) {
        $property = $job->getProperty();
        if ($job->afterClean && ($property !== 'body')) {
          $this->row->{$property} = $this->getProperty($property);
        }
      }
      // Now that everything else has been run.  Grab what's left for the body.
      if (!empty($this->obtainerJobs['body'])) {
        $this->row->body = $this->getProperty('body');
      }

      $this->validateParse();
    }
  }

  /**
   * Get information/properties from html by running the obtainers.
   */
  protected function getProperty($property) {
    if (!isset($this->{$property})) {
      $this->setProperty($property);
    }

    // We can just return the property as any issue should throw an exception
    // form setProperty.
    return $this->{$property};
  }

  /**
   * Set a property.
   */
  protected function setProperty($property) {
    // Make sure our QueryPath object has been initialized.
    $this->initQueryPath();
    // Obtain the property using obtainers.
    $this->{$property} = $this->obtainProperty($property);
  }

  /**
   * Use the obtainers mechanism to extract text from the html.
   */
  protected function obtainProperty($property) {
    $text = '';
    $job = (empty($this->obtainerJobs[$property])) ? '' : $this->obtainerJobs[$property];

    if (empty($job)) {
      $message = t("@class does not have a Job defined for the  property: @property", array('@property' => $property, '@class' => 'SourceParser\HtmlBase'));
      throw new \Exception($message);
    }

    try {
      $class = $job->getClassShortName();
      $searches = $job->getSearches();
      if (!empty($searches)) {
        // There are methods to run, so run them.
        \MigrationTools\Message::make("Obtaining @key via @obtainer_class", array('@key' => $property, '@obtainer_class' => $class));

        $text = $job->run($this->queryPath);
        $length = strlen($text);
        if (!$length) {
          // Nothing was obtained.
          \MigrationTools\Message::make('@property NOT found', array('@property' => $property), \WATCHDOG_DEBUG, 2);
        }
        elseif ($length < 256) {
          // It is short enough to be helpful in debug output.
          \MigrationTools\Message::make('@property found --> @text', array('@property' => $property, '@text' => $text), \WATCHDOG_DEBUG, 2);
        }
        else {
          // It's too long to be helpful in output so just show the length.
          \MigrationTools\Message::make('@property found --> Length: @length', array('@property' => $property, '@length' => $length), \WATCHDOG_DEBUG, 2);
        }
      }
      else {
        // There were no methods to run so message.
        \MigrationTools\Message::make("There were no searches to run for @key via @obtainer_class so it was not executed", array('@key' => $property, '@obtainer_class' => $class));
      }

    }
    catch (Exception $e) {
      \MigrationTools\Message::make("@file_id Failed to set @key, Exception: @error_message", array(
        '@file_id' => $this->fileId,
        '@key' => $property,
        '@error_message' => $e->getMessage(),
      ), \WATCHDOG_ERROR);
    }

    return $text;
  }

  /**
   * Create the queryPath object.
   */
  protected function initQueryPath() {
    if (isset($this->queryPath)) {
      // QueryPath is already initialized.
      return;
    }
    else {
      // Initialize the QueryPath.
      $type_detect = array(
        'UTF-8',
        'ASCII',
        'ISO-8859-1',
        'ISO-8859-2',
        'ISO-8859-3',
        'ISO-8859-4',
        'ISO-8859-5',
        'ISO-8859-6',
        'ISO-8859-7',
        'ISO-8859-8',
        'ISO-8859-9',
        'ISO-8859-10',
        'ISO-8859-13',
        'ISO-8859-14',
        'ISO-8859-15',
        'ISO-8859-16',
        'Windows-1251',
        'Windows-1252',
        'Windows-1254',
      );
      $convert_from = mb_detect_encoding($this->html, $type_detect);
      if ($convert_from != 'UTF-8') {
        // This was not UTF-8 so report the anomaly.
        $message = "Converted from: @convert_from";
        \MigrationTools\Message::make($message, array('@convert_from' => $convert_from), \WATCHDOG_INFO, 1);
      }

      $qp_options = array(
        'convert_to_encoding' => 'UTF-8',
        'convert_from_encoding' => $convert_from,
      );

      // Create query path object.
      try {
        // QueryPath is need as part of this migration but is not a full
        // dependency for this module.  It can be included as the Drupal
        // querypath module or as a library.
        if (function_exists('qp')) {
          try {
            // The QueryPAth qp is less destructive than htmlqp so try it first.
            $this->queryPath = qp($this->html, NULL, $qp_options);
          }
          catch (\Exception $e) {
            // QueryPath qp is less tolerant of badly formed html so it must
            // have failed.
            // Use htmlqp which is more detructive but will fix bad html.
            \MigrationTools\Message::make('Failed to instantiate QueryPath using qp, attempting qphtml, Exception: @error_message', array('@error_message' => $e->getMessage()), FALSE);
            $this->queryPath = htmlqp($this->html, NULL, $qp_options);
          }
        }
        else {
          $message = "QueryPath is required for html source parsing.  Please install the querypath module or add the library.";
          throw new \MigrateException($message);
        }
      }
      catch (\Exception $e) {
        \MigrationTools\Message::make('Failed to instantiate QueryPath for HTML, Exception: @error_message', array('@error_message' => $e->getMessage()), \WATCHDOG_ERROR);
      }
      // Sometimes queryPath fails.  So one last check.
      if (!is_object($this->queryPath)) {
        throw new Exception("{$this->fileId} failed to initialize QueryPath");
      }
    }
  }


  /**
   * Clean and adjust the html in $this->queryPath.
   */
  protected function cleanQueryPathHtml() {
    try {
      \MigrationTools\QpHtml::convertRelativeSrcsToAbsolute($this->queryPath, $this->fileId);
      \MigrationTools\QpHtml::removeFaultyImgLongdesc($this->queryPath);
      // Empty anchors without name attribute will be stripped by ckEditor.
      \MigrationTools\QpHtml::fixNamedAnchors($this->queryPath);
    }
    catch (Exception $e) {
      \MigrationTools\Message::make('@file_id Failed to clean the html, Exception: @error_message', array('@file_id' => $this->fileId, '@error_message' => $e->getMessage()), \WATCHDOG_ERROR);
    }
  }
}
