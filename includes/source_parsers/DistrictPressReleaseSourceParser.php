<?php

/**
 * @file
 * Class DistrictPressReleaseSourceParser
 */

class DistrictPressReleaseSourceParser extends PressReleaseSourceParser {


  /**
   * Defines and returns an array of parsing methods to call in order.
   *
   * @return array
   *   An array of parssing methods to call in order.
   */
  public function getParseOrder() {
    // Specify the parsing methods that should run in order.
    return array(
      // Getting the title relies on html that could be wiped during clean up
      // so let's get it before we clean things up.
      'setTitle',
      'setSubTitle',
      'setDate',
      'setID',
      // The title is set, so let's clean our html up.
      'cleanHtml',
      // With clean html we can get the body out.
      'setBody',
    );
  }

  /**
   * Setter.
   */
  protected function setID($override = '') {
    $default_target_stack = array(
      'pluckTable3y1x' => array(),
    );

    $om = $this->getObtainerMethods('id');
    $id_stack = (!empty($om)) ? $om : $default_target_stack;
    $this->setObtainerMethods(array('id' => $id_stack));

    parent::setID($override);
  }


  /**
   * Setter.
   */
  public function setSubTitle($override = '') {
    // If the subtitle has already been set, leave it alone.
    $st = $this->getSubTitle();
    if (empty($st)) {
      if (empty($override)) {
        $subtitle = $this->runObtainer('ObtainSubTitle', 'subtitle');
      }
      else {
        // The override was invoked, so use it.
        $subtitle = $override;
      }
      $this->subTitle = $subtitle;
    }
  }
}
