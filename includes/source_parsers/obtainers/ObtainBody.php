<?php

/**
 * @file
 * Class ObtainBody
 *
 * Contains a collection of stackable finders that can be arranged
 * as needed to obtain a body or other long html content.
 */

/**
 * {@inheritdoc}
 */
class ObtainBody extends ObtainHtml {

  // Properties declaration.


  /**
   * {@inheritdoc}
   */
  public function __construct($query_path, $target_stack) {
    if (!empty($target_stack) && !empty($query_path) && is_array($target_stack)) {
      $this->setTargetStack($target_stack);
      $this->queryPath = $query_path;

      $this->processMethodStack($query_path, $target_stack, 'ObtainBody');
    }

  }


  // **************** Begin finder target definitions *************************
  // To create a new finder, use this template and put them in alpha order.
  // @codingStandardsIgnoreStart
  /*
  protected function findMethod() {
    $this->setJustFound($this->queryPath->find("{SELECTOR}")->first());
    $text = $this->getJustFound()->text();
    return $text;
  }
  */
  // @codingStandardsIgnoreEnd


  /**
   * Finder method to find the top body.
   *
   * @return string
   *   The string that was found
   */
  protected function findTopBodyHtml() {
    $this->setJustFound($this->queryPath->top('body'));
    $string = $this->getJustFound()->innerHTML();
    // This is essentially everything. so lets not remove it.
    $this->removeMeNot();
    return $string;
  }


  /**
   * Finder method to find the body in .contentSub.
   *
   * @return string
   *   The string that was found
   */
  protected function findClassContentSub() {
    $this->setJustFound($this->queryPath->top('.contentSub'));
    $string = $this->getJustFound()->innerHTML();
    // This is essentially everything. so lets not remove it.
    $this->removeMeNot();
    return $string;
  }
}
