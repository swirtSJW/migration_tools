<?php

/**
 * @file
 * Class EdpaSourceParser
 */

class EdpaSourceParser extends DistrictsSourceParser {

  /**
   * {@inheritdoc}
   */
  public function setBody() {
    $selectors = array(
      "a[href='#content']",
      "div#nav",
      "#searchbox",
      "p.credit",
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);

    $elem = HtmlCleanUp::matchText($this->queryPath, "h5", "USAO Home Page");
    if ($elem) {
      $elem->remove();
    }
    $elem = $this->queryPath->find("a[href='http://www.usa.gov']");
    if ($elem) {
      $elem->parent()->parent()->remove();
    }

    parent::setBody();
  }
}
