<?php
/**
 * @file
 * Class that transforms a given url to the non-alias, non-redirected version.
 */

class LegacyUrlWithAnchorFixer {
  private $url;

  private $prefix;
  private $cleanUrl;
  private $anchor;
  private $redirect;

  /**
   * Constructor.
   */
  public function __construct($url) {
    $this->setUrl($url);
    $this->debug = FALSE;
  }

  /**
   * Url setter.
   */
  private function setUrl($url) {
    // URLs or URI that we want to work with should meet 2 conditions.
    // 1) They should have an anchor, 2) Can not be just an achor.
    if ($this->urlHasAnchor($url) && $this->urlIsNotJustAnAnchor($url)) {
      $this->url = $url;
    }
    else {
      throw new Exception("Url is just an anchor or does not contain an anchor");
    }
  }

  /**
   * Make usre the url has an anchor (ex. http://hello.com/boo#dah).
   */
  private function urlHasAnchor($url) {
    if (substr_count($url, "#") > 0) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Check that the url is not just an anchor (ex. #hello).
   */
  private function urlIsNotJustAnAnchor($url) {
    $first_char = substr($url, 0, 1);
    if (substr_count($first_char, "#") == 0) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Create a version of the given url, using drupals redirect.
   */
  public function fix() {
    $this->breakUrl();
    $this->redirect = $this->getRedirect();
    return $this->prefix . $this->redirect . '#' . $this->anchor;
  }

  /**
   * Break the url into prefix, uri, and anchor.
   */
  private function breakUrl() {
    $this->setPrefix();
    $this->setAnchor();
    $this->setCleanUrl();
  }

  /**
   * Isolate and store whatever is before the uri.
   */
  private function setPrefix() {
    // We could have a full url.
    if (substr_count($this->url, "justice.gov") > 0) {
      $pieces = explode("justice.gov/", $this->url);
      $this->prefix = $pieces[0] . "justice.gov/";
    }
    // Else we might have a rooted url (/) or a relative url (../).
    else {
      $position = 0;
      do {
        $first_char = substr($this->url, $position, 1);
        $position++;
      } while ($first_char == "." || $first_char == "/");

      $this->prefix = substr($this->url, 0, $position - 1);
    }
  }

  /**
   * Get the anchor part from the url.
   */
  private function setAnchor() {
    $pieces = explode("#", $this->url);
    $this->anchor = $pieces[1];
  }

  /**
   * Set the clean url variable. Url without prefix or anchor.
   */
  private function setCleanUrl() {
    $prefix_length = strlen($this->prefix);
    $clean_url = substr($this->url, $prefix_length);
    $clean_url = str_replace("#{$this->anchor}", "", $clean_url);
    $this->cleanUrl = $clean_url;
  }

  /**
   * Get a redirect matching the clean url as source.
   */
  private function getRedirect() {
    $query = db_select('redirect', 't');
    $query->fields("t", array('redirect'));
    $query->condition("source", $this->cleanUrl, "=");
    $results = $query->execute();
    foreach ($results as $result) {
      return $result->redirect;
    }

    throw new Exception("No redirect was found for this URL");
  }
}
