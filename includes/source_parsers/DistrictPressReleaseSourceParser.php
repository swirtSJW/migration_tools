<?php

/**
 * @file
 * Class DistrictPressReleaseSourceParser
 */

class DistrictPressReleaseSourceParser extends SourceParser {

  protected $date;
  protected $subtitle;
  protected $number;

  /**
   * Constructor.
   *
   * @param string $file_id
   *   The file id, e.g. careers/legal/pm7205.html
   * @param string $html
   *   The full HTML data as loaded from the file.
   * @param bool $fragment
   *   Set to TRUE if there are no <html>,<head>, or <body> tags in the HTML.
   * @param array $qp_options
   *   An associative array of options to be passed to the html_qp() function.
   */
  public function __construct($file_id, $html, $fragment = FALSE, $qp_options = array()) {
    $html = StringCleanUp::fixEncoding($html);
    $html = StringCleanUp::stripWindowsCRChars($html);
    $html = StringCleanUp::fixWindowSpecificChars($html);

    $this->initQueryPath($html, $qp_options);
    $this->fileId = $file_id;

    $this->setTitle();
    $this->setSubTitle();
    $this->setDate();
    $this->setNumber();
    $this->cleanHtml();
    $this->setBody();
  }


  /**
   * Setter.
   */
  public function setBody() {
    // If the first paragraph in the content div says archive, lets remove it.
    $elem = HtmlCleanUp::matchText($this->queryPath, ".contentSub > div > p", "Archives");
    if ($elem) {
      $elem->remove();
    }

    $elem = HtmlCleanUp::matchText($this->queryPath, "table", "FOR IMMEDIATE RELEASE");
    if ($elem) {
      $elem->remove();
    }

    $selectors = array(
      "#PRhead1",
      // Remove menus.
      "#navWrap",
      // If we have this layer elements remove them.
      "#Layer3",
      "#Layer4",
      // I don think prs really have useful images other than logos, lets
      // remove them for now, until I am proven wrong.
      "img",
      // Remove h1 tags, we should have already processed them.
      "h1",
      // Get rid of the breadcrumb.
      ".breadcrumb",
      // Get rid of newsLeft.
      ".newsLeft",
      // Get rid of widget.
      "#widget",
      // Remove footers.
      "#footer",
      "a[title='Printer Friendly']",
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);

    parent::setBody();
  }

  /**
   * Helper to check and sanitize strings that could become the title.
   *
   * @param string $text
   *   The text that could be the title.
   * @param null $elem
   *   The querypath element from which the text was acquired.
   *
   * @return string
   *   The text if it was approved. or empty string.
   */
  private function titleSetHelper($text, $elem = NULL) {
    $text = StringCleanUp::superTrim($text);
    if (!empty($text)) {
      if ($elem) {
        $elem->remove();
      }

      // If the title is too long, maybe they are putting both the title and
      // subtitle together. But when the text get to us, I don't see an easy
      // way to identify which is the title and which the subtitle. I will
      // cut the string at the title limit, and put the rest of the string
      // at the subtitle, so we are not throwing away data.
      if (strlen($text) > 255) {
        $pieces = $this->smartSplit($text, 255);
        $text = $pieces[0];
        $this->subtitle = $this->titleSetHelper($pieces[1]);
      }

      $uppercase_version = strtoupper($text);
      if (strcmp($uppercase_version, $text) == 0) {
        $text = ucwords(strtolower($text));
      }

      return $text;
    }
    return "";
  }

  /**
   * A common check to see if we need to continue to find a title or not.
   */
  private function titleCheck($text) {
    return empty($text) || strcmp($text, "News And Press Releases") == 0;
  }

  /**
   * The title and subtitle should have, more or less, the same logic.
   *
   * In this method we will combine all the title/subtitle related logic, and
   * what will make the different in what is what, will be the order in which
   * the functions are called. The title gets first dibs at finding a match.
   */
  private function titlesHelper() {
    $title = "";
    $winner = '';

    // Check the h1
    foreach ($this->queryPath->find("h1") as $h1) {
      // We don't want h1s to override each other.
      if ($this->titleCheck($title)) {
        $text = $h1->text();
        $title = $this->titleSetHelper($text, $h1);
        $winner = 'h1';
      }
      else {
        break;
      }
    }

    // Check the second h2.
    if ($this->titleCheck($title)) {
      $counter = 1;
      foreach ($this->queryPath->find("#contentstart > div > h2") as $h2) {
        if ($counter == 2) {
          $title = $this->titleSetHelper($h2->text(), $h2);
          $winner = 'h2-2nd';
          break;
        }
        $counter++;
      }
    }

    // Maybe the main title is just an h2.
    if ($this->titleCheck($title)) {
      $title = $this->titleSetHelper(HtmlCleanUp::extractFirstElement($this->queryPath, "h2"));
      $winner = 'h2';
    }

    // Straight up matches.
    $selectors = array(
      ".contentSub > div > p[align='center'] > strong",
      // @todo this rule is causing issues with the louisianan pr, can we
      // target it better?
      // ".contentSub > div > div > p > strong",
      "#headline",
      // "p > em",
      "p > strong > em",
      "#contentstart > div > h2",
      // For usao-az.
      '.Part > p',
      // Hail Mary.
      ".MsoNormal",
    );

    while ($this->titleCheck($title) && !empty($selectors)) {
      $selector = array_shift($selectors);
      if ($text = HtmlCleanUp::extractFirstElement($this->queryPath, $selector)) {
        $title = $this->titleSetHelper($text);
        $winner = $selector;
      }
    }

    if (empty($title)) {
      $elems = $this->queryPath->find("#Layer4")->siblings();
      $pcounter = 0;
      // The second p is the title.
      foreach ($elems as $elem) {
        if ($elem->is("p")) {
          $pcounter++;
          if ($pcounter == 6) {
            $title = $elem->text();
            $title = StringCleanUp::superTrim($title);
            $elem->remove();
            $winner = "p-#$pcounter";
          }
        }
      }
    }

    // Maybe the main title is just an h3.
    if ($this->titleCheck($title)) {
      $title = $this->titleSetHelper(HtmlCleanUp::extractFirstElement($this->queryPath, "h3"));
      $winner = 'h3';
    }

    // Output to show progress to aid debugging.
    drush_doj_migration_debug_output("{$this->fileId}  --match[{$winner}]-->  {$this->title}");
    return $title;
  }

