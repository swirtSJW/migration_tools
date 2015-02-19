<?php

/**
 * @file
 * Class ObtainBody
 *
 * Contains a collection of stackable finders that can be arranged
 * as needed to obtain a body or other long html content.
 */

/**
 * Class ObtainBody
 *
 * Obtains the HTML body.
 */
class ObtainBody extends ObtainHtml {

  /**
   * Finder method to find the top body.
   *
   * @return string
   *   The string that was found
   */
  protected function findTopBodyHtml() {
    $element = $this->queryPath->top('body');

    return $element->innerHtml();
  }

  /**
   * Finder method to find the body in .contentSub.
   *
   * @return string
   *   The string that was found
   */
  protected function findClassContentSub() {
    $element = $this->queryPath->top('.contentSub');

    return $element->innerHtml();
  }


  /**
   * Finder method to find the div contents in a table.
   *
   * @return string
   *   The string that was found
   */
  protected function findIdContent3TableTd() {
    $element = $this->queryPath->find('#content3 > table > tbody > tr > td')->first();
    return $element->innerHtml();
  }

}
