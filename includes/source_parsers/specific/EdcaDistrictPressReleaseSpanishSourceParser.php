<?php

/**
 * @file
 * Class SdmsPressSourceParser
 */

class EdcaDistrictPressReleaseSpanishSourceParser extends DistrictPressReleaseSpanishSourceParser {

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    $selectors = array(
      "table",
    );
    // Remove footer menu.
    HtmlCleanUp::removeElements($this->queryPath, $selectors);
  }
}
