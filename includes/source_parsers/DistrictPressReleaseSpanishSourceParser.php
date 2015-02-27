<?php

/**
 * @file
 * Class DistrictPressReleaseSpanishSourceParser
 */

class DistrictPressReleaseSpanishSourceParser extends DistrictPressReleaseSourceParser {
  /**
   * Setter for setting the date from a spanish source.
   */
  protected function setDate($date = '') {
    if (empty($date)) {
      $method_stack = $this->getObtainerMethods('date');
      if (empty($method_stack)) {
        // Set obtainer date stack to use if one has not been set by arguments.
        $method_stack = array(
          'findTableRow1Col2' => array(),
          'findTableRow1Col1' => array(),
          'findTable2Row2Col2' => array(),
          'findPAlignCenter' => array(),
          'findIdContentstartFirst' => array(),
          'findClassNewsRight' => array(),
          'findClassBottomLeftContent' => array(),
          'findProbableDate' => array(),
        );
      }
      $this->setObtainerMethods(array('date' => $method_stack));
      $date_string = $this->runObtainer('ObtainDateSpanish', 'date');
      $this->sourceParserMessage("Raw Date: @date_string", array('@date_string' => $date_string), WATCHDOG_DEBUG, 2);

      if (empty($date_string)) {
        $date = '';
      }
      else {
        $date = date('n/d/Y', strtotime($date_string));
      }
    }
    parent::setDate($date);
  }
}
