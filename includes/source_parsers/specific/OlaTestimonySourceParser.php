<?php
/**
 * @file
 * Description of OlaTestimonySourceParser.
 */

class OlaTestimonySourceParser extends SourceParser {

  /**
   * Get a td from a tr.
   *
   * @param int $position
   *   which td do you want.
   *
   * @return string
   *   the text inside the td.
   */
  private function getTD($position) {
    // The frist td has the date.
    $counter = 0;
    foreach ($this->queryPath->find('td') as $td) {
      if ($counter == $position) {
        return $td->text();
      }
      $counter++;
    }
  }

  /**
   * Get document title.
   *
   * @return string
   *   the title.
   */
  public function getDocumentTitle() {
    return substr($this->getTD(1), 0, 255);
  }

  /**
   * Get the document date.
   *
   * @return string
   *   the date.
   */
  public function getDocumentDate() {
    $date = $this->getTD(0);
    $pieces = explode("-", $date);
    $final = array();
    foreach ($pieces as $piece) {
      $p = StringCleanUp::superTrim($piece);
      $p = substr($p, 0, 2);
      if (!empty($p)) {
        $final[] = $p;
      }
    }
    $final_date = "20{$final[2]}-{$final[0]}-{$final[1]}";
    return $final_date;
  }

  /**
   * Get testimony commitee.
   *
   * @return string
   *   the commitee.
   */
  public function getTestimonyCommittee() {
    return $this->getTD(2);
  }
}
