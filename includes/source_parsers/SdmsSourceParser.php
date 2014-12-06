<?php

/**
 * @file
 * Class SdmsSourceParser
 */

class SdmsSourceParser extends DistrictsSourceParser {

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    $selectors = array(
      "div.footerLeft",
      "div.footerRight",
      "div#footer",
      "div#header",
      "div.rightCol",
      "div#right",
      "div#skipnavigation",
      "div#top",
      "div#left > div#nav",
    );
    // Remove footer menu.
    HtmlCleanUp::removeElements($this->queryPath, $selectors);
  }
}
