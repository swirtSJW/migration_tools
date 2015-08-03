<?php
/**
 * @file
 * Includes NGCaseSourceParser class.
 *
 * This class contains customization to parse cases.
 */

/**
 * Class NGCaseSourceParser.
 *
 * @package migration_tools
 */

abstract class NGCaseSourceParser extends NGNodeSourceParser {
  protected $date;
  protected $link;
  protected $overview;

  /**
   * Getter.
   */
  public function getDate() {
    $date_string = $this->getProperty('date');
    new MigrationMessage("Raw Date: @date_string", array('@date_string' => $date_string), WATCHDOG_DEBUG, 2);

    if (empty($date_string)) {
      $date = '';
    }
    else {
      $date = date('n/d/Y', strtotime($date_string));
      if (!empty($date)) {
        // Output success to show progress to aid debugging.
        new MigrationMessage("Formatted Date: @date", array('@date' => $date), WATCHDOG_DEBUG, 2);
      }
    }

    return $date;
  }

  /**
   * Getter $this->subtitle property.
   */
  public function getLink() {
    $link = $this->getProperty('link');

    return $link;
  }

  /**
   * Gets $this->prNumber property.
   */
  public function getOverview() {
    $overview = $this->getProperty('overview');

    return $overview;
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
    parent::setDefaultObtainersInfo();

    $date = new ObtainerInfo('date', "ObtainDate");
    $this->addObtainerInfo($date);

    $link = new ObtainerInfo('link', "ObtainLink");
    $this->addObtainerInfo($link);

    $overview = new ObtainerInfo('overview', "ObtainOverview");
    $this->addObtainerInfo($overview);
  }
}
