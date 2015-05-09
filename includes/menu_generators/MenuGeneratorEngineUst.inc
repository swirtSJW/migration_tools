<?php

/**
 * @file
 * Contains menu generator engine for UST Regions.
 */

/**
 * Class MenuGeneratorEngineUst.
 */
class MenuGeneratorEngineUst extends MenuGeneratorEngineDistrict {

  /**
   * {@inheritdoc}
   */
  protected function cleanQueryPathHtml() {
    $query_path = $this->getQueryPath();
    // Remove redundant div wrappers from tier subsections specific to UST.
    $tiers = $query_path->find('.leftnav ul > div');
    foreach ($tiers as $tier) {
      $subsection_divs = $tier->find('ul')->find('div');
      foreach ($subsection_divs as $subsection_div) {
        if (is_object($subsection_div)) {
          // Unwrap acts on parent, so you must jump to a child of the div.
          $subsection_div->firstChild()->unwrap();
        }
      }
    }
  }


  /**
   * {@inheritdoc}
   */
  public function getLinkstoPrepend() {
    $prepend = array();
    $title = "U.S. Trustee Program";
    $uri = "ust";
    $prepend['0.0'] = new MenuGeneratorElement($uri, $title, '', 0.0);
    $region = $this->parameters->getOrganization();
    $number = intval(preg_replace("/[^0-9]/", "", $region));
    $title = "Region $number";
    $uri = $region;
    $prepend['0.1'] = new MenuGeneratorElement($uri, $title, '', 0.1);

    return $prepend;
  }
}
