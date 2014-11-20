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
class ObtainTitle extends Obtainer {

  // Properties declaration.


  /**
   * {@inheritdoc}
   */
  public function __construct($query_path, $target_stack = array()) {
    parent::__construct($query_path, $target_stack);
    $this->processMethodStack($query_path, $target_stack, 'ObtainTitle');
  }


  // **************** Begin finder target definitions *************************
  // To create a new finder, use this template and put them in alpha order.
  // @codingStandardsIgnoreStart
  /*
  protected function findMethod() {
    $this->setJustFound($this->queryPath->find("{SELECTOR}")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }
  */
  // @codingStandardsIgnoreEnd



  /**
   * Finder method to find the content from the last item in the breadcrumb.
   * @return text
   *   The text found.
   */
  protected function findClassBreadcrumbLast() {
    $breadcrumb = $this->queryPath->find(".breadcrumb");
    // Remove the anchors. Creates a slight problem in that it is removing
    // elements it may not use, but this is mitigated by the fact that we
    // do not import breadcrumbs.
    $breadcrumb->find(a)->remove();
    $title = $breadcrumb->first()->text();
    $this->removeMeNot();
    return $title;
  }

  /**
   * Finder method to find the content from the last item in the breadcrumb.
   * @return text
   *   The text found.
   */
  protected function findClassBreadcrumbMenuContentLast() {
    $breadcrumb = $this->queryPath->find(".breadcrumbmenucontent")->first();
    // Remove the anchors. Creates a slight problem in that it is removing
    // elements it may not use, but this is mitigated by the fact that we
    // do not import breadcrumbs.
    $breadcrumb->children('a, span, font')->remove();;
    $title = $breadcrumb->text();
    $this->removeMeNot();
    return $title;
  }


