<?php

namespace Drupal\migration_tools\Modifier;

use Drupal\migration_tools\Message;

/**
 * SourceModifierHtml class.
 *
 * Contains methods for performing operations on raw HTML.
 */
class SourceModifierHtml extends SourceModifier {

  /**
   * Runs a StringTools method on the content.
   *
   * @param string $method_name
   *   StringTools method to run
   * @param array $arguments
   *   Optional array of arguments.
   */
  public function runStringTools($method_name, array $arguments = []) {
    if (method_exists('StringTools', $method_name)) {
      if (!method_exists($this, $method_name)) {
        Message::make('The StringTools method @method does not exist and was skipped.', ['@method' => $method_name], Message::DEBUG);
      }
      array_unshift($arguments, $this->content);
      $this->content = call_user_func_array([$this, $method_name], $arguments);
    }
  }

  /**
   * Replace string in HTML.
   *
   * @param string $search
   *   Search pattern
   * @param string $replace
   *   Replacement pattern
   * @param bool $case_insensitive
   *   If TRUE, uses case-insensitive replacement. Ignored if regex = TRUE
   * @param bool $regex
   *   If TRUE, uses regex with preg_replace.
   */
  public function replaceString($search, $replace, $case_insensitive = FALSE, $regex = FALSE) {
    if ($regex) {
      $this->content = preg_replace($search, $replace, $this->content);
    }
    elseif ($case_insensitive) {
      $this->content = str_ireplace($search, $replace, $this->content);
    }
    else {
      $this->content = str_replace($search, $replace, $this->content);
    }
  }
}
