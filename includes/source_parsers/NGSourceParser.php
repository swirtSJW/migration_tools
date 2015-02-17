<?php

/**
 * @file
 * Includes SourceParser class, which parses static HTML files via queryPath.
 */

// composer_manager is supposed to take care of including this library, but
// it doesn't seem to be working.
require DRUPAL_ROOT . '/sites/all/vendor/querypath/querypath/src/qp.php';

/**
 * Class NGSourceParser.
 *
 * @package doj_migration
 */
abstract class NGSourceParser {

  protected $obtainersInfo;
  protected $fileId;
  protected $html;

  public $queryPath;

  /**
   * A specific source parser class should set useful set of obtainers info.
   */
  abstract protected function setDefaultObatinersInfo();

  /**
   * Constructor.
   *
   * @param string $file_id
   *   The file id, e.g. careers/legal/pm7205.html
   * @param string $html
   *   The full HTML data as loaded from the file.
   */
  public function __construct($file_id, $html) {
    $this->fileId = $file_id;

    $html = StringCleanUp::fixEncoding($html);
    $html = StringCleanUp::stripWindowsCRChars($html);
    $html = StringCleanUp::fixWindowSpecificChars($html);
    $this->html = $html;

    $this->setDefaultObatinersInfo();
    $this->drushPrintSeparator();
  }

  /**
   * Add obtainer info for this source parser to use.
   */
  public function addObtainerInfo(ObtainerInfo $oi) {
    $this->obtainersInfo[$oi->getProperty()] = $oi;
  }

  /**
   * Get information/properties from html by running the obtainers.
   */
  protected function getProperty($property) {
    $this->setProperty($property);

    // We can just return the property as any issue should throw an exception
    // form setProperty.
    return $this->{$property};
  }

  /**
   * Set a property.
   */
  protected function setProperty($property) {
    // If it is set, no need to do it again.
    if (isset($this->{$property})) {
      return;
    }

    // Make sure our querypath object has been initialized.
    $this->initQueryPath();

    // Obtain the property using obtainers.
    $this->{$property} = $this->obtainProperty($property);
  }

  /**
   * Use the obtainers mechanism to extract text from the html.
   */
  private function obtainProperty($property) {
    $text = '';

    $obtainer_info = $this->obtainersInfo[$property];

    if (!isset($obtainer_info)) {
      throw new Exception("NGSourceParser does not have obatinaer info for the {$property} property");
    }

    try {
      $class = $obtainer_info->getClass();
      $methods = $obtainer_info->getMethods();

      $obtainer = new $class($this->queryPath, $methods);
      $this->sourceParserMessage("Obtaining @key via @obtainer_class", array('@key' => $property, '@obtainer_class' => $class));

      $text = $obtainer->obtain();
      $this->sourceParserMessage('@property found --> @text', array('@property' => $property, '@text' => $text), WATCHDOG_DEBUG, 2);
    }
    catch (Exception $e) {
      $this->sourceParserMessage("Failed to set @key, Exception: @error_message", array(
        '@key' => $property,
        '@error_message' => $e->getMessage(),
      ), WATCHDOG_ERROR);
    }

    return $text;
  }

  /**
   * Create the queryPath object.
   */
  private function initQueryPath() {
    // If query path is already initialized, get out.
    if (isset($this->queryPath)) {
      return;
    }

    $type_detect = array(
      'UTF-8',
      'ASCII',
      'ISO-8859-1',
      'ISO-8859-2',
      'ISO-8859-3',
      'ISO-8859-4',
      'ISO-8859-5',
      'ISO-8859-6',
      'ISO-8859-7',
      'ISO-8859-8',
      'ISO-8859-9',
      'ISO-8859-10',
      'ISO-8859-13',
      'ISO-8859-14',
      'ISO-8859-15',
      'ISO-8859-16',
      'Windows-1251',
      'Windows-1252',
      'Windows-1254',
    );
    $convert_from = mb_detect_encoding($this->html, $type_detect);

    $qp_options = array(
      'convert_to_encoding' => 'UTF-8',
      'convert_from_encoding' => $convert_from,
    );

    // Create query path object.
    $this->queryPath = htmlqp($this->html, NULL, $qp_options);

    if (!is_object($this->queryPath)) {
      throw new Exception("{$this->fileId} failed to initialize QueryPath");
    }
  }

  /**
   * Prints a log message separator to drush.
   */
  protected function drushPrintSeparator() {
    if (drupal_is_cli() && variable_get('doj_migration_drush_debug', FALSE)) {
      drush_print(str_repeat('-', 40));
      $this->sourceParserMessage('@class: @file_id:', array('@class' => get_class($this), '@file_id' => $this->fileId), WATCHDOG_DEBUG, 0);
    }
  }

  /**
   * Logs a system message.
   *
   * @param string $message
   *   The message to store in the log. Keep $message translatable
   *   by not concatenating dynamic values into it! Variables in the
   *   message should be added by using placeholder strings alongside
   *   the variables argument to declare the value of the placeholders.
   *   See t() for documentation on how $message and $variables interact.
   * @param array $variables
   *   Array of variables to replace in the message on display or
   *   NULL if message is already translated or not possible to
   *   translate.
   * @param int $severity
   *   The severity of the message; one of the following values as defined in
   *   - WATCHDOG_EMERGENCY: Emergency, system is unusable.
   *   - WATCHDOG_ALERT: Alert, action must be taken immediately.
   *   - WATCHDOG_CRITICAL: Critical conditions.
   *   - WATCHDOG_ERROR: Error conditions.
   *   - WATCHDOG_WARNING: Warning conditions.
   *   - WATCHDOG_NOTICE: (default) Normal but significant conditions.
   *   - WATCHDOG_INFO: Informational messages.
   *   - WATCHDOG_DEBUG: Debug-level messages.
   *
   * @param int $indent
   *   (optional). Sets indentation for drush output. Defaults to 1.
   *
   * @link http://www.faqs.org/rfcs/rfc3164.html RFC 3164: @endlink
   */
  protected function sourceParserMessage($message, $variables = array(), $severity = WATCHDOG_NOTICE, $indent = 1) {
    $type = get_class($this);
    watchdog($type, $message, $variables, $severity);

    if (drupal_is_cli() && variable_get('doj_migration_drush_debug', FALSE)) {
      $formatted_message = format_string($message, $variables);
      drush_print($formatted_message, $indent);
    }
  }
}

/**
 * Information about which property we are dealing with.
 *
 * Including the class and methods to be called in that obtainer.
 */
class ObtainerInfo {
  private $property;
  private $class;
  private $methods = array();

  /**
   * Constructor.
   */
  public function __construct($property, $class = "") {
    $this->property = $property;

    $pieces = explode("_", $property);
    $class = "";
    foreach ($pieces as $piece) {
      $class .= ucfirst($piece);
    }
    $this->setClass("Obtain{$class}");
  }

  /**
   * Setter.
   */
  private function setClass($class) {
    // @todo Maybe we should validate the class here.
    $this->class = $class;
  }

  /**
   * Getter.
   */
  public function getProperty() {
    return $this->property;
  }

  /**
   * Getter.
   */
  public function getClass() {
    return $this->class;
  }

  /**
   * Add a new method to be called during obtainer processing.
   */
  public function addMethod($method_name) {
    // @todo Maybe we should validate the method names here?
    $this->methods[] = $method_name;
  }

  /**
   * Getter.
   */
  public function getMethods() {
    return $this->methods;
  }
}
