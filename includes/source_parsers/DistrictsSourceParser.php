<?php
/**
 * @file
 * DistrictSourceParser.
 */

class DistrictsSourceParser extends SourceParser {

  /**
   * Get subbanner.
   */
  protected function getSubBanner() {
    $subbanner = NULL;
    $images = $this->queryPath->find('img');
    foreach ($images as $image) {
      $src = $image->attr('src');
      $src = strtoupper($src);
      if (substr_count($src, "SUBBANNER") > 0) {
        return $image;
      }
    }
  }


  /**
   * {@inheritdoc}
   */
  protected function setTitle($override = '') {
    if (empty($override)) {
      // h1 is top priority.
      $title = $this->queryPath->find("h1")->first()->text();
      $this->queryPath->find("h1")->first()->remove();
      // If no title, try to get it from the sub banner.
      $subbanner = $this->getSubBanner();
      if (empty($title) && $subbanner) {
        $title = $subbanner->attr('alt');
        $title = str_ireplace("banner", "", $title);
        // Check to see if alt is just placeholder to discard.
        if (stristr($title, 'placeholder')) {
          $title = '';
        }
      }
      if (empty($title)) {
        // Try the last item in the breadcrumb.
        $breadcrumb = $this->queryPath->find(".breadcrumb");
        // Remove the anchors.
        $breadcrumb->find(a)->remove();
        $title = trim($breadcrumb->first()->text());
      }
    }
    else {
      // The override was invoked, so use it.
      $title = $override;
    }

    // Pass it to the parent::setTitle to process string cleanup or trigger
    // a fallback for title sources.
    parent::setTitle($title);
  }


  /**
   * {@inheritdoc}
   */
  public function setBody() {
    $subbanner = $this->getSubBanner();
    if ($subbanner) {
      $subbanner->remove();
    }

    // Remove <a href="#top">Return to Top</a>.
    $this->removeLinkReturnToTop();
    // Rewrap p.greyHeadline and div.greyHeadline to h2.
    $selectors_to_rewrap = array('p.greyHeadline', 'div.greyHeadline');
    $new_wrapper = '<h2 class="subheading" />';
    HtmlCleanup::rewrapElements($this->queryPath, $selectors_to_rewrap, $new_wrapper);
    // Remove breadcrumbs.
    $this->queryPath->find('.breadcrumb')->first()->remove();

    parent::setBody();
  }


  /**
   * Remove 'return to top' link.
   */
  protected function removeLinkReturnToTop() {
    // Remove <a href="#top">Return to Top</a>.
    $items = $this->queryPath->find("a");
    foreach ($items as $item) {
      $text = $item->text();
      if (stristr($text, 'return to top')) {
        $item->remove();
        break;
      }
    }
  }

}
