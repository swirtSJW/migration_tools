<?php
/**
 * @file
 * EdpaPressSourceParser.
 */

class EdpaPressSourceParser extends DistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  public function setBody() {
    $a = HtmlCleanUp::matchText($this->queryPath, "a", "index-news");
    if ($a) {
      $a->remove();
    }
    parent::setBody();
  }
}
