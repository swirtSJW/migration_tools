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

  /**
   * {@inheritdoc}
   */
  public function __construct($query_path, $method_stack) {
    parent::__construct($query_path, $method_stack);
    $this->processMethodStack($query_path, $method_stack, 'ObtainDate');
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
   * Finder method to find the .BottomLeftContent.
   *
   * @return string
   *   The string that was found
   */
  protected function findClassBottomLeftContent() {
    $this->setJustFound($this->queryPath->top('.BottomLeftContent'));
    $string = $this->getJustFound()->text();

    return $string;
  }


  /**
   * Finder method to find the .lastupdate.
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
   * Finder method to find the #contentstart > p.
   *
   * @return string
   *   The string that was found
   */
  protected function findIdContentstartFirst() {
    $this->setJustFound($this->queryPath->find('#contentstart > p'));
    $string = $this->getJustFound()->text();

    return $string;
  }


  /**
   * Finder method to find the .newsRight.
   *
   * @return string
   *   The string that was found
   */
  protected function findClassNewsRight() {
    $this->setJustFound($this->queryPath->top('.newsRight'));
    $string = $this->getJustFound()->text();

    return $string;
  }


  /**
   * Method for returning the p that is aligned center.
   * @return text
   *   The string found.
   */
  protected function findPAlignCenter() {
    foreach ($this->queryPath->find("p") as $p) {
      $align = $p->attr('align');
      if (strcmp($align, "right") == 0) {
        $text = $p->text();
        $this->setJustFound($p);
        break;
      }
    }

    return $text;
  }


  /**
   * Finder method to find dates by its accompanying text.
   *
   * @return string
   *   The string that was found
   */
  protected function findProbableDate() {
    // Selectors to run through.
    $selectors = array(
      '.BottomLeftContent',
      '#dateline',
      'p',
      '.newsLeft',
    );
    // Text strings to search for.
    $search_strings = array(
      "FOR IMMEDIATE RELEASE",
      "NEWS RELEASE SUMMARY",
      "FOR IMMEDIATE  RELEASE",
      "IMMEDIATE RELEASE",
    );
    // Loop through the selectors.
    foreach ($selectors as $selector) {
      // Loop through the search strings.
      foreach ($search_strings as $search_string) {
        // Search for the string.
        $elem = HtmlCleanUp::matchText($this->queryPath, $selector, $search_string);

        if (!empty($elem)) {
          $text = $elem->text();
          // Clean string.
          $processed_text = $this->cleanPossibleText($text);
          // Evaluate string.
          $this->setPossibleText($processed_text);
          $valid = $this->validatePossibleText();
          if ($valid) {
            // We have a winner.
            $this->setJustFound($elem);
            $this->setCurrentFindMethod("findProbableDate| selector:$selector  search string:$search_string");

            return $text;
          }
        }
      }
    }

    return '';
  }


  /**
   * Method for returning the table cell at row 1,  column 1.
   * @return text
   *   The string found.
   */
  protected function findTableRow1Col1() {
    $table = $this->queryPath->find("table");
    $text = $this->getFromTable($table, 1, 1);
    return $text;
  }


  /**
   * Method for returning the table cell at row 1,  column 2.
   * @return text
   *   The string found.
   */
  protected function findTableRow1Col2() {
    $table = $this->queryPath->find("table");
    $text = $this->getFromTable($table, 1, 2);
    return $text;
  }


  /**
   * Method for returning the 2nd table cell at row 2, column 2.
   * @return text
   *   The string found.
   */
  protected function findTable2Row2Col2() {
    $table = $this->queryPath->find("table");
    $counter = 1;
    foreach ($table as $t) {
      if ($counter == 2) {
        $text = $this->getFromTable($t, 2, 2);
        break;
      }
      $counter++;
    }

    return $text;
  }

  /**
   * Method for returning the table cell at 3rd row, 1st column.
   * @return text
   *   The string found.
   */
  protected function findTableRow3Col1() {
    $table = $this->queryPath->find("table");
    $text = $this->getFromTable($table, 3, 1);
    return $text;
  }

  /**
   * Get a very specific span.
   *
   * Check that it could be a date, and return it.
   *
   * @return string
   *   Possible date.
   */
  protected function findSpanFontSize8() {
    foreach ($this->queryPath->find("span[style = 'font-size:8.0pt']") as $elem) {
      $text = $elem->text();
      // Validate string.
      if (substr_count($text, "IMMEDIATE RELEASE") > 0) {
        $this->setJustFound($elem);
        $this->setCurrentFindMethod("findSpanFontSize8");
        return $text;
      }
    }
    return "";
  }

  /**
   * A paragraph with the style1 class and a br in the inner html.
   */
  protected function findStyle1PwithBr() {
    $elems = $this->queryPath->find("p.style1");
    foreach ($elems as $p) {
      $html = $p->html();
      if (substr_count($html, "<br/>") > 0) {
        $this->setJustFound($p);
        $pieces = explode("<br/>", $html);
        $text = strip_tags($pieces[1]);
        return $text;
      }
    }
    return "";
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

    // Remove some strings that often accompany dates.
    $remove = array(
      'FOR',
      'IMMEDIATE',
      'RELEASE',
      'NEWS RELEASE SUMMARY –',
      'news',
      'press',
      'release',
      'updated',
      'update',
    );
    $text = str_ireplace($remove, '', $text);

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
    $text = $this->getText();
    if ((!empty($format)) && (!empty($text))) {
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
