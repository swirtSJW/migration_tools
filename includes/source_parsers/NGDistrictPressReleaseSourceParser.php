<?php

/**
 * @file
 * Class NGDistrictPressReleaseSourceParser
 */

class NGDistrictPressReleaseSourceParser extends NGPressReleaseSourceParser {

  /**
   * Getter.
   */
  public function getID() {
    return $this->getProperty('id');
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObatinersInfo() {
    parent::setDefaultObatinersInfo();

    $id = new ObtainerInfo("id");
    $id->addMethod('findTable3y1x');
    $this->addObtainerInfo($id);
  }
}
