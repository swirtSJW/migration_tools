<?php

/**
 * @file
 * Class DistrictPressReleaseSourceParser
 */

class DistrictPressReleaseSourceParser extends SourceParser {


  /**
   * Defines and returns an array of parsing methods to call in order.
   *
   * @return array
   *   An array of parssing methods to call in order.
   */
  public function getParseOrder() {
    // Specify the parsing methods that should run in order.
    return array(
      // Getting the title relies on html that could be wiped during clean up
      // so let's get it before we clean things up.
      'setTitle',
      'setSubTitle',
      'setDate',
      'setID',
      // The title is set, so let's clean our html up.
      'cleanHtml',
      // With clean html we can get the body out.
      'setBody',
    );
  }


  /**
   * Setter.
   */
  public function setBody($override = '') {
    // If the first paragraph in the content div says archive, lets remove it.
    $elem = HtmlCleanUp::matchText($this->queryPath, ".contentSub > div > p", "Archives");
    if ($elem) {
      $elem->remove();
    }

    $elem = HtmlCleanUp::matchText($this->queryPath, "table", "FOR IMMEDIATE RELEASE");
    if ($elem) {
      $elem->remove();
    }

    // Build selectors to remove.
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
      "a[href='#top']",
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);

    $default_target_stack = array();

    $body_stack = (!empty($this->getObtainerMethods('body'))) ? $this->getObtainerMethods('body') : $default_target_stack;
    $this->setObtainerMethods(array('body' => $body_stack));

    parent::setBody($override);
  }


  /**
   * Sets $this->title.
   *
   * This is duplicated so that we can use ObtainTitlePressRelease obtainer.
   */
  protected function setTitle($override = '') {

    // QueryPathing our way through things can fail.
    // In that case let's still set things and inform people about the issue.
    try {
      if (empty($override)) {
        // Default stack: Use this if none was defined in
        // $arguments['obtainer_methods'].
        $default_target_stack = array(
          'findAnyH1',
          'findIdContentstartDivH2Sec',
          'findH2First',
          'findClassContentSubDivPCenterStrong',
          'findClassContentSubDivDivPStrong',
          'findIdHeadline',
          'findPStrongEm',
          'findIdContentstartDivH2',
          'findDivClassContentSubDivDivCenter',
        );

        $title_find_stack = (!empty($this->getObtainerMethods('title'))) ? $this->getObtainerMethods('title') : $default_target_stack;
        $obtained_title = new ObtainTitlePressRelease($this->queryPath, $title_find_stack);
        $title = $obtained_title->getText();

        // Check for discarded text and for a setSubTitle method.
        // Some extensions may have them, but the base class does not.
        if (!empty($obtained_title->getTextDiscarded()) && method_exists($this, 'setSubTitle')) {
          // Put the discarded text into the subtitle.  It might not be right,
          // but at least it is not lost.
          $this->setSubTitle($obtained_title->getTextDiscarded());
        }

      }
      else {
        // The override was invoked, so use it.
        $title = $override;
        $title = ObtainTitle::cleanPossibleText($title);
      }

      $this->title = $title;
      // Output to show progress to aid debugging.
      drush_doj_migration_debug_output("{$this->fileId}  --->  {$this->title}");
    }
    catch (Exception $e) {
      $this->title = "";
      watchdog('doj_migration', '%file: failed to set the title', array('%file' => $this->fileId), WATCHDOG_ALERT);
      drush_doj_migration_debug_output("ERROR DistrictPressReleaseSourceParser: {$this->fileId} failed to set title.");
    }
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
   * Setter.
   */
  protected function setID($override = '') {
    $default_target_stack = array(
      'findTable3y1x',
    );

    $id_stack = (!empty($this->getObtainerMethods('id'))) ? $this->getObtainerMethods('id') : $default_target_stack;
    $this->setObtainerMethods(array('id' => $id_stack));

    parent::setID($override);
  }


  /**
   * Setter.
   */
  public function setSubTitle($override = '') {
    // If the subttile has already been set, leave it alone.
    if (empty($this->getSubTitle())) {
      if (empty($override)) {
        $default_target_stack = array();

        $subtitle = $this->runObtainer('ObtainTitle', 'subtitle', $default_target_stack);
      }
      else {
        // The override was invoked, so use it.
        $subtitle = $override;
        $subtitle = ObtainTitle::cleanPossibleText($subtitle);
      }
      $this->subTitle = $subtitle;
      // Output to show progress to aid debugging.
      drush_doj_migration_debug_output("--Subtitle->  {$this->getSubTitle()}");
    }
  }

}