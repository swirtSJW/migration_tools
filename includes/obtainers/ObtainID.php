<?php

/**
 * @file
 * Class ObtainID
 */

/**
 * {@inheritdoc}
 */
class ObtainID extends ObtainHtml {

  /**
   * Method for returning the table cell at 3rd row, 1st column.
   *
   * @return text
   *   The string found.
   */
  protected function findTable3y1x() {
    $table = $this->queryPath->find("table");
    $text = $this->getFromTable($table, 3, 1);

    return $text;
  }
}
