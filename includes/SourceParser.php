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
  protected $title;

  /**
   * Constructor.
   *
   * @param $id
   *  The filepath, e.g. careers/legal/pm7205.html
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
    $this->setTitle();
    $this->stripComments();

    // Calling $this->stripLegacyElements will remove a lot of markup, so some
    // properties (e.g., $this->title) must be set before calling it.
    $this->stripLegacyElements();
    $this->convertRelativeSrcsToAbsolute();
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

    // Remove elements and their children.
    $this->qp->find('img[src="/gif/sealdoj.gif"]')->parent('p')->remove();
    $this->removeElements(array(
      'a[name="sitemap"]',
      'a[name="maincontent"]',
      'img[src="/gif/sealdoj.gif"]',
      'div.skip',
      'div.hdrwrpr',
      'div.breadcrumbmenu',
      'div.footer',
      'div.clear',
      'div.lastupdate',
      'div.thick-bar',
      'div.rightcolumn',
    ));

    // Remove extraneous html wrapping elements, leaving children intact.
    $this->removeWrapperElements(array(
      'body > blockquote',
      '.bdywrpr',
      '.gridwrpr',
      '.leftcol-subpage',
      '.leftcol-subpage-content',
      '.bodytextbox',
      'body > div',
    ));

    // Remove style attribute from elements.
    $this->qp->find('.narrow-bar')->removeAttr('style');

    // Remove matching elements containing only &nbsp; or nothing.
    $this->removeEmptyElements(array(
      'div',
      'span',
    ));

    // Empty anchors without name attribute will be stripped by ckEditor.
    $this->fixNamedAnchors();
  }

  /**
   * Empty anchors without name attribute will be stripped by ckEditor.
   */
  public function fixNamedAnchors() {
    $elements = $this->qp->find('a');
    foreach ($elements as $element) {
      $contents = trim($element->innerXHTML());
      if ($contents == '') {
        if ($id = $element->attr('id')) {
          $element->attr('name', $id);
        }
      }
    }
  }

  /**
   * Makes relative sources values on <a> and <img> tags absolute.
   */
  public function convertRelativeSrcsToAbsolute() {
    // A list of attributes to convert, keyed by HTML tag (NOT selector).
    $attributes = array(
      'img' => 'src',
      'a' => 'href',
    );

    $elements = $this->qp->find(implode(', ', array_keys($attributes)));
    foreach ($elements as $element) {
      $attribute = $attributes[$element->tag()];

      $url = parse_url($element->attr($attribute));
      if ($url) {
        $is_relative = empty($url['scheme']) && !empty($url['path']) && substr($url['path'], 0, 1) !== '/';

        if ($is_relative) {
          $dir_path = dirname($this->id);
          $new_url = '/' . $dir_path . '/' . $url['path'];
          $element->attr($attribute, $new_url);
        }
      }
    }
  }

  /**
   * Removes a wrapping element, leaving children intact.
   *
   * @param array $selectors
   *   An array of selectors for the wrapping element(s).
   */
  public function removeWrapperElements(array $selectors) {
    foreach ($selectors as $selector) {
      $this->qp->find($selector)->children()->unwrap();
    }
  }

  /**
   * @param array $selectors
   *   An array of selectors to remove.
   */
  public function removeElements(array $selectors) {
    foreach ($selectors as $selector) {
      $this->qp->find($selector)->remove();
    }
  }

  /**
   * @param array $selectors
   */
  public function removeEmptyElements(array $selectors) {
    foreach ($selectors as $selector) {
      $elements = $this->qp->find($selector);
      foreach ($elements as $element) {
        $contents = trim($element->innerXHTML());
        $empty_values = array(
          '&nbsp;',
          '',
        );
        if (in_array($contents, $empty_values)) {
          $element->remove();
        }
      }
    }
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
   * Returns and removes last updated date from markup.
   *
   * @return string
   *   The update date.
   */
  public function extractUpdatedDate() {
    $element = trim($this->qp->find('.lastupdate'));
    $contents = $element->text();
    if ($contents) {
      $contents = trim(str_replace('Updated:', '', $contents));
    }
    $element->remove();

    return $contents;
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
   * Sets $this->title using breadcrumb or <title>.
   */
  public function setTitle() {
    // First attempt to get the title from the breadcrumb.
    $wrapper = $this->qp->find('.breadcrumbmenucontent');
    $wrapper->children('a, span, font')->remove();
    $title = $wrapper->text();

    // If there was no breadcrumb title, get from <title> tag.
    if (!$title) {
      $title = $this->qp->find('title')->innerHTML();
    }

    // Clean string.
    $title = $this->removeWhitespace($title);

    // Truncate title to max of 255 characters.
    if (strlen($title) > 255) {
      $title = substr($title, 0, 255);
    }

    $this->title = $title;
  }

  /**
   * Removes various types of whitespace from a string.
   *
   * @param string $text
   *   The text string from which to remove whitespace.
   * @return string
   */
  public function removeWhitespace($text) {
    $text = trim(str_replace('&nbsp;', '', $text));
    // Remove unicode whitespace
    // @see http://stackoverflow.com/questions/4166896/trim-unicode-whitespace-in-php-5-2
    $text = preg_replace('/^\p{Z}+|\p{Z}+$/u', '', $text);

    return $text;
  }

  /**
   * Return the title for this content.
   */
  public function getTitle() {
    if (!isset($this->title)) {
      $this->setTitle();
    }

    return $this->title;
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

  /**
   * Crude search for strings matching US States.
   */
  public function getUsState() {
    $states_blob = file_get_contents(drupal_get_path('module', 'doj_migration') . '/sources/us-states.txt');
    $states = explode("\n", $states_blob);
    $elements = $this->qp->find('p');
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
