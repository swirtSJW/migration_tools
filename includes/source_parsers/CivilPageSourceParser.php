<?php
/**
 * @file
 * CivilPageSourceParser.
 */

class CivilPageSourceParser extends SourceParser {

  /**
   * {@inheritdoc}
   */
  public function setTitle() {
    parent::setTitle();

    // If we do not have a title, use the h1 tag.
    if (empty($this->title)) {
      $items = $this->queryPath->find("h1");
      foreach ($items as $item) {
        $this->title = $item->text();
        $item->remove();
        break;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function setBody() {
    // Remove the shortline div, and the section header.
    HtmlCleanUp::removeElements($this->queryPath, array('#shortline', ".section_header"));

    // Remove the menu ul.
    $items = $this->queryPath->find("ul>li>a");
    foreach ($items as $item) {
      $text = $item->text();
      if (substr_count($text, "Civil Division Home") > 0) {
        $item->parent('ul')->remove();
        break;
      }
    }

    parent::setBody();
  }

}
