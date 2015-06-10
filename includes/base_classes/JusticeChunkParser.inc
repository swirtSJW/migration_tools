<?php

/**
 * @file
 * Contains classes for chunking HTML files.
 */

/**
 * Class JusticeChunkParser
 *
 * Parses a single HTML file into many chunks, each of which is mapped to a
 * separate destination node. Used for 'many to many' migrations where
 * each file contains content for more than one destination.
 *
 * @package doj_migration
 */
abstract class JusticeChunkParser extends MigrateContentParser {
  /**
   * $this->setContent() will be called by the migrate module. Other methods
   * can rely on this being set.
   */
  protected $content;
  protected $sourceParser;
  protected $chunks;

  /**
   * {@inheritdoc}
   */
  public function setContent($content) {
    parent::setContent($content);

    // Create a new SourceParser to handle HTML content.
    // We don't need the full ID now. Migrate will generate it and make it
    // available in prepareRow().
    $this->sourceParser = new SimpleSourceParser('placeholder', $this->content);
    $this->setChunks();
  }

  /**
   * Gets $this->chunks.
   *
   * @return QueryPath
   *   A QueryPath object containing one or more elements.
   */
  public function getChunks() {
    if (!isset($this->chunks)) {
      $this->setChunks();
    }
    return $this->chunks;
  }

  /**
   * {@inheritdoc}
   */
  public function getChunkCount() {
    $chunks = $this->getChunks();
    return count($chunks);
  }

  /**
   * Sets $this->chunks.
   *
   * This method should parse that markup in $this->content and return an array
   * of discrete content chunks.
   */
  abstract public function setChunks();

  /**
   * Logs a system message and optionally outputs to drush terminal.
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
  protected function chunkparserMessage($message, $variables = array(), $severity = WATCHDOG_NOTICE, $indent = 1) {
    $type = get_class($this);
    if ($severity) {
      watchdog($type, $message, $variables, $severity);
    }
    if (drupal_is_cli() && variable_get('doj_migration_drush_debug', FALSE)) {
      $formatted_message = format_string($message, $variables);
      drush_print("$type: $formatted_message", $indent);
      if ((variable_get('doj_migration_drush_stop_on_error', FALSE)) && ($severity <= WATCHDOG_ERROR)) {
        throw new MigrateException("$type: Stopped for debug.\n -- Run \"drush mi {migration being run}\" to try again. \n -- Run \"drush vset doj_migration_drush_stop_on_error FALSE\" to disable auto-stop.");
      }
    }
  }
}
