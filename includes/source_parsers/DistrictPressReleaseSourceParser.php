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
      "a[href='http://www.justice.gov/usao/wvn']",
      "a[href='https://www.justice.gov/usao/wvn']",
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);

    $default_target_stack = array();

    $om = $this->getObtainerMethods('body');
    $body_stack = (!empty($om)) ? $om : $default_target_stack;
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
          'findH1Any',
          'findIdContentstartDivH2Sec',
          'findH2First',
          'findClassContentSubDivPCenterStrong',
          'findClassContentSubDivDivPStrong',
          'findIdHeadline',
          'findPStrongEm',
          'findIdContentstartDivH2',
          'findDivClassContentSubDivDivCenter',
        );

        $om = $this->getObtainerMethods('title');
        $title_find_stack = (!empty($om)) ? $om : $default_target_stack;
        $obtained_title = new ObtainTitlePressRelease($this->queryPath, $title_find_stack);
        $title = $obtained_title->getText();

        // Check for discarded text and for a setSubTitle method.
        // Some extensions may have them, but the base class does not.
        $td = $obtained_title->getTextDiscarded();
        if (!empty($td) && method_exists($this, 'setSubTitle')) {
          // Put the discarded text into the subtitle.  It might not be right,
          // but at least it is not lost.
          $this->setSubTitle($td);
        }

      }
      else {
        // The override was invoked, so use it.
        $title = $override;
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
  protected function setDate($override = '') {
    // Default stack: Use this if none was defined in
    // $arguments['obtainer_methods'].
    $default_target_stack = array(
      'findTableRow1Col2',
      'findTableRow1Col1',
      'findTable2Row2Col2',
      'findPAlignCenter',
      'findIdContentstartFirst',
      'findClassNewsRight',
      'findClassBottomLeftContent',
      'findProbableDate',
    );

    $om = $this->getObtainerMethods('date');
    $date_find_stack = (!empty($om)) ? $om : $default_target_stack;
    $this->setObtainerMethods(array('date' => $date_find_stack));

    return parent::setDate($override);
  }


  /**
   * Setter.
   */
  protected function setID($override = '') {
    $default_target_stack = array(
      'findTable3y1x',
    );

    $om = $this->getObtainerMethods('id');
    $id_stack = (!empty($om)) ? $om : $default_target_stack;
    $this->setObtainerMethods(array('id' => $id_stack));

    parent::setID($override);
  }


  /**
   * Setter.
   */
  public function setSubTitle($override = '') {
    // If the subtitle has already been set, leave it alone.
    $st = $this->getSubTitle();
    if (empty($st)) {
      if (empty($override)) {
        $default_target_stack = array();

        $subtitle = $this->runObtainer('ObtainSubTitle', 'subtitle', $default_target_stack);
      }
      else {
        // The override was invoked, so use it.
        $subtitle = $override;
      }
      $this->subTitle = $subtitle;
      // Output to show progress to aid debugging.
      drush_doj_migration_debug_output("--Subtitle->  {$this->getSubTitle()}");
    }
  }

}
