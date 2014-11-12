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
      }
    }
    else {
      // The override was invoked, so use it.
      $title = $override;
    }

    if (empty($title)) {
      // This method came up empty so use the parent method as fallback.
      parent::setTitle();
    }
    else {
      $this->title = $title;
    }
    // Output to show progress to aid debugging.
    drush_print_r("{$this->fileId}  --->  {$this->title}");
  }


  /**
   * Sets a title retrieved using an array of selectors searched in order.
   *
   * The first selector to find something wins.
   *
   * @param array $selectors
   *   Querypath selectors to use in order for finding title text.
   * @param string $title_default
   *   The title to use if all selectors come up empty.
   *
   * @return string
   *   The current title text.
   */
  public function overrideSetTitle($selectors = array(), $title_default = '') {
    $title = '';
    foreach (is_array($selectors) ? $selectors : array() as $selector) {
      // If we have a title, no more searching.
      if (!empty($title)) {
        break;
      }
      $found_text = trim($this->queryPath->find($selector)->first()->text());
      if (!empty($found_text)) {
        $title = $found_text;
        $this->queryPath->find($selector)->first()->remove();
      }
      $title = (!empty($found_text)) ? $found_text : '';
    }
    $title = (empty($title)) ? $title_default : $title;
    // Set the found title only if we have no other.
    if (!empty($title)) {
      $this->setTitle($title);
    }
    return $this->getTitle();
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
