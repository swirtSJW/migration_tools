<?php

/**
 * @file
 * Class DistrictPressReleaseSourceParser
 */

class PressReleaseSourceParser extends SourceParser {


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
      'setDate',
      // The title is set, so let's clean our html up.
      'cleanHtml',
      // With clean html we can get the body out.
      'setBody',
    );
  }

  /**
   * Setter.
   */
  public function setBody($body = '') {
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
      "#navWrap",
      "#Layer3",
      "#Layer4",
      "img",
      "h1",
      ".breadcrumb",
      ".newsLeft",
      "#widget",
      "#footer",
      "a[title='Printer Friendly']",
      "a[href='#top']",
      "a[href='http://www.justice.gov/usao/wvn']",
      "a[href='https://www.justice.gov/usao/wvn']",
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);

    parent::setBody($body);
  }


  /**
   * Sets $this->title.
   *
   * This is duplicated so that we can use ObtainTitlePressRelease obtainer.
   */
  protected function setTitle($title = '') {
    try {
      if (empty($title)) {
        $method_stack = $this->getObtainerMethods('title');
        if (empty($method_stack)) {
          $method_stack = array();
          $method_stack[] = array('pluckAnySelectorUntilValid', array('h1'));
          $method_stack[] = array(
            'pluckSelector',
            array("#contentstart > div > h2", 2),
          );
          $method_stack[] = array('pluckSelector', array("h2", 1));
          $method_stack[] = array(
            'pluckSelector',
            array(".contentSub > div > p[align='center'] > strong", 1),
          );
          $method_stack[] = array(
            'pluckSelector',
            array(".contentSub > div > div > p > strong", 1),
          );
          $method_stack[] = array('pluckSelector', array("#headline", 1));
          $method_stack[] = array('pluckSelector', array("p > strong > em", 1));
          $method_stack[] = array(
            'pluckSelector',
            array("#contentstart > div > h2", 1),
          );
        }
        $this->setObtainerMethods(array('title' => $method_stack));
        $title = $this->runObtainer('ObtainTitlePressRelease', 'title');
      }

      $this->title = $title;
      // Output to show progress to aid debugging.
      new MigrationMessage('Title found --> @title', array('@title' => $this->title), WATCHDOG_DEBUG, 1);
    }
    catch (Exception $e) {
      $this->title = '';
      new MigrationMessage("Error setting title.", array(), WATCHDOG_ERROR, 1);
    }
  }

  /**
   * Setter.
   */
  protected function setDate($date = '') {
    if (empty($date)) {
      $method_stack = $this->getObtainerMethods('date');
      if (empty($method_stack)) {
        // Set obtainer date stack to use if one has not been set by arguments.
        $method_stack = array();
        $method_stack[] = array('pluckTableRow1Col2');
        $method_stack[] = array('pluckTableRow1Col1');
        $method_stack[] = array('pluckTable2Row2Col2');
        $method_stack[] = array('pluckSelector', array("p[align='center']", 1));
        $method_stack[] = array('pluckSelector', array('#contentstart > p', 1));
        $method_stack[] = array('pluckSelector', array('.newsRight', 1));
        $method_stack[] = array('pluckSelector', array('.BottomLeftContent', 1));
        $method_stack[] = array('pluckProbableDate');
      }
      $this->setObtainerMethods(array('date' => $method_stack));
    }

    return parent::setDate($date);
  }
}
