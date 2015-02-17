<?php
/**
 * @file
 * Az related source parsers.
 */

class NdiaPressSourceParser extends DistrictPressReleaseSourceParser {

  /**
   * {@inheritdoc}
   */
  public function setBody() {
    $match = HtmlCleanUp::matchText($this->queryPath, "p", "Peter Deegan");
    if ($match) {
      $match->remove();
    }
    parent::setBody();
  }
}
