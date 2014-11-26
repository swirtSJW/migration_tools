<?php

/**
 * @file
 * Class ObtainDate
 *
 * Contains a collection of stackable finders that can be arranged
 * as needed to obtain a date content.
 */

/**
 * {@inheritdoc}
 */
class ObtainDate extends Obtainer {

  // Properties declaration.


  /**
   * {@inheritdoc}
   */
  public function __construct($query_path, $target_stack) {
    parent::__construct($query_path, $target_stack);

    $this->processMethodStack($query_path, $target_stack, 'ObtainDate');
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


  /**
   * Finder method to find the .lastupdate .
   *
   * @return string
   *   The string that was found
   */
  protected function findClassLastupdate() {
    $this->setJustFound($this->queryPath->top('.lastupdate'));
    $string = $this->getJustFound()->text();

    return $string;
  }


  /**
   * Method for returning the table cell at 3rd row, 1st column.
   * @return text
   *   The string found.
   */
  protected function findTable3y1x() {
    $table = $this->queryPath->find("table");
    $text = SourceParser::getFromTable($table, 3, 1);
    return $text;
  }


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

    // We want out titles to be only digits and ascii chars so we can produce
    // clean aliases.
    $text = StringCleanUp::convertNonASCIItoASCII($text);

    // Checking again in case another process rendered it non UTF-8.
    $is_utf8 = mb_check_encoding($text, 'UTF-8');

    if (!$is_utf8) {
      $text = StringCleanUp::fixEncoding($text);
    }

    // Remove white space-like things from the ends and decodes html entities.
    $text = StringCleanUp::superTrim($text);

    return $text;
  }


  /**
   * Convert an obtained date into another format.
   *
   * @param string $format
   *   The format of the date to be returned.
   *   http://php.net/manual/en/function.date.php
   *
   * @return string
   *   The formatted date string.
   */
  public function formatDate($format = 'n/d/Y') {
    if ((!empty($format)) && (!empty($this->getText()))) {
      // We have a format and a date to use.
      $date_string = date($format, strtotime($this->getText()));
    }
    return (!empty($date_string)) ? $date_string : '';
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
        // If we can't form a date out of it, it must not be a date.
      case !strtotime($text);
        return FALSE;

      default:
        return TRUE;
    }
  }

}
