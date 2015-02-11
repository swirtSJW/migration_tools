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
class ObtainDate extends ObtainHtml {

  /**
   * Finder method to find the .BottomLeftContent.
   *
   * @return string
   *   The string that was found
   */
  protected function findClassBottomLeftContent() {
    $element = $this->queryPath->top('.BottomLeftContent');
    $this->setElementToRemove($element);

    return $element->text();
  }


  /**
   * Finder method to find the .lastupdate.
   *
   * @return string
   *   The string that was found
   */
  protected function findClassLastupdate() {
    $element = $this->queryPath->top('.lastupdate');
    $this->setElementToRemove($element);
    $element->text();

    return $element->text();
  }

  /**
   * Finder method to find the #contentstart > p.
   *
   * @return string
   *   The string that was found
   */
  protected function findIdContentstartFirst() {
    $element = $this->queryPath->find('#contentstart > p');
    $this->setElementToRemove($element);

    return $element->text();
  }


  /**
   * Finder method to find the .newsLeft.
   *
   * @return string
   *   The string that was found
   */
  protected function findClassNewsLeft() {
    $element = $this->queryPath->top('.newsLeft');
    $this->setElementToRemove($element);

    return $element->text();
  }

  /**
   * Finder method to find the .newsRight.
   *
   * @return string
   *   The string that was found
   */
  protected function findClassNewsRight() {
    $element = $this->queryPath->top('.newsRight');
    $this->setElementToRemove($element);

    return $element->text();
  }

  /**
   * Finder method to find the .style2.
   *
   * @return string
   *   The string that was found
   */
  protected function findClassStyle2() {
    $element = $this->queryPath->top('.style2');
    $this->setElementToRemove($element);

    return $element->text();
  }

  /**
   * Method for returning the p that is aligned center.
   *
   * @return text
   *   The string found.
   */
  protected function findPAlignCenter() {
    foreach ($this->queryPath->find("p") as $p) {
      $align = $p->attr('align');
      if (strcmp($align, "right") == 0) {
        $text = $p->text();
        $this->setElementToRemove($p);
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
        $element = HtmlCleanUp::matchText($this->queryPath, $selector, $search_string);

        if (!empty($element)) {
          $text = $element->text();

          // Remove accompanying text and clean string.
          $text = str_replace($search_string, '', $text);
          $text = $this->cleanString($text);
          $valid = $this->validateString($text);

          if ($valid) {
            $this->setElementToRemove($element);
            $this->obtainerMessage("findProbableDate| selector: @selector  search string: @search_string", array('@selector' => $selector, '@search_string' => $search_string), WATCHDOG_DEBUG);

            return $text;
          }
        }
      }
    }

    return '';
  }

  /**
   * Method for returning the table cell at row 1, column 1.
   *
   * @return string
   *   The string found.
   */
  protected function findTableRow1Col1() {
    $table = $this->queryPath->find("table");
    $text = $this->getFromTable($table, 1, 1);

    return $text;
  }


  /**
   * Method for returning the table cell at row 1,  column 2.
   *
   * @return string
   *   The string found.
   */
  protected function findTableRow1Col2() {
    $table = $this->queryPath->find("table");
    $text = $this->getFromTable($table, 1, 2);

    return $text;
  }


  /**
   * Method for returning the 2nd table cell at row 2, column 2.
   *
   * @return string
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
   * Method for returning the table cell at 3rd row, 1st column.
   * @return text
   *   The string found.
   */
  protected function findTable3Row3Col2() {

    $table = $this->queryPath->find("table");
    $counter = 1;
    foreach ($table as $t) {
      if ($counter == 3) {
        $text = $this->getFromTable($t, 3, 2);
        break;
      }
      $counter++;
    }

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
        $this->setElementToRemove($elem);
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
        $this->setElementToRemove($p);
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
  public static function cleanText($text) {
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
   * Evaluates $possibleText and if it checks out, returns TRUE.
   *
   * @param string $string
   *   The string to validate.
   *
   * @return bool
   *   TRUE if possibleText can be used as a title.  FALSE if it cant.
   */
  protected function validateString($string) {
    // Run through any evaluations.  If it makes it to the end, it is good.
    // Case race, first to evaluate TRUE aborts the text.
    switch (TRUE) {
      // List any cases below that would cause it to fail validation.
      case empty($string):
      case is_object($string):
      case is_array($string);
        // If we can't form a date out of it, it must not be a date.
      case !strtotime($string);
        return FALSE;

      default:
        return TRUE;
    }
  }

}
