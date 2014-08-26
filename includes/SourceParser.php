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
    $this->queryPath = HTMLCleanUp::initQueryPath($this->html);
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

    $html = StringCleanUp::fixEncoding($html);

    $html = HTMLCleanUp::convertRelativeSrcsToAbsolute($html, $this->fileId);

    // Strip Windows'' CR chars.
    $html = StringCleanUp::stripWindowsCRChars($html);

    // Clean up specific to the Justice site.
    $html = HTMLCleanUp::stripOrFixLegacyElements($html);

    $this->html = $html;
  }

  /**
   * Sets $this->title using breadcrumb or <title>.
   */
  protected function setTitle() {
    // First attempt to get the title from the breadcrumb.
    $query_path = HTMLCleanUp::initQueryPath($this->html);
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
    $query_path = HTMLCleanUp::initQueryPath($this->html);
    $body = $query_path->top('body')->innerHTML();
    $this->body = $body;
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
    $query_path = HTMLCleanUp::initQueryPath($this->html);
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
    $query_path = HTMLCleanUp::initQueryPath($this->html);
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
    $query_path = HTMLCleanUp::initQueryPath($this->html);
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

class HTMLCleanUp {

  /**
   * Create the queryPath object.
   */
  public static function initQueryPath($html) {
    $qp_options = array();
    return htmlqp($html, NULL, $qp_options);
  }

  /**
   * Wrap an HTML fragment in the correct head/meta tags.
   *
   * This ensures that that UTF-8 is correctly detected.
   */
  static public function wrapHTML($html) {
    // We add surrounding <html> and <head> tags.
    $wrapped_html = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">';
    $wrapped_html .= '<html xmlns="http://www.w3.org/1999/xhtml"><head><meta http-equiv="Content-Type" content="text/html; charset=utf-8" /></head><body>';
    $wrapped_html .= $html;
    $wrapped_html .= '</body></html>';
    return $wrapped_html;
  }

  /**
   * Removes legacy elements from HTML that are no longer needed.
   */
  public static function stripOrFixLegacyElements($html) {
    // STRIP.
    // Strip comments.
    $query_path = HTMLCleanUp::initQueryPath($html);

    foreach ($query_path->top()->xpath('//comment()')->get() as $comment) {
      $comment->parentNode->removeChild($comment);
    }

    // Remove elements and their children.
    $query_path->find('img[src="/gif/sealdoj.gif"]')->parent('p')->remove();
    HTMLCleanUp::removeElements($query_path, array(
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
    HTMLCleanUp::removeWrapperElements($query_path, array(
      'body > blockquote',
      '.bdywrpr',
      '.gridwrpr',
      '.leftcol-subpage',
      '.leftcol-subpage-content',
      '.bodytextbox',
      'body > div',
    ));

    // Remove style attribute from elements.
    $query_path->find('.narrow-bar')->removeAttr('style');

    // Remove matching elements containing only &nbsp; or nothing.
    HTMLCleanUp::removeEmptyElements($query_path, array(
      'div',
      'span',
    ));

    // Remove black title bar with eagle image (if present).
    HTMLCleanUp::removeTitleBarImage($query_path);

    // FIX.
    // Empty anchors without name attribute will be stripped by ckEditor.
    HTMLCleanUp::fixNamedAnchors($query_path);

    // Some pages have images as subtitles. Turn those into html.
    HTMLCleanUp::changeSubTitleImagesForHtml($query_path);

    return $query_path->html();
  }

  /**
   * Removes elements matching CSS selectors.
   *
   * @param array $selectors
   *   An array of selectors to remove.
   */
  protected static function removeElements($query_path, array $selectors) {
    foreach ($selectors as $selector) {
      $query_path->find($selector)->remove();
    }
    return $query_path;
  }

  /**
   * Removes a wrapping element, leaving child elements intact.
   *
   * @param array $selectors
   *   An array of selectors for the wrapping element(s).
   */
  protected static function removeWrapperElements($query_path, array $selectors) {
    foreach ($selectors as $selector) {
      $children = $query_path->find($selector)->children();
      $children->unwrap();
    }
  }

  /**
   * Removes empty elements matching selectors.
   *
   * @param array $selectors
   *   An array of selectors to remove.
   */
  protected static function removeEmptyElements($query_path, array $selectors) {
    foreach ($selectors as $selector) {
      $elements = $query_path->find($selector);
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
   * Remove eagle image title bar divs.
   *
   * Eagle image bars are always inside '<div style="margin-bottom:(15|20)px">'.
   * It appears that they are the only elements with this style applied.
   * Nonetheless, if more than one match, remove only the first.
   */
  protected static function removeTitleBarImage($query_path) {
    // Find divs that are immediately followed by img tags.
    $elements = $query_path->find('div > img')->parent();
    foreach ($elements as $element) {
      // Eagle banner bars always preceed headlines.
      if (preg_match('/class=\"headline/', $element->html())) {
        break;
      }
      if (preg_match('/style=\"(margin|padding)-bottom:(\s)*(15|20)px/i', $element->html())) {
        // We found an eagle image title bar: remove it and we're done.
        $element->remove();
        break;
      }
    }
  }

  /**
   * Removes legacy usage of javascript:exitWinOpen() for external links.
   */
  protected static function removeExtLinkJS($query_path) {
    $elements = $query_path->find('a');

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
   * Empty anchors without name attribute will be stripped by ckEditor.
   */
  protected static function fixNamedAnchors($query_path) {
    $elements = $query_path->find('a');
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
  public static function convertRelativeSrcsToAbsolute($html, $file_id) {
    $query_path = HTMLCleanUp::initQueryPath($html);

    // A list of attributes to convert, keyed by HTML tag (NOT selector).
    $attributes = array(
      'img' => 'src',
      'a' => 'href',
    );

    $elements = $query_path->find(implode(', ', array_keys($attributes)));
    foreach ($elements as $element) {
      $attribute = $attributes[$element->tag()];

      $url = parse_url($element->attr($attribute));

      if ($url) {
        $is_relative = empty($url['scheme']) && !empty($url['path']) && substr($url['path'], 0, 1) !== '/';

        if ($is_relative) {
          $dir_path = dirname($file_id);
          $new_url = '/' . $dir_path . '/' . $url['path'];

          // We might get some double '//', let's clean them.
          $new_url = str_replace("//", "/", $new_url);

          $element->attr($attribute, $new_url);
        }
      }
    }
    return $query_path->html();
  }

  /**
   * Change sub-header images to <h2> html titles.
   */
  protected static function changeSubTitleImagesForHtml($query_path) {
    // Find all headline divs with an image inside.
    $elements = $query_path->find('div.headline > img')->parent();

    foreach ($elements as $element) {
      $image = $element->find('img');
      $alt = $image->attr('alt');
      $element->html("<h2>{$alt}</h2>");
    }
  }
}

class StringCleanUp {
  /**
   * Deal with encodings.
   */
  public static function fixEncoding($string) {
    // If the content is not UTF8, we assume it's WINDOWS-1252. This fixes
    // bogus character issues. Technically it could be ISO-8859-1 but it's safe
    // to convert this way.
    // http://en.wikipedia.org/wiki/Windows-1252
    $enc = mb_detect_encoding($string, 'UTF-8', TRUE);
    if (!$enc) {
      return mb_convert_encoding($string, 'UTF-8', 'WINDOWS-1252');
    }
    return $string;
  }

  /**
   * Map of unconventional chars to there some what equivalents.
   *
   * @return array
   *   An array with the mappings.
   */
  public static function funkyCharsMap() {
    $convert_table = array(
      '&amp;' => 'and',   '@' => 'at',    '©' => 'c', '®' => 'r', 'À' => 'a',
      'Á' => 'a', 'Â' => 'a', 'Ä' => 'a', 'Å' => 'a', 'Æ' => 'ae','Ç' => 'c',
      'È' => 'e', 'É' => 'e', 'Ë' => 'e', 'Ì' => 'i', 'Í' => 'i', 'Î' => 'i',
      'Ï' => 'i', 'Ò' => 'o', 'Ó' => 'o', 'Ô' => 'o', 'Õ' => 'o', 'Ö' => 'o',
      'Ø' => 'o', 'Ù' => 'u', 'Ú' => 'u', 'Û' => 'u', 'Ü' => 'u', 'Ý' => 'y',
      'ß' => 'ss','à' => 'a', 'á' => 'a', 'â' => 'a', 'ä' => 'a', 'å' => 'a',
      'æ' => 'ae','ç' => 'c', 'è' => 'e', 'é' => 'e', 'ê' => 'e', 'ë' => 'e',
      'ì' => 'i', 'í' => 'i', 'î' => 'i', 'ï' => 'i', 'ò' => 'o', 'ó' => 'o',
      'ô' => 'o', 'õ' => 'o', 'ö' => 'o', 'ø' => 'o', 'ù' => 'u', 'ú' => 'u',
      'û' => 'u', 'ü' => 'u', 'ý' => 'y', 'þ' => 'p', 'ÿ' => 'y', 'Ā' => 'a',
      'ā' => 'a', 'Ă' => 'a', 'ă' => 'a', 'Ą' => 'a', 'ą' => 'a', 'Ć' => 'c',
      'ć' => 'c', 'Ĉ' => 'c', 'ĉ' => 'c', 'Ċ' => 'c', 'ċ' => 'c', 'Č' => 'c',
      'č' => 'c', 'Ď' => 'd', 'ď' => 'd', 'Đ' => 'd', 'đ' => 'd', 'Ē' => 'e',
      'ē' => 'e', 'Ĕ' => 'e', 'ĕ' => 'e', 'Ė' => 'e', 'ė' => 'e', 'Ę' => 'e',
      'ę' => 'e', 'Ě' => 'e', 'ě' => 'e', 'Ĝ' => 'g', 'ĝ' => 'g', 'Ğ' => 'g',
      'ğ' => 'g', 'Ġ' => 'g', 'ġ' => 'g', 'Ģ' => 'g', 'ģ' => 'g', 'Ĥ' => 'h',
      'ĥ' => 'h', 'Ħ' => 'h', 'ħ' => 'h', 'Ĩ' => 'i', 'ĩ' => 'i', 'Ī' => 'i',
      'ī' => 'i', 'Ĭ' => 'i', 'ĭ' => 'i', 'Į' => 'i', 'į' => 'i', 'İ' => 'i',
      'ı' => 'i', 'Ĳ' => 'ij','ĳ' => 'ij','Ĵ' => 'j', 'ĵ' => 'j', 'Ķ' => 'k',
      'ķ' => 'k', 'ĸ' => 'k', 'Ĺ' => 'l', 'ĺ' => 'l', 'Ļ' => 'l', 'ļ' => 'l',
      'Ľ' => 'l', 'ľ' => 'l', 'Ŀ' => 'l', 'ŀ' => 'l', 'Ł' => 'l', 'ł' => 'l',
      'Ń' => 'n', 'ń' => 'n', 'Ņ' => 'n', 'ņ' => 'n', 'Ň' => 'n', 'ň' => 'n',
      'ŉ' => 'n', 'Ŋ' => 'n', 'ŋ' => 'n', 'ñ' => 'n', 'Ō' => 'o', 'ō' => 'o',
      'Ŏ' => 'o', 'ŏ' => 'o', 'Ő' => 'o', 'ő' => 'o', 'Œ' => 'oe','œ' => 'oe',
      'Ŕ' => 'r', 'ŕ' => 'r', 'Ŗ' => 'r', 'ŗ' => 'r', 'Ř' => 'r', 'ř' => 'r',
      'Ś' => 's', 'ś' => 's', 'Ŝ' => 's', 'ŝ' => 's', 'Ş' => 's', 'ş' => 's',
      'Š' => 's', 'š' => 's', 'Ţ' => 't', 'ţ' => 't', 'Ť' => 't', 'ť' => 't',
      'Ŧ' => 't', 'ŧ' => 't', 'Ũ' => 'u', 'ũ' => 'u', 'Ū' => 'u', 'ū' => 'u',
      'Ŭ' => 'u', 'ŭ' => 'u', 'Ů' => 'u', 'ů' => 'u', 'Ű' => 'u', 'ű' => 'u',
      'Ų' => 'u', 'ų' => 'u', 'Ŵ' => 'w', 'ŵ' => 'w', 'Ŷ' => 'y', 'ŷ' => 'y',
      'Ÿ' => 'y', 'Ź' => 'z', 'ź' => 'z', 'Ż' => 'z', 'ż' => 'z', 'Ž' => 'z',
      'ž' => 'z', 'ſ' => 'z', 'Ə' => 'e', 'ƒ' => 'f', 'Ơ' => 'o', 'ơ' => 'o',
      'Ư' => 'u', 'ư' => 'u', 'Ǎ' => 'a', 'ǎ' => 'a', 'Ǐ' => 'i', 'ǐ' => 'i',
      'Ǒ' => 'o', 'ǒ' => 'o', 'Ǔ' => 'u', 'ǔ' => 'u', 'Ǖ' => 'u', 'ǖ' => 'u',
      'Ǘ' => 'u', 'ǘ' => 'u', 'Ǚ' => 'u', 'ǚ' => 'u', 'Ǜ' => 'u', 'ǜ' => 'u',
      'Ǻ' => 'a', 'ǻ' => 'a', 'Ǽ' => 'ae','ǽ' => 'ae','Ǿ' => 'o', 'ǿ' => 'o',
      'ə' => 'e', 'Ё' => 'jo','Є' => 'e', 'І' => 'i', 'Ї' => 'i', 'А' => 'a',
      'Б' => 'b', 'В' => 'v', 'Г' => 'g', 'Д' => 'd', 'Е' => 'e', 'Ж' => 'zh',
      'З' => 'z', 'И' => 'i', 'Й' => 'j', 'К' => 'k', 'Л' => 'l', 'М' => 'm',
      'Н' => 'n', 'О' => 'o', 'П' => 'p', 'Р' => 'r', 'С' => 's', 'Т' => 't',
      'У' => 'u', 'Ф' => 'f', 'Х' => 'h', 'Ц' => 'c', 'Ч' => 'ch','Ш' => 'sh',
      'Щ' => 'sch', 'Ъ' => '-', 'Ы' => 'y', 'Ь' => '-', 'Э' => 'je','Ю' => 'ju',
      'Я' => 'ja', 'а' => 'a', 'б' => 'b', 'в' => 'v', 'г' => 'g', 'д' => 'd',
      'е' => 'e', 'ж' => 'zh','з' => 'z', 'и' => 'i', 'й' => 'j', 'к' => 'k',
      'л' => 'l', 'м' => 'm', 'н' => 'n', 'о' => 'o', 'п' => 'p', 'р' => 'r',
      'с' => 's', 'т' => 't', 'у' => 'u', 'ф' => 'f', 'х' => 'h', 'ц' => 'c',
      'ч' => 'ch', 'ш' => 'sh','щ' => 'sch','ъ' => '-','ы' => 'y', 'ь' => '-',
      'э' => 'je', 'ю' => 'ju','я' => 'ja','ё' => 'jo','є' => 'e', 'і' => 'i',
      'ї' => 'i', 'Ґ' => 'g', 'ґ' => 'g', 'א' => 'a', 'ב' => 'b', 'ג' => 'g',
      'ד' => 'd', 'ה' => 'h', 'ו' => 'v', 'ז' => 'z', 'ח' => 'h', 'ט' => 't',
      'י' => 'i', 'ך' => 'k', 'כ' => 'k', 'ל' => 'l', 'ם' => 'm', 'מ' => 'm',
      'ן' => 'n', 'נ' => 'n', 'ס' => 's', 'ע' => 'e', 'ף' => 'p', 'פ' => 'p',
      'ץ' => 'C', 'צ' => 'c', 'ק' => 'q', 'ר' => 'r', 'ש' => 'w', 'ת' => 't',
      '™' => 'tm',
    );

    return $convert_table;
  }

  /**
   * Take all things that are not digits or the alphabet and simplify it.
   *
   * This should get rid of most accents, and language specific chars.
   *
   * @param string $string
   *   A string.
   *
   * @return string
   *   The converted string.
   */
  public static function convertNonASCIItoASCII($string) {
    foreach (StringCleanUp::funkyCharsMap() as $weird => $normal) {
      $string = str_replace($weird, $normal, $string);
    }
    return $string;
  }

  /**
   * Trim string from various types of whitespace.
   *
   * @param string $string
   *   The text string from which to remove whitespace.
   *
   * @return string
   *   The trimmed string.
   */
  public static function superTrim($string) {
    // Remove unicode whitespace
    // @see http://stackoverflow.com/questions/4166896/trim-unicode-whitespace-in-php-5-2
    $string = preg_replace('/^[\pZ\pC]+|[\pZ\pC]+$/u', '', $string);
    return $string;
  }

  /**
   * Strip windows CR characters.
   */
  public static function stripWindowsCRChars($string) {
    // We need to strip the Windows CR characters, because otherwise we end up
    // with &#13; in the output.
    // http://technosophos.com/content/querypath-whats-13-end-every-line
    return str_replace(chr(13), '', $string);
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
  $undesirables = array("»", "Â");

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
