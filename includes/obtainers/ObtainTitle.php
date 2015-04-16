<?php

/**
 * @file
 * Class ObtainTitle
 *
 * Contains a collection of stackable finders that can be arranged
 * as needed to obtain a title/heading and possible subtitle/subheading.
 */

/**
 * {@inheritdoc}
 */
class ObtainTitle extends ObtainHtml {

  /**
   * {@inheritdoc}
   */
  protected function processString($string) {
    return $this->truncateString($string);
  }

  /**
   * Truncates and sets the discarded if there is a remainder.
   */
  protected function truncateString($string) {
    $split = $this->truncateThisWithoutHTML($string, 255, 2);

    // @todo Add debugging to display $split['remaining'].
    // $this->setTextDiscarded($split['remaining']);

    return $split['truncated'];
  }

  /**
   * Find the content from the last anchor in the breadcrumb chain.
   *
   * @param string $selector
   *   Selector of the breadcrumb container.
   *
   * @return string
   *   The text found.
   */
  protected function findBreadcrumbLastAnchor($selector) {
    $title = '';
    if (!empty($selector)) {
      $breadcrumb = $this->queryPath->find($selector);
      $title = $breadcrumb->find(a)->last()->text();
      // This element makes up a bigger whole, so it is not set to be removed.
      $this->setCurrentFindMethod("findBreadcrumbLastAnchor($selector)");
    }
    return $title;
  }

  /**
   * Find the content from the last non-anchor in the breadcrumb chain.
   *
   * @param string $selector
   *   Selector of the breadcrumb container.
   *
   * @return string
   *   The text found.
   */
  protected function findBreadcrumbLastNonAnchor($selector) {
    $title = '';
    if (!empty($selector)) {
      $breadcrumb = $this->queryPath->find($selector);
      // Clone the breadcrumb so the next operations are non-destructive.
      $clone = clone $breadcrumb;
      $clone->find(a)->remove();
      $title = $clone->first()->text();
      // This element makes up a bigger whole, so it is not set to be removed.
      $this->setCurrentFindMethod("findBreadcrumbLastNonAnchor($selector)");
    }
    return $title;
  }

  /**
   * Finder method to find the content from the last item in the breadcrumb.
   *
   * @return string
   *   The text found.
   */
  protected function findClassBreadcrumbMenuContentLast() {
    $breadcrumb = $this->queryPath->find(".breadcrumbmenucontent")->first();
    // Clone the breadcrumb so the next operations are non-destructive.
    $clone = clone $breadcrumb;
    $clone->children('a, span, font')->remove();
    $title = $clone->text();
    // This element makes up a bigger whole, so it is not set to be removed.

    return $title;
  }


  /**
   * Finder method to find #Layer4 and the 5th paragraph on the page first line.
   *
   * @return string
   *   The text found.
   */
  protected function findIdLayer4P5Firstline() {
    $elem = $this->queryPath->find("#Layer4")->siblings('p:nth-of-type(5)');
    $title = $elem->innerHTML();
    $title = self::trimAtBr($title);
    // Since this is a substring we can not remove the entire element,
    // so we have to evaluate the title and if it checks out, then remove the
    // the text and put the rest back.
    $this->extractAndPutBack($title, $elem);

    return $title;
  }


  /**
   * Finder method to find #Layer4 and the 5th paragraph on the page if short.
   *
   * @return string
   *   The text found.
   */
  protected function pluckIdLayer4P5ShortEnough() {
    $elem = $this->queryPath->find("#Layer4")->siblings('p:nth-of-type(5)');
    $this->setElementToRemove($elem);
    $title = $elem->innerHTML();
    // If this value is fairly short, we can use the whole thing.
    $length = drupal_strlen($title);
    if ($length > 210) {
      // Too long, so return empty so it will move on to the next finder.
      $title = '';
    }

    return $title;
  }


  /**
   * Method to find #Layer4 and the 5th paragraph up to the first empty br.
   *
   * @return string
   *   The text found.
   */
  protected function findIdLayer4P5UptoEmptyBr() {
    $elem = $this->queryPath->find("#Layer4")->siblings('p:nth-of-type(5)');
    $title = $elem->innerHTML();
    $title = $this->trimAtBrBlank($title, $elem, 210);

    return $title;
  }

  /**
   * Finder method to find #Layer4 and the 6th paragraph on the page if short.
   *
   * @return string
   *   The text found.
   */
  protected function pluckIdLayer4P6ShortEnough() {
    $elem = $this->queryPath->find("#Layer4")->siblings('p:nth-of-type(6)');
    $this->setElementToRemove($elem);
    $title = $elem->innerHTML();
    // If this value is fairly short, we can use the whole thing.
    $length = drupal_strlen($title);
    if ($length > 210) {
      // Too long, so return empty so it will move on to the next finder.
      $title = '';
    }

    return $title;
  }

