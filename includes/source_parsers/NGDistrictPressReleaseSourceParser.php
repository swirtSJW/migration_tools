<?php
/**
 * @file
 * Includes NGDistrictPressReleaseSourceParser class.
 *
 * This class contains the customization for NGPrssReleaseSourceParser to
 * parse District press releases.
 */

/**
 * Class NGDicstrictPressReleaseSourceParser.
 *
 * @package doj_migration
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
  protected function setDefaultObtainersInfo() {
    parent::setDefaultObtainersInfo();

    $id = new ObtainerInfo("id");
    $id->addMethod('findTable3y1x');
    $this->addObtainerInfo($id);
  }
}
