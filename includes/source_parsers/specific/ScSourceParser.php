<?php
/**
 * @file
 * Az related source parsers.
 */

class ScPressSourceParser extends DistrictPressReleaseSourceParser {

  /**
   * {@inheritdoc}
   */
  public function setBody() {
    $elems = $this->queryPath->find('a[alt = "Pledge"]');
    foreach ($elems as $elem) {
      $elem->remove();
    }
    parent::setBody();
  }
}
