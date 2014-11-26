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
      // Default stack: Use this if none was defined in migration class.
      $default_target_stack = array(
        'findH1First',
        'findH1Any',
        'findClassBreadcrumbLast',
        'findClassBreadcrumbMenuContentLast',
        'findSubBannerAlt',

      );
      $title_stack = (!empty($this->getObtainerMethods('title'))) ? $this->getObtainerMethods('title') : $default_target_stack;
      $this->setObtainerMethods(array('title' => $title_stack));
    }
    else {
      // The override was invoked, so use it.
      $title = $override;
      $title = ObtainTitle::cleanPossibleText($title);
    }

    // Pass it to the parent::setTitle to process string cleanup or trigger
    // a fallback for title sources.
    parent::setTitle($title);
  }


  /**
   * {@inheritdoc}
   */
  public function setBody($override = '') {
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

    parent::setBody($override = '');
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
