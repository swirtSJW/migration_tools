<?php
/**
 * @file
 * Source parsers for the District of Minnesota.
 */

class MnPageSourceParser extends NGDistrictPageSourceParser {
  // @codingStandardsIgnoreStart
  protected $content_type;
  // @codingStandardsIgnoreEnd

  /**
   * Getter.
   */
  public function getContentType() {
    return $this->getProperty('content_type');
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
    parent::setDefaultObtainersInfo();
    $ct = new ObtainerInfo('content_type');
    $ct->addMethod("findType");
    $this->addObtainerInfo($ct);
  }
}

class MnPressSourceParser extends NGDistrictPressReleaseSourceParser {
  // @codingStandardsIgnoreStart
  protected $content_type;
  // @codingStandardsIgnoreEnd

  /**
   * Getter.
   */
  public function getContentType() {
    return $this->getProperty('content_type');
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
    parent::setDefaultObtainersInfo();
    $ct = new ObtainerInfo('content_type');
    $ct->addMethod("findType");
    $this->addObtainerInfo($ct);
  }
}
