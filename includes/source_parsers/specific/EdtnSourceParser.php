<?php
/**
 * @file
 * Source parsers for the Eastern District of Tennessee.
 */

class EdtnPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    $images = HtmlCleanUp::matchAll($this->queryPath, "img", "Banner Image", "attr", "alt");
    foreach ($images as $img) {
      $img->remove();
    }

    $links = HtmlCleanUp::matchAll($this->queryPath, "a", "Printer Friendly", "attr", "title");
    foreach ($links as $link) {
      $link->remove();
    }
  }
}

class EdtnPressSourceParser extends NGDistrictPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();

    // Remove the first 2 h2.
    $counter = 0;
    foreach ($this->queryPath->find("h2") as $h2) {
      if ($counter == 0 || $counter == 1) {
        $h2->remove();
      }
      else {
        break;
      }
      $counter++;
    }

    $hr = $this->queryPath->find("div[align='center'] > hr")->first();
    $hr->remove();
  }
}
