<?php
/**
 * @file
 * UT related source parsers.
 */

class UtPressSourceParser extends DistrictPressReleaseSourceParser {

  /**
   * {@inheritdoc}
   */
  public function setBody() {
    foreach ($this->queryPath->find("p[align='center']") as $p) {
      $p->removeAttr('align');
    }
    parent::setBody();
  }
}
