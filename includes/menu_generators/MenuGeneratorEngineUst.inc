<?php

/**
 * @file
 * Contains menu generator engine for USAO districts.
 */

/**
 * Class MenuGeneratorEngineUst.
 */
class MenuGeneratorEngineUst extends MenuGeneratorEngineDistrict {

  /**
   * {@inheritdoc}
   */
  protected function getQueryPath() {
    $query_path = parent::getQueryPath();

    $tiers = $query_path->find('.leftnav ul > div');
    foreach ($tiers as $tier) {
      $previous_li = $tier->prev('li');
      $tier->appendTo($previous_li);
    }

    return $this->queryPath;
  }
}
