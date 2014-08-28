<?php

/**
 * @file
 * Includes SourceParser class, which parses static HTML files via queryPath.
 */

/**
 * Class SourceParser.
 *
 * @package doj_migration
 */
class SourceParser {

  protected $fileId;
  protected $html;
  protected $title;
  protected $body;
  public $queryPath;

  /**
   * Constructor.
   *
   * @param string $file_id
   *   The file id, e.g. careers/legal/pm7205.html
   * @param string $html
   *   The full HTML data as loaded from the file.
   * @param bool $fragment
   *   Set to TRUE if there are no <html>,<head>, or <body> tags in the HTML.
   */
  public function __construct($file_id, $html, $fragment = FALSE) {
    // @todo We are not using $fragment, clean it up.
    $this->fileId = $file_id;
    $html = StringCleanUp::fixEncoding($html);

    // First we set the html var to something similar to what we receive.
    $this->setHtml($html, $fragment);

    // Getting the title relies on html that could be wiped during clean up
    // so let's get it before we clean things up.
    $this->setTitle();

    // The title is set, so let's clean our html up.
    $this->setCleanHtml($html, $fragment);

    // With clean html we can get the body out, and set our var.
    $this->setBody();

    // We don't use this here public var, keeping it for backwards compatablity.
    $this->queryPath = HtmlCleanUp::initQueryPath($this->html);
  }

  /**
   * Set the html var.
   */
  protected function setHtml($html, $fragment = FALSE) {
    $this->html = $html;
  }

  /**
   * Set the html var after some cleaning.
   */
  protected function setCleanHtml($html, $fragment = FALSE) {
    try {
      $html = StringCleanUp::fixEncoding($html);

      $html = HtmlCleanUp::convertRelativeSrcsToAbsolute($html, $this->fileId);

      // Strip Windows'' CR chars.
      $html = StringCleanUp::stripWindowsCRChars($html);

      // Clean up specific to the Justice site.
      $html = HtmlCleanUp::stripOrFixLegacyElements($html);

      $this->html = $html;
    }
    catch (Exception $e) {
      $this->html = $html;
      watchdog('doj_migration', '%file: failed to clean the html', array('%file' => $this->fileId), WATCHDOG_ALERT);
    }
  }

  /**
   * Sets $this->title using breadcrumb or <title>.
   */
  protected function setTitle() {

    // QueryPathing our way through things can fail.
    // In that case let's still set things and inform people about the issue.

    try {
      // First attempt to get the title from the breadcrumb.
      $query_path = HtmlCleanUp::initQueryPath($this->html);
      $wrapper = $query_path->find('.breadcrumbmenucontent')->first();
      $wrapper->children('a, span, font')->remove();
      $title = $wrapper->text();

      // If there was no breadcrumb title, get it from the <title> tag.
      if (!$title) {
        $title = $query_path->find('title')->innerHTML();
      }
      // If there are any html special chars let's change those to its char
      // equivalent.
      $title = html_entity_decode($title, ENT_COMPAT, 'UTF-8');

      // There are also numeric html special chars, let's change those.
      module_load_include('inc', 'doj_migration', 'includes/doj_migration');
      $title = doj_migration_html_entity_decode_numeric($title);

      // We want out titles to be only digits and ascii chars so we can produce
      // clean aliases.
      $title = StringCleanUp::convertNonASCIItoASCII($title);

      // Remove undesirable chars.
      $title = str_replace("»", "", $title);

      // We also want to trim the string.
      $title = StringCleanUp::superTrim($title);

      // $title = $this->removeUndesirableChars($title);
      // $title = $this->changeSpecialforRegularChars($title);

      // Truncate title to max of 255 characters.
      if (strlen($title) > 255) {
        $title = substr($title, 0, 255);
      }
      $this->title = $title;
    }
    catch (Exception $e) {
      $this->title = "";
      watchdog('doj_migration', '%file: failed to set the title', array('%file' => $this->fileId), WATCHDOG_ALERT);
    }
  }

