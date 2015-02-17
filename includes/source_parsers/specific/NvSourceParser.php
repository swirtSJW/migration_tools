<?php
/**
 * @file
 * NV related source parsers.
 */

class NvPressSourceParser extends DistrictPressReleaseSourceParser {

  /**
   * {@inheritdoc}
   */
  public function setBody() {
    foreach ($this->queryPath->find("div[align='center']") as $div) {
      $div->removeAttr('align');
    }
    parent::setBody();
  }
}
