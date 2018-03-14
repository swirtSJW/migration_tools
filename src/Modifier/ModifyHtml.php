<?php

namespace Drupal\migration_tools\Modifier;

use Drupal\migration_tools\StringTools;

/**
 * The ModifyHtml defining removers, and changers.
 *
 * Removers must:
 *   Be protected functions that remove elements from the QueryPath object.
 *   Return a count of the items removed.
 *   Follow the naming patten removeThingToBeRemoved().
 *
 * Changers must:
 *   Be protected functions that alter elements from the QueryPath object.
 *   Return a count of the items changed.
 *   Follow the naming patten changeDescriptionOfChange().
 */
class ModifyHtml extends Modifier {

  /**
   * Change any HTML class from one to a new classname.
   *
   * @param string $original_classname
   *   The classname to change from.
   * @param string $new_classname
   *   The new classname.  Removes the class if empty.
   *
   * @return int
   *   Count of items removed.
   */
  protected function changeClassName($original_classname, $new_classname = '') {
    $count = 0;
    if (!empty($original_classname)) {
      $elements = $this->queryPath->find(".{$original_classname}");
      foreach ((is_object($elements)) ? $elements : [] as $element) {
        if (empty($new_classname)) {
          $element->removeAttr('class');
        }
        else {
          $element->attr('class', $new_classname);
        }

        $count++;
      }
    }

    return $count;
  }

  /**
   * Remove a class from a class from all elements.
   *
   * @param string $classname
   *   The classname to remove.
   *
   * @return int
   *   Count of items removed.
   */
  protected function changeRemoveClassName($classname) {
    return $this->changeClassName($classname);
  }

  /**
   * Remove all tables that are empty or contain only whitespace.
   *
   * @return int
   *   Count of items removed.
   */
  protected function removeEmptyTables() {
    $count = 0;
    $tables = $this->queryPath->find('table');
    foreach ((is_object($tables)) ? $tables : [] as $table) {
      $table_contents = $table->text();
      // Remove whitespace in order to evaluate if it is empty.
      $table_contents = StringTools::superTrim($table_contents);

      if (empty($table_contents)) {
        $table->remove();
        $count++;
      }
    }

    return $count;
  }

  /**
   * Remover for all matching selector on the page.
   *
   * @param string $selector
   *   The selector to find.
   *
   * @return int
   *   Count of items removed.
   */
  protected function removeSelectorAll($selector) {
    $count = 0;
    if (!empty($selector)) {
      $elements = $this->queryPath->find($selector);
      foreach ((is_object($elements)) ? $elements : [] as $element) {
        $element->remove();
        $count++;
      }
    }

    return $count;
  }

  /**
   * Remover for Nth  selector on the page.
   *
   * @param string $selector
   *   The selector to find.
   * @param int $n
   *   (optional) The depth to find.  Default: first item n=1.
   *
   * @return int
   *   Count of items removed.
   */
  protected function removeSelectorN($selector, $n = 1) {
    $n = ($n > 0) ? $n - 1 : 0;
    if (!empty($selector)) {
      $elements = $this->queryPath->find($selector);
      foreach ((is_object($elements)) ? $elements : [] as $i => $element) {
        if ($i == $n) {
          $element->remove();

          return 1;
        }
      }
    }

    return 0;
  }

  /**
   * Remove style attribute from selector.
   *
   * @param object $selector
   *   The selector to find.
   *
   * @return int
   *   Count of style attributes removed.
   */
  protected function removeStyleAttr($selector) {
    $count = 0;
    if (!empty($selector)) {
      $elements = $this->queryPath->find($selector);
      foreach ((is_object($elements)) ? $elements : [] as $element) {
        $element->removeAttr('style');
        $count++;
      }
    }

    return $count;
  }

}
