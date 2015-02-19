<?php
/**
 * @file
 * ObtainContentType.
 */

class ObtainContentType extends Obtainer {
  /**
   * Find the type of an html file.
   */
  protected function findType() {
    $body = $this->queryPath->find('body')->first();
    $text = $body->text();
    if (substr_count($text, "IMMEDIATE RELEASE") > 0) {
      return "press_release";
    }
    else {
      return "page";
    }
  }
}
