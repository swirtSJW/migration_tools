<?php
/**
 * @file
 * DistrictSourceParser.
 */

class NGDistrictPageSourceParser extends NGNodeSourceParser {

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    $subbanner = $this->getSubBanner();
    if ($subbanner) {
      $subbanner->remove();
    }

    // Remove <a href="#top">Return to Top</a>.
    $this->removeLinkReturnToTop();

    $this->removeTableBackgrounds();

    // Rewrap p.greyHeadline and div.greyHeadline to h2.
    $selectors_to_rewrap = array('p.greyHeadline', 'div.greyHeadline');
    $new_wrapper = '<h6 class="subheading" />';
    HtmlCleanup::rewrapElements($this->queryPath, $selectors_to_rewrap, $new_wrapper);
    // Remove breadcrumbs.
    $this->queryPath->find('.breadcrumb')->first()->remove();
  }

  /**
   * Get sub-banner.
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

  /**
   * Removes the background from tables in markup by adding class.
   */
  protected function removeTableBackgrounds() {
    $tables = $this->queryPath->find('table');
    foreach ($tables as $table) {
      $table->addClass('no-background');
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObatinersInfo() {
    parent::setDefaultObatinersInfo();

    $title = new ObtainerInfo("title");
    $title->addMethod('findH1First');
    $title->addMethod('findH1Any');
    $title->addMethod('findClassBreadcrumbLast');
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findSubBannerAlt');
    $this->addObtainerInfo($title);
  }
}
