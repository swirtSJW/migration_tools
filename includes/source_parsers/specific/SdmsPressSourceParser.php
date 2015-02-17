<?php

/**
 * @file
 * Class SdmsPressSourceParser
 */

class SdmsPressSourceParser extends DistrictPressReleaseSourceParser {

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    $selectors = array(
      "div#headSearch",
    );
    // Remove footer menu.
    HtmlCleanUp::removeElements($this->queryPath, $selectors);
  }
}