  /**
   * Finder method to find #Layer4 and the 7th paragraph on the page if short.
   *
   * @return string
   *   The text found.
   */
  protected function pluckIdLayer4P7ShortEnough() {
    $elem = $this->queryPath->find("#Layer4")->siblings('p:nth-of-type(7)');
    $this->setElementToRemove($elem);
    $title = $elem->innerHTML();
    // If this value is fairly short, we can use the whole thing.
    $length = drupal_strlen($title);
    if ($length > 210) {
      // Too long, so return empty so it will move on to the next finder.
      $title = '';
    }

    return $title;
  }


  /**
   * Finder method to find the content sub-banner alt.
   * @return string
   *   The text found.
   */
  protected function findSubBannerAlt() {
    return $this->grabSubBannerAttr('alt');
  }

  /**
   * Finder method to find the content sub-banner title.
   * @return string
   *   The text found.
   */
  protected function findSubBannerTitle() {
    return $this->grabSubBannerAttr('title');
  }

  /**
   * Find the first image in the very specific contentSub div, and get the alt.
   */
  protected function findFirstContentSubImageAlt() {
    $elem = $this->queryPath->find('.contentSub > div > img')->first();
    if ($elem) {
      return $elem->attr('alt');
    }
    return "";
  }


  /**
   * Find first centered aligned paragraph after the first hr.
   */
  protected function pluckFirstCenteredAlignPAfterHr() {
    $hr = $this->queryPath->find("hr")->first();

    if (!empty($hr)) {
      $elem = $hr->next();
      if ($elem->is('p') && ($elem->attr('align') == 'center'
        || $elem->attr('style') == "text-align:center;")) {
        $this->setElementToRemove($elem);
        $text = $elem->text();
        return $text;
      }
    }
    return "";
  }

  /**
   * Find any attribute of any selector.
   *
   * @param string $selector
   *   The selector to find.
   * @param string $attribute
   *   The attribute to find on the selector. Example: alt, title, etc.
   * @param string $depth
   *   (optional) The depth to find.
   *
   * @return string
   *   The text found.
   */
  protected function findSelectorAttribute($selector, $attribute, $depth = 1) {
    if (!empty($selector)) {
      $elements = $this->queryPath->find($selector);
      foreach ((is_object($elements)) ? $elements : array() as $i => $element) {
        $i++;
        if ($i == $depth) {
          $this->setCurrentFindMethod("findSelectorAttribute($selector, $attribute, " . $i . ')');
          return $element->attr("{$attribute}");
        }
      }
    }
  }

  // ***************** Helpers ***********************************************.

  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = strip_tags($text);
    // Titles can not have html entities.
    $text = html_entity_decode($text, ENT_COMPAT, 'UTF-8');

    // There are also numeric html special chars, let's change those.
    module_load_include('inc', 'doj_migration', 'includes/doj_migration');
    $text = doj_migration_html_entity_decode_numeric($text);

    // We want out titles to be only digits and ascii chars so we can produce
    // clean aliases.
    $text = StringCleanUp::convertNonASCIItoASCII($text);

    // Remove white space-like things from the ends and decodes html entities.
    $text = StringCleanUp::superTrim($text);
    // Remove multiple spaces.
    $text = preg_replace('!\s+!', ' ', $text);
    // Convert to ucwords If the entire thing is caps. Otherwise leave it alone
    // for preservation of acronyms.
    // Caveat: will obliterate acronyms if the entire title is caps.
    $uppercase_version = strtoupper($text);
    similar_text($uppercase_version, $text, $percent);
    if ($percent > 95.5) {
      // Nearly the entire thing is caps.
      $text = strtolower($text);
    }
    $text = StringCleanUp::makeWordsFirstCapital($text);

    // Remove undesirable chars.
    $text = str_replace('»', '', $text);

    return $text;
  }

  /**
   * Grab method to find the content sub-banner attribute.
   * @return string
   *   The text found.
   */
  protected function grabSubBannerAttr($attribute = 'alt') {
    $title = $this->grabSubBannerString($attribute);
    // Remove the text 'banner'.
    $title = str_ireplace('banner', '', $title);
    // Check to see if alt is just placeholder to discard.
    $placeholder_texts = array(
      'placeholder',
      'place-holder',
      'place_holder',
    );
    foreach ($placeholder_texts as $needle) {
      if (stristr($title, $needle)) {
        // Just placeholder text, so ignore this text.
        $title = '';
      }
    }

    return $title;
  }

  /**
   * Get subbanner image.
   */
  protected function grabSubBannerString($attribute = 'alt') {
    $subbanner = NULL;
    $images = $this->queryPath->find('img');
    foreach ($images as $image) {
      $src = $image->attr('src');
      if (stristr($src, 'subbanner')) {
        return $image->attr($attribute);
      }
    }
    return '';
  }
}
