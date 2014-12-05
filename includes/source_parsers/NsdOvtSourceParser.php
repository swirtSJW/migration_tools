<?php

/**
 * @file
 * Class NsdOvtSourceParser
 */

class NsdOvtSourceParser extends SourceParser {

  /**
   * {@inheritdoc}
   */
  public function setBody($override = '') {

    foreach ($this->queryPath->find("img") as $img) {
      $alt = $img->attr("alt");
      print_r($alt . "\n");
      $checks = array(
        "Foreign Prosecutions",
        "Banner",
        "U.S. Prosecutions",
        "OVT",
      );
      foreach ($checks as $check) {
        if (substr_count($alt, $check) > 0) {
          $img->remove();
          break;
        }
      }
    }
    parent::setBody();
  }
}
