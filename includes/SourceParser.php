<?php

/**
 * @file
 * Includes SourceParser class, which parses static HTML files via queryPath.
 */

// composer_manager is supposed to take care of including this library, but
// it doesn't seem to be working.
require DRUPAL_ROOT . '/sites/all/vendor/queryPath/queryPath/src/qp.php';

/**
 * Class SourceParser.
 *
 * @package doj_migration
 */
class SourceParser {

  protected $fileId;
  protected $html;
  public $queryPath;
  protected $title;

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
    $this->fileId = $file_id;
    $this->html = $html;

    $this->initQueryPath();
    $this->setTitle();

    // Calling $this->stripLegacyElements will remove a lot of markup, so some
    // properties (e.g., $this->title) must be set before calling it.
    $this->stripLegacyElements();

    // Empty anchors without name attribute will be stripped by ckEditor.
    $this->fixNamedAnchors();
    $this->convertRelativeSrcsToAbsolute();
  }

  /**
   * Create the queryPath object.
   */
  protected function initQueryPath() {
    $qp_options = array(
      'convert_to_encoding' => 'utf-8',
      'convert_from_encoding' => 'utf-8',
      'strip_low_ascii' => FALSE,
    );
    $this->queryPath = htmlqp($this->html, NULL, $qp_options);
  }

  /**
   * Removes legacy elements from HTML that are no longer needed.
   */
  protected function stripLegacyElements() {

    // Strip comments.
    foreach ($this->queryPath->top()->xpath('//comment()')->get() as $comment) {
      $comment->parentNode->removeChild($comment);
    }

    // Remove elements and their children.
    $this->queryPath->find('img[src="/gif/sealdoj.gif"]')->parent('p')->remove();
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
    $this->queryPath->find('.narrow-bar')->removeAttr('style');

    // Remove matching elements containing only &nbsp; or nothing.
    $this->removeEmptyElements(array(
      'div',
      'span',
    ));
  }

  /**
   * Empty anchors without name attribute will be stripped by ckEditor.
   */
  public function fixNamedAnchors() {
    $elements = $this->queryPath->find('a');
    foreach ($elements as $element) {
      $contents = trim($element->innerXHTML());
      if ($contents == '') {
        if ($anchor_id = $element->attr('id')) {
          $element->attr('name', $anchor_id);
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

    $elements = $this->queryPath->find(implode(', ', array_keys($attributes)));
    foreach ($elements as $element) {
      $attribute = $attributes[$element->tag()];

      $url = parse_url($element->attr($attribute));
      if ($url) {
        $is_relative = empty($url['scheme']) && !empty($url['path']) && substr($url['path'], 0, 1) !== '/';

        if ($is_relative) {
          $dir_path = dirname($this->fileId);
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
      $this->queryPath->find($selector)->children()->unwrap();
    }
  }

  /**
   * Removes elements matching CSS selectors.
   *
   * @param array $selectors
   *   An array of selectors to remove.
   */
  public function removeElements(array $selectors) {
    foreach ($selectors as $selector) {
      $this->queryPath->find($selector)->remove();
    }
  }

  /**
   * Removes empty elements matching selectors.
   *
   * @param array $selectors
   *   An array of selectors to remove.
   */
  public function removeEmptyElements(array $selectors) {
    foreach ($selectors as $selector) {
      $elements = $this->queryPath->find($selector);
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
   * Returns and removes last updated date from markup.
   *
   * @return string
   *   The update date.
   */
  public function extractUpdatedDate() {
    $element = trim($this->queryPath->find('.lastupdate'));
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
    $body = $this->queryPath
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
    $wrapper = $this->queryPath->find('.breadcrumbmenucontent');
    $wrapper->children('a, span, font')->remove();
    $title = $wrapper->text();

    // If there was no breadcrumb title, get from <title> tag.
    if (!$title) {
      $title = $this->queryPath->find('title')->innerHTML();
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
   *
   * @return string
   *   The trimmed string.
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
    $anchors = $this->queryPath->find('a[href^="mailto:"]');
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
    $states_blob = trim(file_get_contents(drupal_get_path('module', 'doj_migration') . '/sources/us-states.txt'));
    $states = explode("\n", $states_blob);
    $elements = $this->queryPath->find('p');
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
