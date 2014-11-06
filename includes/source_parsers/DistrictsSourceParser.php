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
  protected function setTitle() {
    $subbanner = $this->getSubBanner();
    if ($subbanner) {
      $title = $subbanner->attr('alt');
      $this->title = str_ireplace("banner", "", $title);
    }

    if ($this->title == "Placeholder  Image") {
      $this->title = $this->queryPath->find("h1")->text();
    }

    if (empty($this->title)) {
      parent::setTitle();
    }
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
    // Rewrap p.greyHeadline to h2.
    $selectors_to_rewrap = array('p.greyHeadline');
    $new_wrapper = '<h2 class="subheading" />';
    HtmlCleanup::rewrapElements($this->queryPath, $selectors_to_rewrap, $new_wrapper);
    // Remove breadcrumbs.
    $this->queryPath->find('.breadcrumb')->remove();

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