  /**
   * Finder  first .contentSub > div > p[align='center'] > strong on the page.
   * @return text
   *   The text found.
   */
  protected function findClassContentSubDivPCenterStrong() {
    $this->setJustFound($this->queryPath->find(".contentSub > div > p[align='center'] > strong")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }


  /**
   * Finder  first .contentSub > div > strong on the page.
   * @return text
   *   The text found.
   */
  protected function findClassContentSubDivDivPStrong() {
    $this->setJustFound($this->queryPath->find(".contentSub > div > div > p > strong")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }


  /**
   * Find the first element  ".MsoNormal".  RISKY.
   * @return string
   *   The text found.
   */
  protected function findClassMsoNormal() {
    $this->setJustFound($this->queryPath->find(".MsoNormal")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }


  /**
   * Finder  first div.contentSub > div > div[align='center'] on the page.
   *
   * Added for AZ
   *
   * @return text
   *   The text found.
   */
  protected function findDivClassContentSubDivDivCenter() {
    $this->setJustFound($this->queryPath->find("div.contentSub > div > div[align='center']")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }


  /**
   * Finder method "div > div[align='center'] > div.Part > p" first on the page.
   *
   * Added for AZ
   *
   * @return text
   *   The text found.
   */
  protected function findDivDivCenterDivClassPartP() {
    $this->setJustFound($this->queryPath->find("div > div[align='center'] > div.Part > p")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }


  /**
   * Finder method to Loop through all h1 first H1 to evaluate gets it.
   * @return text
   *   The text found.
   */
  protected function findH1Any() {

    // Check all h1
    foreach ($this->queryPath->find("h1") as $key => $h1) {
      $this->setJustFound($h1);
      $text = $h1->text();
      $this->setPossibleText($text);
      $this->cleanPossibleText();
      if ($this->validatePossibleText()) {
        $this->setCurrentFindMethod("findAnyH1-i={$key}");
        // Return the original string to avoid double cleanup causing issues.
        return $text;
      }
    }
    // If it made it this far, nothing was found.
    return '';
  }

  /**
   * Finder method to find the content of the first H1 on the page.
   * @return text
   *   The text found.
   */
  protected function findH1First() {
    $this->setJustFound($this->queryPath->find("h1")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }


  /**
   * Finder method to find the content of the first H2 on the page.
   * @return text
   *   The text found.
   */
  protected function findH2First() {
    $this->setJustFound($this->queryPath->find("h2")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }


  /**
   * Finder method to find the content of the first H3 on the page.
   * @return text
   *   The text found.
   */
  protected function findH3First() {
    $this->setJustFound($this->queryPath->find("h3")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }


  /**
   * Finder method "#contentstart > div > h2" first on the page.
   * @return text
   *   The text found.
   */
  protected function findIdContentstartDivH2() {
    $this->setJustFound($this->queryPath->find("#contentstart > div > h2")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }


  /**
   * Find the elements with #contentstart > div > h2.
   * @return string
   *   The text found.
   */
  protected function findIdContentstartDivH2Sec() {
    $this->setJustFound($this->queryPath->find("#contentstart > div > h2"));
    foreach ($this->justFound as $key => $h2) {
      // Key starts at 0.
      if ($key == 1) {
        $text = $h2->text();
        return $text;
      }
    }
    return '';
  }


  /**
   * Finder method to find the content of the first #headline on the page.
   * @return text
   *   The text found.
   */
  protected function findIdHeadline() {
    $this->setJustFound($this->queryPath->find("#headline")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }


  /**
   * Finder method to find #Layer4 and the 2nd paragraph on the page.
   * @return text
   *   The text found.
   */
  protected function findIdLayer4P2() {
    $elems = $this->queryPath->find("#Layer4")->siblings();

    $pcounter = 0;
    // The sixth p is the title.
    foreach ($elems as $elem) {
      if ($elem->is("p")) {
        $pcounter++;
        if ($pcounter == 2) {
          $this->setJustFound($elem);
          $title = $elem->text();
        }
      }
    }
    return $title;
  }


  /**
   * Finder method to find #Layer4 and the 6th paragraph on the page.
   * @return text
   *   The text found.
   */
  protected function findIdLayer4P6() {
    $elems = $this->queryPath->find("#Layer4")->siblings();

    $pcounter = 0;
    // The sixth p is the title.
    foreach ($elems as $elem) {
      if ($elem->is("p")) {
        $pcounter++;
        if ($pcounter == 6) {
          $this->setJustFound($elem);
          $title = $elem->text();
          break;
        }
      }
    }
    return $title;
  }


  /**
   * Find  the content of the first  "p > strong > em" on the page.
   * @return text
   *   The text found.
   */
  protected function findPStrongEm() {
    $this->setJustFound($this->queryPath->find("p > strong > em")->first());
    $title = $this->getJustFound()->text();
    return $title;
  }


  /**
   * Finder method to find the content sub-banner alt.
   * @return text
   *   The text found.
   */
  protected function findSubBannerAlt() {
    return $this->grabSubBannerAttr('alt');
  }

  /**
   * Finder method to find the content sub-banner title.
   * @return text
   *   The text found.
   */
  protected function findSubBannerTitle() {
    return $this->grabSubBannerAttr('title');
  }


  /**
   * Finder method to find the content of the title.
   * @return text
   *   The text found.
   */
  protected function findTitleTag() {
    $this->setJustFound($this->queryPath->find("title"));
    $title = $this->getJustFound()->innerHTML();
    return $title;
  }


  // ***************** Helpers ***********************************************.

  /**
   * Cleans $possibleText or $override and puts it back.
   *
   * @param string $override
   *   Optional override text to clean and return if used publicly.
   *
   * @return string
   *   The cleaned text.
   */
  public function cleanPossibleText($override = '') {
    // Use the override text if it has been provided.
    $text = (!empty($override)) ? $override : $this->getPossibleText();
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
    if (strcmp($uppercase_version, $text) == 0) {
      // The entire thing is caps.
      $text = ucwords(strtolower($text));
    }

    // Remove undesirable chars.
    $text = str_replace('»', '', $text);

    // Can not be longer than 255 chars. Trimming must be done LAST!
    $text = $this->truncateThisWithoutHTML($text, 255, 2);

    $this->setPossibleText($text);

    // Return the $text in case this is being used publicly function.
    return $text;
  }

  /**
   * Grab method to find the content sub-banner attribute.
   * @return text
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
    $this->removeMeNot();
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
  }


  /**
   * Evaluates $possibleText and if it checks out, returns TRUE.
   *
   * @return bool
   *   TRUE if possibleText can be used as a title.  FALSE if it cant.
   */
  protected function validatePossibleText() {
    $text = $this->getPossibleText();
    // Run through any evaluations.  If it makes it to the end, it is good.
    // Case race, first to evaluate TRUE aborts the text.
    switch (TRUE) {
      // List any cases below that would cause it to fail validation.
      case empty($text):
      case is_object($text):
      case is_array($text);

        return FALSE;

      default:
        return TRUE;

    }
  }

}
