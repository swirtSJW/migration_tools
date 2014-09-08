<?php
/**
 * @file
 * Helper function to clean up html.
 */

class HtmlCleanUp {

  /**
   * Removes legacy elements from HTML that are no longer needed.
   */
  public static function stripOrFixLegacyElements($query_path) {
    // STRIP.
    // Strip comments.
    // @TODO make sure $query_path is a $query_path object.
    foreach ($query_path->top()->xpath('//comment()')->get() as $comment) {
      $comment->parentNode->removeChild($comment);
    }

    // Removing doj seal.
    $query_path->find('img[src="/gif/sealdoj.gif"]')->remove();

    // Remove elements and their children.
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
      'div.leftcolmenu',
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

  }

  /**
   * Removes elements matching CSS selectors.
   *
   * @param object $query_path
   *   A query path object.
   * @param array $selectors
   *   An array of selectors to remove.
   */
  protected static function removeElements($query_path, array $selectors) {
    foreach ($selectors as $selector) {
      $query_path->find($selector)->remove();
    }
  }

  /**
   * Removes elements matching CSS selectors from html.
   *
   * @param string $html
   *   Html to get processed.
   * @param array $selectors
   *   An array of selectors to remove.
   *
   * @return string
   *   Processed html.
   */
  public static function removeElementsFromHtml($html = '', array $selectors = array()) {
    // Put the shell on the html to extract with more certainty later.
    $html = '<div class="throw-away-parser-shell">' . $html . '</div>';
    $query_path = htmlqp($html, NULL, array());
    HTMLCleanUp::removeElements($query_path, $selectors);

    // Grab the html from the shell.
    $processed_html = $query_path->top('.throw-away-parser-shell')->innerHTML();
    return $processed_html;
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
  public static function convertRelativeSrcsToAbsolute($query_path, $file_id) {

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
