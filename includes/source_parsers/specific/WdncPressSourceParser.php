<?php

/**
 * @file
 * Class WdncPressSourceParser
 */

class WdncPressSourceParser extends DistrictPressReleaseSourceParser {

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    $selectors = array(
      "c",
    );
    // Remove footer menu.
    HtmlCleanUp::removeElements($this->queryPath, $selectors);
  }
}
