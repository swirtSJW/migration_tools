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
    parent::setBody();
  }
}
