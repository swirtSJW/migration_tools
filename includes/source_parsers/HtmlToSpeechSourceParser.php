<?php
/**
 * @file
 * HtmlToSpeechSourceParser.
 */

class HtmlToSpeechSourceParser extends SourceParser {

  protected $city;
  protected $state;
  protected $country;
  protected $speechDate;

  /**
   * {@inheritdoc}
   */
  public function __construct($file_id, $html, $fragment = FALSE) {
    parent::__construct($file_id, $html, $fragment);

    // Get the location of the speech.
    $this->setLocation();

    // Get the date of the speech from the body.
    $this->setSpeechDate();

    // Since we are extracting the location, we need to reset the body.
    $this->setBody();
  }

  /**
   * Setter.
   */
  protected function setLocation() {
    try {
      module_load_include('inc', 'doj_migration', 'includes/HtmlCleanUp');

      // Location string.
      $ls = HtmlCleanUp::extractFirstElement($this->queryPath, '.speechlocation');

      $location = trim($ls);

      // @todo geoCodeString does not seem to belong in a migration class
      // it should be in the place where general functions are.
      $address = JusticeBaseMigration::geoCodeString($location);

      $this->city = $address['locality'];
      $this->state = $address['administrative_area_level_1'];
      $this->country = $address['country'];
    }
    catch (Exception $e) {
      watchdog("doj_migration", "{$this->fileid} failed to acquire a location, error: {$e->getMessage()}");
    }
  }

  /**
   * Getter.
   */
  public function getCity() {
    return $this->city;
  }

  /**
   * Getter.
   */
  public function getState() {
    return $this->state;
  }

  /**
   * Getter.
   */
  public function getCountry() {
    return $this->country;
  }

  /**
   * Setter.
   */
  protected function setSpeechDate() {
    try {
      $this->speechDate = "";
      // Speech date string.
      $sds = HtmlCleanUp::extractFirstElement($this->queryPath, '.speechdate');

      if (!empty($sds)) {
        $date = DateTime::createFromFormat("l, F j, Y", $sds);
        if ($date) {
          $this->speechDate = $date->format('Y-m-d');
        }
        else {
          watchdog("doj_migration", "{$this->fileId} date does not have the format l, F j, Y: {$sds}");
        }
      }
      else {
        watchdog("doj_migration", "{$this->fileId} failed to acquire a date");
      }
    }
    catch(Exception $e) {
      watchdog("doj_migration", "{$this->fileId} failed to acquire a date :error {$e->getMessage()}");
    }
  }

  /**
   * Getter.
   */
  public function getSpeechDate() {
    return $this->speechDate;
  }

  /**
   * Override the setTitle function to add extra logic.
   */
  protected function setTitle() {
    parent::setTitle();

    if (empty($this->title) || strcasecmp($this->title, "Justice News") == 0 || strcasecmp($this->title, "Discursos, Declaraciones y Testimonio") == 0) {
      try {
        $title = HtmlCleanUp::extractFirstElement($this->queryPath, ".presscontenttitle");
        if (!empty($title)) {
          $this->title = $title;
        }
      }
      catch(Exception $e) {
        watchdog('doj_migration', "{$this->fileId} failed to set title from speech body :error {$e->getMessage()}");
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function setBody() {
    // Get rid of the justice news header.
    try {
      HtmlCleanUp::removeElements($this->queryPath, array('.justicenews-header'));
    }
    catch(Exception $e) {
      watchdog("doj_migration", "{$this->fileId} failed to remove the justicenews header :error {$e->getMessage()}");
    }
    parent::setBody();
  }
}
