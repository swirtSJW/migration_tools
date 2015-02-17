<?php

/**
 * @file
 * Class NdwvSourceParser
 */

class NdwvPressSourceParser extends DistrictPressReleaseSourceParser {

  /**
   * {@inheritdoc}
   */
  public function setBody($override = "") {

    foreach ($this->queryPath->find("p") as $p) {
      $text = $p->text();
      if (substr_count($text, "(304) 234-0100") > 0) {
        $p->remove();
      }
    }

    parent::setBody($override);
  }
}
