<?php

/**
 * @file
 * Includes SourceParser class, which parses static HTML files via QueryPath.
 */

// composer_manager is supposed to take care of including this library, but
// it doesn't seem to be working.
require DRUPAL_ROOT . '/sites/all/vendor/querypath/querypath/src/qp.php';

class SourceParser {
  protected $id;
  protected $html;
  public $qp;

  /**
   * Constructor.
   *
   * @param $id
   *  The filename, e.g. pm7205.html
   * @param $html
   *  The full HTML data as loaded from the file.
   * @param boolean $fragment
   *   Set to TRUE if there are no <html>,<head>, or <body> tags in the HTML.
   *
   */
  public function __construct($id, $html, $fragment = FALSE) {
    $this->id = $id;
    $this->html = $html;

    $this->charTransform();
    //$this->fixEncoding();

    if ($fragment) {
      $this->wrapHTML();
    }

    $this->initQP();

    if (!$fragment) {
      $this->addUtf8Metatag();
    }
    $this->stripComments();
    $this->stripLegacyElements();
  }

  /**
   * Replace characters.
   */
  protected function charTransform() {
    // We need to strip the Windows CR characters, because otherwise we end up
    // with &#13; in the output.
    // http://technosophos.com/content/querypath-whats-13-end-every-line
    $this->html = str_replace(chr(13), '', $this->html);
  }

  /**
   * Deal with encodings.
   */
  protected function fixEncoding() {
    // If the content is not UTF8, we assume it's WINDOWS-1252. This fixes
    // bogus character issues. Technically it could be ISO-8859-1 but it's safe
    // to convert this way.
    // http://en.wikipedia.org/wiki/Windows-1252
    $enc = mb_detect_encoding($this->html, 'UTF-8', TRUE);
    if (!$enc) {
      $this->html = mb_convert_encoding($this->html, 'UTF-8', 'WINDOWS-1252');
    }
  }

  /**
   * Wrap an HTML fragment in the correct head/meta tags so that UTF-8 is
   * correctly detected, and for the parsers and tidiers.
   */
  protected function wrapHTML() {
    // We add surrounding <html> and <head> tags.
    $html = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">';
    $html .= '<html xmlns="http://www.w3.org/1999/xhtml"><head><meta http-equiv="Content-Type" content="text/html; charset=utf-8" /></head><body>';
    $html .= $this->html;
    $html .= '</body></html>';
    $this->html = $html;
  }

  /**
   * Adds an <meta> tag setting charset to UTF-8 for a full HTML page.
   */
  protected function addUtf8Metatag() {
    $metatag = '<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />';
    $this->qp->find('head')->append($metatag);
  }

  /**
   * Create the QueryPath object.
   */
  protected function initQP() {
    $qp_options = array(
      'convert_to_encoding' => 'utf-8',
      'convert_from_encoding' => 'utf-8',
      'strip_low_ascii' => FALSE,
    );
    $this->qp = htmlqp($this->html, NULL, $qp_options);
  }

  /**
   * Remove the comments from the HTML.
   */
  protected function stripComments() {
    foreach ($this->qp->top()->xpath('//comment()')->get() as $comment) {
      $comment->parentNode->removeChild($comment);
    }
  }

  /**
   * Removes legacy elements from HTML that are no longer needed.
   */
  protected function stripLegacyElements() {
    // Remove logo and containing <p>.
    $this->qp->find('p > img[src="/gif/sealdoj.gif"]')->parent()->remove();
    // Remove logo if there was no containing <p>.
    $this->qp->find('img[src="/gif/sealdoj.gif"]')->remove();

    // Remove <blockquote> wrapper from elements in <body>.
    $this->qp->find('body > blockquote')->children()->unwrap();
  }

  /**
   * Returns and removes an inline title from the <body>.
   *
   * Matches only <p><u><strong>Label</strong></u> Text</p> or
   * <p><strong><u>Label</u></strong>: Text</p>.
   *
   * @param mixed $labels
   *   The label text for which to search, or a flat array of labels. The first
   *   label matched will be used.
   *
   * @return mixed
   *   FALSE if no match was found, otherwise, value of labeled <p>.
   */
  public function extractInlineTitle($labels) {

    if (is_string($labels)) {
      $labels = array($labels);
    }

    foreach ($labels as $label) {
      // Process body markup.
      $parent = $this->qp
        ->xpath("//strong[contains(text(), '$label')] | //u[contains(text(), '$label')]")
        ->parent('p');
      if ($parent) {
        $parent->remove('u');
        $parent->remove('strong');

        // Remove leading ':'. Double trim is intentional.
        $value = trim($parent->innerHTML());
        if (strpos($value, ':') === 0) {
          $value = substr($value, 1);
        }
        $value = trim($value);

        // Remove parent element from <body>.
        $parent->remove();

        return $value;
      }
    }

    return NULL;
  }

  /**
   * Return content of <body> element.
   */
  public function getBody() {
    $body = $this->qp
      ->top('body')
      ->innerHTML();
    $body = trim($body);
    return $body;
  }

  /**
   * Return content of <title> element.
   */
  public function getTitle() {
    $title = $this->qp->find('title')->innerHTML();
    $title = trim($title);

    // Truncate title to max of 255 characters.
    if (strlen($title) > 255) {
      $title = substr($title, 0, 255);
    }

    return $title;
  }

  /**
   * Returns the contents of <a href="mailto:*" /> elements in <body>.
   *
   * @return null|string
   *   A string of email addresses separated by pipes.
   */
  public function getEmailAddresses() {
    $anchors = $this->qp->find('a[href^="mailto:"]');
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
}
