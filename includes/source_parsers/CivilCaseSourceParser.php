<?php
/**
 * @file
 * CivilCaseSourceParser.
 */

class CivilCaseSourceParser extends CivilPageSourceParser {

  /**
   * Replace the Title if necessary.
   *
   * @param string $css_selector
   *   A css selector.
   */
  private function replaceTitle($css_selector) {
    // Lets try to further clean up repetitive titles from cases.
    if ((strcmp($this->title, "Consumer Protection Branch") == 0) ||
      (strcmp($this->title, "Closed Cases") == 0) ||
      (strcmp($this->title, "Untitled Document") == 0) ||
      (strcmp($this->title, "Current Cases") == 0) ||
      (strcmp($this->title, "Cases") == 0)) {

      foreach ($this->queryPath->find($css_selector) as $t) {
        $title = StringCleanUp::superTrim($t->text());
        $title = str_replace(array("\n", "\r"), "", $title);
        $title = preg_replace('/\s+/', ' ', $title);
        if (strlen($title) <= 255) {
          $this->title = $title;
        }
        break;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function setTitle() {
    parent::setTitle();
    $this->replaceTitle('td > div > blockquote');
    $this->replaceTitle('td > div > p');
    $this->replaceTitle('td > div > h1');
    $this->replaceTitle('td > div > h6');
    $this->replaceTitle('div.left > p.center');
    $this->replaceTitle('td > p');
    $this->replaceTitle('td > div');

    // Every piece of content seems to have this, but it is wrong form most
    // pieces of content.
    $this->replaceTitle('td > blockquote > p');
  }
}
