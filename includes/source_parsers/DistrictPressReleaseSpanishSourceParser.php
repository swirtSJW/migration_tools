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
          'pluckTableRow1Col2' => array(),
          'pluckTableRow1Col1' => array(),
          'pluckTable2Row2Col2' => array(),
          'pluckSelector' => array("p[align='center']", 1),
          'pluckSelector' => array('#contentstart > p', 1),
          'pluckSelector' => array('.newsRight', 1),
          'pluckSelector' => array('.BottomLeftContent', 1),
          'pluckProbableDate' => array(),
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
