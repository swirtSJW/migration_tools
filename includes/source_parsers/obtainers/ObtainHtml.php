<?php

/**
 * @file
 * Class ObtainHtml
 *
 * Contains a collection of stackable finders that can be arranged
 * as needed to obtain a body or other long html content.
 */

/**
 * Obtains HTML using and stack of finder methods.
 */
class ObtainHtml extends Obtainer {

  /**
   * {@inheritdoc}
   */
  public function __construct($query_path, $method_stack) {
    parent::__construct($query_path, $method_stack);
    $this->processMethodStack($query_path, $method_stack, 'ObtainHtml');
  }


  // **************** Begin finder target definitions *************************
  // To create a new finder, use this template and put them in alpha order.
  // @codingStandardsIgnoreStart
  /*
  protected function findMethod() {
    $this->setJustFound($this->queryPath->find("{SELECTOR}")->first());
    $text = $this->getJustFound()->text();
    return $text;
  }
  */
  // @codingStandardsIgnoreEnd


  // ***************** Helpers ***********************************************.

  /**
   * Cleans $text and returns it.
   *
   * @param string $text
   *   Text to clean and return.
   *
   * @return string
   *   The cleaned text.
   */
  public static function cleanPossibleText($text = '') {
    // There are also numeric html special chars, let's change those.
    module_load_include('inc', 'doj_migration', 'includes/doj_migration');
    $text = doj_migration_html_entity_decode_numeric($text);

    // Checking again in case another process rendered it non UTF-8.
    $is_utf8 = mb_check_encoding($text, 'UTF-8');

    if (!$is_utf8) {
      $text = StringCleanUp::fixEncoding($text);
    }

    $text = StringCleanUp::stripCmsLegacyMarkup($text);

    // Remove specific strings.
    // Strings to remove must be sorted by complexity.  More complex must come
    // before smaller or less complex things.
    $strings_to_remove = array(
      'updated:',
      'updated',
    );
    foreach ($strings_to_remove as $string_to_remove) {
      $text = str_ireplace($string_to_remove, '', $text);
    }

    // Remove white space-like things from the ends and decodes html entities.
    $text = StringCleanUp::superTrim($text);

    // Return the $text in case this is being used publicly.
    return $text;
  }

  /**
   * Evaluates $possibleText and if it checks out, returns TRUE.
   *
   * @return bool
   *   TRUE if possibleText can be used as a title.  FALSE if it cant.
   */
  protected function validatePossibleText() {
    $text = $this->getPossibleText();
    // Run through any evaluations.  If it makes it to the end, it is good.
    // Case race, first to evaluate TRUE aborts the text.
    switch (TRUE) {
      // List any cases below that would cause it to fail validation.
      case empty($text):
      case is_object($text):
      case is_array($text);
        return FALSE;

      default:
        return TRUE;

    }
  }
}
