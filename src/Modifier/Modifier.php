<?php

namespace Drupal\migration_tools\Modifier;

use Drupal\migration_tools\Message;

/**
 * Information about which property we are dealing with.
 *
 * Including the class and methods to be called within that modifier.
 */
abstract class Modifier {
  public $modifiers = [];
  public $preModifiers = [];
  public $htmlPreModifiers = [];
  public $htmlPostModifiers = [];
  public $queryPath;

  // HTML contents for html modifiers.
  protected $html;

  /**
   * Constructor.
   *
   * @param object $query_path
   *   The query path object by reference.
   */
  public function __construct(&$query_path = NULL) {
    $this->queryPath = $query_path;
  }

  /**
   * Set Query Path.
   *
   * @param object $query_path
   *   Query Path.
   */
  public function setQueryPath(&$query_path) {
    $this->queryPath = $query_path;
  }

  /**
   * Add a new method to be called during modifier processing.
   *
   * @param string $method_name
   *   The name of the method to call.
   * @param array $arguments
   *   (optional) An array of arguments to be passed to the $method. Defaults
   *   to an empty array.
   *
   * @return Modifier
   *   Returns $this to allow chaining.
   */
  public function addModifier($method_name, array $arguments = []) {
    // @todo Maybe we should validate the method names here?
    $this->modifiers[] = [
      'method_name' => $method_name,
      'arguments' => $arguments,
    ];

    return $this;
  }

  /**
   * Add a new method to be called during pre modifier processing.
   *
   * @param string $method_name
   *   The name of the method to call.
   * @param array $arguments
   *   (optional) An array of arguments to be passed to the $method. Defaults
   *   to an empty array.
   *
   * @return Modifier
   *   Returns $this to allow chaining.
   */
  public function addPreModifier($method_name, array $arguments = []) {
    // @todo Maybe we should validate the method names here?
    $this->preModifiers[] = [
      'method_name' => $method_name,
      'arguments' => $arguments,
    ];

    return $this;
  }

  /**
   * Add a new method to be called on HTML prior to queryPath.
   *
   * Runs before any built-in HTML modifications are done.
   *
   * @param string $method_name
   *   The name of the method to call.
   * @param array $arguments
   *   (optional) An array of arguments to be passed to the $method. Defaults
   *   to an empty array.
   *
   * @return Modifier
   *   Returns $this to allow chaining.
   */
  public function addHtmlPreModifier($method_name, array $arguments = []) {
    // @todo Maybe we should validate the method names here?
    $this->htmlPreModifiers[] = [
      'method_name' => $method_name,
      'arguments' => $arguments,
    ];

    return $this;
  }

  /**
   * Add a new method to be called on HTML prior to queryPath.
   *
   * Runs after any built-in HTML modifications are done.
   *
   * @param string $method_name
   *   The name of the method to call.
   * @param array $arguments
   *   (optional) An array of arguments to be passed to the $method. Defaults
   *   to an empty array.
   *
   * @return Modifier
   *   Returns $this to allow chaining.
   */
  public function addHtmlPostModifier($method_name, array $arguments = []) {
    // @todo Maybe we should validate the method names here?
    $this->htmlPostModifiers[] = [
      'method_name' => $method_name,
      'arguments' => $arguments,
    ];

    return $this;
  }
  /**
   * Runs the modifiers and reports which were successful.
   *
   * @param bool $pre
   *   If TRUE, runs before jobs.
   */
  public function run($pre = FALSE) {
    $modifiers = $pre ? $this->preModifiers : $this->modifiers;
    $alter_log = [];
    $total_requested = count($modifiers);
    foreach ($modifiers as $key => $modifier) {
      if (!method_exists($this, $modifier['method_name'])) {
        Message::make('The modifier method @method does not exist and was skipped.', ['@method' => $modifier['method_name']], Message::DEBUG);
      }
      // The modifier exists, so run it.
      // Reset QueryPath pointer to top of document.
      $this->queryPath->top();

      $count = call_user_func_array([$this, $modifier['method_name']], $modifier['arguments']);
      // Record only what worked.
      if ($count) {
        $args = implode(', ', $modifier['arguments']);
        $alter_log[] = "x{$count} {$modifier['method_name']}({$args}) ";
      }
    }
    Message::makeSummary($alter_log, $total_requested, 'Modifiers applied successfully:');
  }

  /**
   * Runs the modifiers and reports which were successful.
   *
   * @param string $html
   *   HTML contents to run modifiers on.
   *
   * @param bool $pre
   *   If TRUE, runs before any other built-in HTML modifications.
   *
   * @return string
   *   Modified HTML.
   */
  public function runHtmlModifiers($html, $pre = TRUE) {
    $this->html = $html;
    $modifiers = $pre ? $this->htmlPreModifiers : $this->htmlPostModifiers;
    $alter_log = [];
    $total_requested = count($modifiers);
    foreach ($modifiers as $key => $modifier) {
      if (!method_exists($this, $modifier['method_name'])) {
        Message::make('The modifier method @method does not exist and was skipped.', ['@method' => $modifier['method_name']], Message::DEBUG);
      }

      $count = call_user_func_array([$this, $modifier['method_name']], $modifier['arguments']);
      // Record only what worked.
      if ($count) {
        $args = implode(', ', $modifier['arguments']);
        $alter_log[] = "x{$count} {$modifier['method_name']}({$args}) ";
      }
    }
    Message::makeSummary($alter_log, $total_requested, 'HTML Modifiers applied successfully:');
    return $this->html;
  }
}
