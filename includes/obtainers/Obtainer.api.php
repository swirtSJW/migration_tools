<?php

/**
 * @file
 * Contains example Obtainer class.
 */


/**
 * Obtainer for title property for /atr pages.
 *
 * @package doj_migration
 * @subpackage atr
 */
class ObtainExample extends ObtainHtml {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($string) {
    parent::cleanString($string);
  }

  /**
   * {@inheritdoc}
   */
  protected function validateString($string) {
    parent::validateString($string);
  }

  /**
   * {@inheritdoc}
   */
  protected function processString($string) {
    parent::processString($string);
  }

  /**
   * Find example snippet.
   */
  protected function findExampleClass() {
    $element = $this->queryPath->find(".example");

    return $element->text();
  }
}

