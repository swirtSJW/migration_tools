<?php
/**
 * @file
 * OlaPageSourceParser.
 */

class OlaPageSourceParser extends SourceParser {

  /**
   * {@inheritdoc}
   */
  public function setBody() {
    $images = $this->queryPath->find('img');

    // If the first image says bar, is most likely one of the eagle image
    // banners.
    foreach ($images as $img) {
      if (substr_count($img->attr('src'), "-bar") > 0 || substr_count($img->attr('src'), "-banner") > 0) {
        $img->remove();
      }
      break;
    }
    parent::setBody();
  }
}
