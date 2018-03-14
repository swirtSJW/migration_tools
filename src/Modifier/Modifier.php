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
  public $queryPath;

  /**
   * Constructor.
   *
   * @param object $query_path
   *   The query path object by reference.
   */
  public function __construct(&$query_path) {
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

}
