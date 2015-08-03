<?php
/**
 * @file
 * Description of OlaWitnessSourceParser.
 */

class OlaWitnessSourceParser extends SourceParser {

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
   * Get the name.
   */
  public function getName() {
    $a = $this->queryPath->find("a");
    return $a->text();

  }

  /**
   * Get after anchor.
   *
   * Witness info is encapsulated in td tags. Inside the td there are multiple
   * sets of anchors and text after wards. This function helps get the text
   * that is placed after the anchor.
   */
  public function getAfterAnchor() {
    $html = $this->queryPath->html();

    // Get rid of the anchor.
    $pieces = explode("</a>", $html);
    $text = $pieces[1];
    // @codingStandardsIgnoreStart
    $text = str_replace('<br>', "", $text);
    // @codingStandardsIgnoreEnd
    $text = str_replace("</body></html>", "", $text);
    return $text;
  }

  /**
   * Get the position.
   */
  public function getPosition() {
    $aa = $this->getAfterAnchor();
    $pieces = explode(",", $aa);
    $position = trim($pieces[0]);
    return $position;
  }

  /**
   * Get the component.
   */
  public function getComponent() {
    $aa = $this->getAfterAnchor();
    $pieces = explode(",", $aa);
    $ignore = array_shift($pieces);
    $stuff = implode(",", $pieces);
    $stuff = preg_split('/[\r\n]+/', $stuff);
    $good = array();
    foreach ($stuff as $s) {
      $s = trim($s);
      if (!empty($s)) {
        $good[] = $s;
      }
    }
    $stuff = implode(" ", $good);
    $possible_components = explode(", ", $stuff);
    $components = array();
    foreach ($possible_components as $pc) {
      if (!empty($pc)) {
        $query = db_select("taxonomy_term_data", "t")->fields("t", array('name'))->condition('vid', 6, '=')
          ->condition('name', "%{$pc}%", "LIKE");

        $results = $query->execute();
        foreach ($results as $result) {
          $components[] = $result->name;
          break;
        }
      }
    }

    return (!empty($components)) ? $components[0] : "";
  }

  /**
   * Get the filename.
   */
  public function getFileName() {
    $a = $this->queryPath->find("a");
    $href = $a->attr('href');
    $pieces = explode("/", $href);
    $filename = array_pop($pieces);
    return $filename;
  }

  /**
   * Get the source directory.
   */
  public function getSourceDirectory() {
    $a = $this->queryPath->find("a");
    $href = $a->attr('href');
    $pieces = explode("/", $href);
    $name = array_pop($pieces);
    $dir = array_pop($pieces);
    $src_dir = variable_get("migration_tools_base_dir") . "/ola/testimony/{$dir}" . "/";
    return $src_dir;
  }

  /**
   * Get the destination directory.
   */
  public function getDestinationDirectory() {
    $filename = $this->getFileName();
    $pieces = explode("-", $filename);
    $destination_dir = "public:///testimonies/witnesses/attachments/{$pieces[0]}/{$pieces[1]}/{$pieces[2]}/";
    return $destination_dir;
  }
}
