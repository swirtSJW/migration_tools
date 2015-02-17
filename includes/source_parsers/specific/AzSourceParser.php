<?php
/**
 * @file
 * Az related source parsers.
 */

class AzPressSourceParser extends DistrictPressReleaseSourceParser {

  /**
   * {@inheritdoc}
   */
  public function setBody() {
    // Clean up headers.
    foreach ($this->queryPath->find("div.Part")->siblings() as $div) {
      $text = $div->text();
      $class = $div->attr('class');

      if ($class != "Part") {
        $strings = array(
          "WWW.JUSTICE.GOV/USAO/AZ",
          "Telephone",
          "Public Affairs",
        );
        $match = TRUE;
        foreach ($strings as $string) {
          if (substr_count($text, $string) <= 0) {
            $match = FALSE;
            break;
          }
        }

        if ($match) {
          $div->remove();
        }
      }
    }

    // Remove the div containing the empty anchor with id=top.
    $elements = $this->queryPath->find("div > p > a#top");
    $a = $elements->first();
    if (isset($a)) {
      $div = $a->parent()->parent();
      $div->remove();
    }

    parent::setBody();
  }
}
