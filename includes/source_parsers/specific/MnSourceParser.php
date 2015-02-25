<?php
/**
 * @file
 * Source parsers for the District of Minnesota.
 */

class MnPageSourceParser extends NGDistrictPageSourceParser {

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
    parent::setDefaultObtainersInfo();
    $ct = new ObtainerInfo('content_type');
    $ct->addMethod("findPRImmediateRelease");
    $this->addObtainerInfo($ct);
  }
}

class MnPressSourceParser extends NGDistrictPressReleaseSourceParser {

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
    parent::setDefaultObtainersInfo();
    $ct = new ObtainerInfo('content_type');
    $ct->addMethod("findPRImmediateRelease");
    $this->addObtainerInfo($ct);
  }
}
