<?php
/**
 * @file
 * District of Culombia Source Parsers.
 */

class DcPressSourceParser extends DistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  public function setBody($body = '') {
    $this->queryPath->find("div.BottomRightContent")->first()->remove();
    parent::setBody($body);
  }
}
