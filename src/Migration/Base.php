<?php

/**
 * @file
 * Contains base migration class for all migrations.
 */

namespace MigrationTools\Migration;

/**
 * Abstract intermediate class holding the most basic common helper methods.
 *
 * @package migration_tools
 */
abstract class Base extends \Migration {
  /**
   * The base path used to coral all the redirects from the legacy server.
   *
   * This is the path that the source server will redirect requests to.
   * Example:
   * Request for www.legacy.com/abc/this-page.html?p=2
   * Gets redirected by legacy to
   * www.new.com/{$redirectBase}/abc/this-page.html?p=2
   * Drupal then redirects this request to
   * www.new.com/actual-path/abc/page-title-pattern
   * @var string
   */
  public $redirectCoral;

  /**
   * @var string $sourceParserClass
   *   The class name of the source parser. Used to instantiate $sourceParser.
   */
  protected $sourceParserClass;

  /**
   * @var SourceParser $sourceParser
   *   The source parser object for a given row.
   */
  protected $sourceParser;

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
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }

  /**
   * Add multiple field mappings at once.
   *
   * @param array $mappings
   *   An array of field mappings in the form of source_key => dest_key.
   *
   * @param bool $warn_on_override
   *   Set to FALSE to prevent warnings when there's an existing mapping.
   */
  public function addFieldMappings(array $mappings, $warn_on_override = TRUE) {
    foreach ($mappings as $source => $destination) {
      $this->addFieldMapping($source, $destination, $warn_on_override);
    }
  }

  /**
   * Arguments in keyed array passed among migration classes and parsers.
   *
   * @var array $arguments
   */
  protected $arguments = array();

  /**
   * Basic getter for $arguments.
   *
   * @return array
   *   Whatever has been stored in $this->arguments.
   */
  public function getArguments() {
    return $this->arguments;
  }

  /**
   * Merges an array into the existing arguments array.
   *
   * @param array $new_args
   *   Keyed array matching the format of the arguments array, to be merged.
   *
   * @return array
   *   Array of the complete arguments array with the new elements.
   */
  protected function mergeArguments($new_args = '') {
    if (!empty($new_args) && is_array($new_args)) {
      $this->arguments = array_merge($this->arguments, $new_args);
    }
  }

  /**
   * Gets a single argument from the arguments array.
   *
   * @param string $arg_key
   *   The key of the item to return from the Arguments array.
   *
   * @return mixed
   *   Whatever is stored in the $keys's value, or NULL if not in the arguments.
   */
  protected function getArgument($arg_key = '') {
    if (!empty($arg_key)) {
      $args = $this->getArguments();
      if (array_key_exists($arg_key, $args)) {
        return $args[$arg_key];
      }
    }
    return NULL;
  }


  /**
   * Unsets NULL properties on a single dimensional object.
   *
   * @param obj $row
   *   The object to iterate over.
   */
  public function removeEmptyProperties(&$row) {
    foreach ($row as $key => $property) {
      if (is_null($property)) {
        unset($row->$key);
      }
      elseif (is_string($property) && !$property) {
        unset($row->$key);
      }
    }
  }
}
