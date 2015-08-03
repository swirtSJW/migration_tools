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
   * Finder method to find the first .newsRight.
   *
   * @return string
   *   The string that was found
   */
  protected function pluckClassNewsRightLast() {
    $element = $this->queryPath->top('.newsRight')->last();
    $this->setElementToRemove($element);

    return $element->text();
  }


  /**
   * Method for returning the div > p that is aligned left.
   *
   * @return text
   *   The string found.
   */
  protected function pluckDivPAlignLeft() {
    foreach ($this->queryPath->find("div > p") as $p) {
      $align = $p->attr('align');
      if (strcmp($align, "left") == 0) {
        $text = $p->text();
        $this->setElementToRemove($p);
        break;
      }
    }

    return $text;
  }


  /**
   * Method for returning the div.contentSub > div.
   *
   * @return text
   *   The string found.
   */
  protected function findDivClassContentSubDiv3() {
    // Due to the nature of the text extraction, it can not be removed.
    $text = $this->queryPath->find("div.contentSub > div")->next()->next()->text();
    $text = trim(trim($text));
    $pos = strpos($text, "\n");
    $text = substr($text, 0, $pos);

    return $text;
  }

  /**
   * Finder method to find dates by its accompanying text.
   *
   * @return string
   *   The string that was found
   */
  protected function pluckProbableDate() {
    // Selectors to run through.
    $selectors = array(
      '.BottomLeftContent',
      '#dateline',
      'p',
      '.newsLeft',
      '.newsRight',
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
            new MigrationMessage("pluckProbableDate| selector: @selector  search string: @search_string", array('@selector' => $selector, '@search_string' => $search_string), WATCHDOG_DEBUG);

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
  protected function pluckTableRow1Col1() {
    $table = $this->queryPath->find("table");
    $text = $this->pluckFromTable($table, 1, 1);

    return $text;
  }

  /**
   * Method for returning the table cell at row 1,  column 2.
   *
   * @return string
   *   The string found.
   */
  protected function pluckTableRow1Col2() {
    $table = $this->queryPath->find("table");
    $text = $this->pluckFromTable($table, 1, 2);

    return $text;
  }

  /**
   * Method for returning the 1st table, cell at row 2, column 1.
   *
   * @return string
   *   The string found.
   */
  protected function pluckTable1Row2Col1() {
    $table = $this->queryPath->find("table");
    foreach ($table as $key => $t) {
      if ($key == 0) {
        $text = $this->pluckFromTable($t, 2, 1);
        break;
      }
    }

    return $text;
  }

  /**
   * Method for returning the 2nd table cell at row 2, column 2.
   *
   * @return string
   *   The string found.
   */
  protected function pluckTable2Row2Col2() {
    $table = $this->queryPath->find("table");
    $counter = 1;
    foreach ($table as $t) {
      if ($counter == 2) {
        $text = $this->pluckFromTable($t, 2, 2);
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
  protected function pluckTableRow3Col1() {
    $table = $this->queryPath->find("table");
    $text = $this->pluckFromTable($table, 3, 1);
    return $text;
  }

  /**
   * Method for returning the table cell at 3rd row, 1st column.
   * @return text
   *   The string found.
   */
  protected function pluckTable3Row3Col2() {

    $table = $this->queryPath->find("table");
    $counter = 1;
    foreach ($table as $t) {
      if ($counter == 3) {
        $text = $this->pluckFromTable($t, 3, 2);
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
  protected function pluckSpanFontSize8() {
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
  protected function pluckStyle1PwithBr() {
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
  public static function cleanString($text) {

    // There are also numeric html special chars, let's change those.
    module_load_include('inc', 'migration_tools', 'includes/migration_tools');
    $text = strongcleanup::decodehtmlentitynumeric($text);

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
      'CONTACT:',
      'Press Release',
      'news',
      'press',
      'release',
      'updated',
      'update',
      'sunday',
      'monday',
      'tuesday',
      'wednesday',
      'thursday',
      'friday',
      'saturday',
      // Intentional mispellings.
      'thurday',
      'wendsday',
      'firday',
    );
    // Replace these with nothing.
    $text = str_ireplace($remove, '', $text);

    $remove = array(
      '.',
      ',',
      "\n",
    );
    // Replace these with spaces.
    $text = str_ireplace($remove, ' ', $text);

    // Fix mispellings and abbreviations.
    $replace = array(
      'septmber' => 'september',
      'arpil' => 'april',
      'febraury' => 'february',
      '2103' => '2013',
      '2104' => '2014',
      '2105' => '2015',
    );
    $text = str_ireplace(array_keys($replace), array_values($replace), $text);

    // Remove multiple spaces.
    $text = preg_replace('/\s{2,}/u', ' ', $text);
    // Remove any text following the 4 digit year.
    $years = range(1995, 2015);
    foreach ($years as $year) {
      $pos = strpos($text, (string) $year);
      if ($pos !== FALSE) {
        $text = substr($text, 0, ($pos + 4));
        break;
      }
    }

    // Remove white space-like things from the ends and decodes html entities.
    $text = StringCleanUp::superTrim($text);

    return $text;
  }

  /**
   * Returns array of month names.
   *
   * @return array
   *   One entry for each month name.
   */
  public static function returnMonthNames() {
    return array(
      'January',
      'February',
      'March',
      'April',
      'May',
      'June',
      'July',
      'August',
      'September',
      'October',
      'November',
      'December',
    );
  }

  /**
   * Evaluates $string for presence of more than 1 element from month array.
   *
   * @param string $string
   *   The string to validate.
   *
   * @return bool
   *   TRUE if more than one Month found in string.  FALSE if not.
   */
  public static function searchStringDoubleMonth($string) {
    $months = self::returnMonthNames();
    $count = 0;
    $bmultiple = FALSE;
    foreach ($months as $month) {
      $lower_month = strtolower($month);
      $lower_string = strtolower($string);
      if (strstr($lower_string, $lower_month)) {
        $count++;
        if ($count > 1) {
          $bmultiple = TRUE;
          break;
        }
      }
    }
    return $bmultiple;
  }

  /**
   * Searches for a date range overlapping months, returns single date.
   *
   * Looks specifically for month names as defined in self::returnMonthNames().
   * In the case of multiple months found in $string, returns the latter date.
   * Example: "March 28 - April 4", returns April 4.
   *
   * @param string $string
   *   The string to clean.
   *
   * @return string
   *   Return string includes only 1 month.
   */
  public static function removeMultipleMonthRange($string) {
    $bmultiple = self::searchStringDoubleMonth($string);
    if ($bmultiple === TRUE) {
      $explode = explode('-', $string);
      if (!empty($explode[1])) {
        $latter_date = $explode[1];
        return $latter_date;
      }
    }

    return $string;
  }

  /**
   * Evaluates $string and if it checks out, returns TRUE.
   *
   * @param string $string
   *   The string to validate.
   *
   * @return bool
   *   TRUE if possibleText can be used as a date.  FALSE if it cant.
   */
  protected function validateString($string) {
    // Run through any evaluations.  If it makes it to the end, it is good.
    // Case race, first to evaluate TRUE aborts the text.
    switch (TRUE) {
      // List any cases below that would cause it to fail validation.
      case empty($string):
      case is_object($string):
      case is_array($string):
      case (strlen($string) < 7):
        // If we can't form a date out of it, it must not be a date.
      case !strtotime($string):
        return FALSE;

      default:
        return TRUE;
    }
  }

}
