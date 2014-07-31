<?php

/**
 * @file
 * Includes SourceParser class, which parses static HTML files via queryPath.
 */

// composer_manager is supposed to take care of including this library, but
// it doesn't seem to be working.
require DRUPAL_ROOT . '/sites/all/vendor/querypath/querypath/src/qp.php';

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

    $this->charTransform();
    $this->fixEncoding();
    $this->html = $this->changeSpecialforRegularChars($this->html);

    if ($fragment) {
      $this->wrapHTML();
    }
    $this->initQueryPath();
    $this->setTitle();

    // Calling $this->stripLegacyElements will remove a lot of markup, so some
    // properties (e.g., $this->title) must be set before calling it.
    $this->stripLegacyElements();

    // Empty anchors without name attribute will be stripped by ckEditor.
    $this->fixNamedAnchors();
    $this->removeExtLinkJS();
    $this->convertRelativeSrcsToAbsolute();

    // Some pages have images as subtitles. Turn those into html.
    $this->changeSubTitleImagesForHtml();
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
   * Replace characters.
   */
  protected function charTransform() {
    // We need to strip the Windows CR characters, because otherwise we end up
    // with &#13; in the output.
    // http://technosophos.com/content/querypath-whats-13-end-every-line
    $this->html = str_replace(chr(13), '', $this->html);
  }

  /**
   * Wrap an HTML fragment in the correct head/meta tags.
   *
   * This ensures that that UTF-8 is correctly detected.
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
      'img[src="/oip/foiapost/file_transfer.gif"]',
    ));

    // Remove black title bar with eagle image (if present).
    $this->removeTitleBarImage();

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
   * Removes legacy usage of javascript:exitWinOpen() for external links.
   */
  public function removeExtLinkJS() {
    $elements = $this->queryPath->find('a');

    // This should replace tags matching
    // <a href="javascript:exitWinOpen('http://example.com');">Example</a>
    // with <a href="http://example.com">Example</a>.
    $patterns[] = "|javascript:exitWinOpen\('([^']+)'\);|";

    // This should replace tags matching
    // <a href="/cgi-bin/outside.cgi?http://nccic.org/tribal/">Tribal</a>
    // with <a href="http://nccic.org/tribal/">Tribal</a>
    $patterns[] = "|/cgi-bin/outside.cgi\?([^']+)|";

    foreach ($elements as $element) {
      $href = $element->attr('href');
      if ($href) {
        foreach ($patterns as $pattern) {
          preg_match($pattern, $href, $matches);
          if (isset($matches) && !empty($matches[1])) {
            $new_url = $matches[1];
            $element->attr('href', $new_url);
          }
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
   * Remove eagle image title bar divs.
   *
   * Eagle image bars are always inside '<div style="margin-bottom:(15|20)px">'.
   * It appears that they are the only elements with this style applied.
   * Nonetheless, if more than one match, remove only the first.
   */
  public function removeTitleBarImage() {
    // Find divs that are immediately followed by img tags.
    $elements = $this->queryPath->find('div > img')->parent();
    foreach ($elements as $element) {
      // Eagle banner bars always preceed headlines.
      if (preg_match('/class=\"headline/', $element->html())) {
        break;
      }
      if (preg_match('/style=\"margin-bottom: ?(15|20)px/', $element->html())) {
        // We found an eagle image title bar: remove it and we're done.
        $element->remove();
        break;
      }
    }
  }

  /**
   * Change sub-header images to <h2> html titles.
   */
  public function changeSubTitleImagesForHtml() {
    // Find all headline divs with an image inside.
    $elements = $this->queryPath->find('div.headline > img')->parent();

    foreach ($elements as $element) {
      $image = $element->find('img');
      $alt = $image->attr('alt');
      $element->html("<h2>{$alt}</h2>");
    }
  }

  /**
   * Replace special chars with regular chars.
   */
  public function changeSpecialforRegularChars($text) {
    $text = str_replace(array("“", "”", "<93>", "<94>"), '"', $text);
    $text = str_replace(array("’", "‘", "<27>", "<91>", "<92>"), "'", $text);
    $text = str_replace("–", "-", $text);

    return $text;
  }

  /**
   * Replace special HTML chars.
   *
   * @param string $text
   *   The string where the special chars might be.
   *
   * @return string
   *   The string with the special chars replaced.
   */
  public function changeHTLMSpecialChars($text) {
    return htmlspecialchars_decode($text);
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
    $wrapper = $this->queryPath->find('.breadcrumbmenucontent')->first();
    $wrapper->children('a, span, font')->remove();
    $title = $wrapper->text();

    // If there was no breadcrumb title, get from <title> tag.
    if (!$title) {
      $title = $this->queryPath->find('title')->innerHTML();
    }

    // Clean string.
    $title = $this->removeUndesirableChars($title);
    $title = $this->removeWhitespace($title);
    $title = $this->changeSpecialforRegularChars($title);
    $title = $this->changeHTLMSpecialChars($title);

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
    $text = preg_replace('/^[\pZ\pC]+|[\pZ\pC]+$/u', '', $text);

    return $text;
  }

  /**
   * Remove undesirable chars from a string.
   */
  public function removeUndesirableChars($text) {
    return str_replace(array("»", "Â"), "", $text);
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