  /**
   * Setter.
   */
  protected function setTitle() {
    $this->title = $this->titlesHelper();
  }

  /**
   * Setter.
   */
  private function setSubTitle() {
    if (empty($this->subtitle)) {
      $this->subtitle = $this->titlesHelper();
    }
  }

  /**
   * Getter.
   */
  public function getSubtitle() {
    return $this->subtitle;
  }

  /**
   * Helper to choose and sanitize possible date strings.
   *
   * @param string $text
   *   The string that could be the date.
   * @param null $elem
   *   The querypath element from which the date came.
   *
   * @return string
   *   The date string or NULL.
   */
  private function dateSetHelper($text, $elem = NULL) {
    $text = StringCleanUp::superTrim($text);
    if (strtotime($text)) {
      if ($elem) {
        $elem->remove();
      }
      return $text;
    }
  }

  /**
   * Setter.
   */
  protected function setDate() {

    // Matches on tables.
    // Second td in the first tr of the table is the date.
    $table = $this->queryPath->find("table");

    $date = $this->dateSetHelper($this->getFromTable($table, 1, 2));

    if (empty($date)) {
      if (substr_count($text = $this->getFromTable($table, 1, 1), "FOR IMMEDIATE RELEASE") > 0) {
        $pieces = explode("\n", $text);
        $date = $this->dateSetHelper($pieces[1]);
      }
    }

    if (empty($date)) {
      // We need the second table.
      $counter = 1;
      foreach ($table as $t) {
        if ($counter == 2) {

          $text = $this->getFromTable($t, 2, 2);

          $date = $this->dateSetHelper($text);
        }
        $counter++;
      }
    }

    if (empty($date)) {
      foreach ($this->queryPath->find("p") as $p) {
        $align = $p->attr('align');
        if (strcmp($align, "right") == 0) {

          $date = $this->dateSetHelper($p->text(), $p);

          break;
        }
      }
    }

    // Straight up matches.
    $selectors = array(
      "#contentstart > p",
      ".newsRight",
    );
    while (empty($date) && !empty($selectors)) {
      $selector = array_shift($selectors);
      $text = HtmlCleanUp::extractFirstElement($this->queryPath, $selector);
      $date = $this->dateSetHelper($text);
    }

    // Matches with text conditionals.
    $selectors = array(
      ".BottomLeftContent" => "FOR IMMEDIATE RELEASE",
      "#dateline" => "NEWS RELEASE SUMMARY –",
      "p" => "FOR IMMEDIATE RELEASE",
    );

    while (empty($date) && !empty($selectors)) {
      $selector = array_shift(array_keys($selectors));
      $cleanup = array_shift($selectors);
      if ($elem = HtmlCleanUp::matchText($this->queryPath, $selector, $cleanup)) {
        $text = StringCleanUp::superTrim(str_replace($cleanup, "", $text = $elem->text()));
        $date = $this->dateSetHelper($text, $elem);
      }
    }

    // Other matches.
    if (empty($date)) {
      $pcounter = 0;
      // The second p is the title.
      foreach ($this->queryPath->find("#Layer4")->siblings() as $elem) {
        if ($elem->is("p")) {
          $pcounter++;
          if ($pcounter == 5) {
            $text = str_replace("FOR IMMEDIATE RELEASE", "", $elem->text());
            $date = $this->dateSetHelper($text, $elem);
            break;
          }
        }
      }
    }

    if (empty($date)) {
      $selector = ".newsLeft";
      $cleanup = "FOR IMMEDIATE RELEASE";
      if ($elem = HtmlCleanUp::matchText($this->queryPath, $selector, $cleanup)) {
        $elem->find("a")->remove();
        $text = StringCleanUp::superTrim(str_replace($cleanup, "", $text = $elem->text()));
        $date = $this->dateSetHelper($text, $elem);
      }
    }
    $this->date = $date;
  }

  /**
   * Getter.
   */
  public function getDate() {
    return $this->date;
  }

  /**
   * Setter.
   */
  protected function setNumber() {
    $table = $this->queryPath->find("table");
    $text = $this->getFromTable($table, 3, 1);
    $this->number = $text;
  }

  /**
   * Getter.
   */
  public function getNumber() {
    return $this->number;
  }

  /**
   * Split a string respecting a max lenght, but not destroying words.
   *
   * @param string $string
   *   The string.
   *
   * @param int $max_length
   *   The maximum length of the split.
   */
  private function smartSplit($string, $max_length) {
    $array = explode(' ', $string);

    $current_length = 0;
    $index = 0;

    foreach ($array as $word) {
      // +1 because the word will receive back the space in the end that it
      // loses in explode()
      $word_length = strlen($word) + 1;

      if (($current_length + $word_length) <= $max_length) {
        $current_length += $word_length;
        $index++;
      }
      else {
        break;
      }
      $output[0] = array_slice($array, 0, $index);
      $output[1] = array_slice($array, $index);

      for ($i = 0; $i < 2; $i++) {
        $output[$i] = implode(" ", $output[$i]);
      }

    }

    return $output;
  }
}
