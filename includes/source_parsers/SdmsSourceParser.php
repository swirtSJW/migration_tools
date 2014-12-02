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
    );
    // Remove footer menu.
    HtmlCleanUp::removeElements($this->queryPath, $selectors);
  }
}
