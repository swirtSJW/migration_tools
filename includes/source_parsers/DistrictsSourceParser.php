<?php
/**
 * @file
 * DistrictSourceParser.
 */

class DistrictsSourceParser extends SourceParser {

  /**
   * Constructor.
   *
   * @param string $file_id
   *   The file id, e.g. careers/legal/pm7205.html
   * @param string $html
   *   The full HTML data as loaded from the file.
   * @param bool $fragment
   *   Set to TRUE if there are no <html>,<head>, or <body> tags in the HTML.
   * @param array $qp_options
   *   An associative array of options to be passed to the html_qp() function.
   * @param array $arguments
   *   An associative array arguments passed up from the migration class.
   */
  public function __construct($file_id, $html, $fragment = FALSE, $qp_options = array(), $arguments = array()) {
    // Set default arguments.
    $arguments['header_element'] = 'h6';

    parent::__construct($file_id, $html, $fragment, $qp_options, $arguments);
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
      $om = $this->getObtainerMethods('title');
      $title_stack = (!empty($om)) ? $om : $default_target_stack;
      $this->setObtainerMethods(array('title' => $title_stack));
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
  public function setBody($override = '') {
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

    parent::setBody($override);
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
}