  /**
   * Return the title for this content.
   */
  public function getTitle() {
    // The title gets set in the constructor, no need to check here.
    return $this->title;
  }


  /**
   * Get the body from html and set the body var.
   */
  protected function setBody() {
    try {
      $query_path = HtmlCleanUp::initQueryPath($this->html);
      $body = $query_path->top('body')->innerHTML();

      $enc = mb_detect_encoding($body, 'UTF-8', TRUE);
      if (!$enc) {
        watchdog("doj_migration", "%file body needed its encoding fixed!!!", array('%file' => $this->fileId), WATCHDOG_NOTICE);
      }

      $body = StringCleanUp::fixEncoding($body);
      $this->body = $body;
    }
    catch (Exception $e) {
      $this->body = "";
      watchdog('doj_migration', '%file: failed to set the body', array('%file' => $this->fileId), WATCHDOG_ALERT);
    }
  }

  /**
   * Return content of <body> element.
   */
  public function getBody() {
    // The body gets set in the constructor, no need to check for existance.
    return $this->body;
  }

  /**
   * Returns and removes last updated date from markup.
   *
   * @return string
   *   The update date.
   */
  public function extractUpdatedDate() {
    $query_path = HtmlCleanUp::initQueryPath($this->html);
    $element = trim($query_path->find('.lastupdate'));
    $contents = $element->text();
    if ($contents) {
      $contents = trim(str_replace('Updated:', '', $contents));
    }

    // Here we are modifying html, so let's set our global again.
    $element->remove();
    $this->html = $query_path->html();

    return $contents;
  }

  /**
   * Returns the contents of <a href="mailto:*" /> elements in <body>.
   *
   * @return null|string
   *   A string of email addresses separated by pipes.
   */
  public function getEmailAddresses() {
    $query_path = HtmlCleanUp::initQueryPath($this->html);
    $anchors = $query_path->find('a[href^="mailto:"]');
    if ($anchors) {
      $email_addresses = array();
      foreach ($anchors as $anchor) {
        $email_addresses[] = $anchor->text();
      }
      $email_addresses = implode('|', $email_addresses);
      return $email_addresses;
    }

    return NULL;
  }

  /**
   * Crude search for strings matching US States.
   */
  public function getUsState() {
    $query_path = HtmlCleanUp::initQueryPath($this->html);
    $states_blob = trim(file_get_contents(drupal_get_path('module', 'doj_migration') . '/sources/us-states.txt'));
    $states = explode("\n", $states_blob);
    $elements = $query_path->find('p');
    foreach ($elements as $element) {
      foreach ($states as $state) {
        list($abbreviation, $state_title) = explode('|', $state);
        if (strpos(strtolower($element->text()), strtolower($state_title)) !== FALSE) {
          return $abbreviation;
        }
      }
    }
    return NULL;
  }
}

/**
 * Don't think we need this code any more, but I will leave it in here
 * just in case
 */
/**
public function changeSpecialForHtmlSpecialChars($text) {
  $special = array("»" => "&raquo;");

  foreach ($special as $find => $replace) {
    $text = str_replace($find, $replace, $text);
  }

  return $text;
}

public function removeUndesirableChars($text, $exclusions = array()) {
  $undesirables = array("Â");

  $undesirables = array_diff($undesirables, $exclusions);

  foreach ($undesirables as $remove_char) {
    $text = str_replace($remove_char, "", $text);
  }

  return $text;
}

public function changeSpecialforRegularChars($text) {
  $text = str_replace(array("“", "”", "<93>", "<94>"), '"', $text);
  $text = str_replace(array("’", "‘", "<27>", "<91>", "<92>"), "'", $text);
  $text = str_replace("–", "-", $text);

  return $text;
}

 public function changeElementTag($selectors) {
    foreach ($selectors as $old_selector => $new_selector) {
      $elements = $this->queryPath->find($old_selector);
      foreach ($elements as $element) {
        $element->wrapInner('<' . $new_selector . '></' . $new_selector . '>');
        $element->children($new_selector)->first()->unwrap($old_selector);
      }
    }
  }
*/
