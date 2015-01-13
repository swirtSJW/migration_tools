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


  // ***************** Helpers ***********************************************

  /**
   * Cleans $text and returns it.
   *
   * @param string $text
   *   Text to clean and return.
   *
   * @return string
   *   The cleaned text.
   */
  public static function cleanString($text) {
    // There are also numeric html special chars, let's change those.
    module_load_include('inc', 'doj_migration', 'includes/doj_migration');
    $text = doj_migration_html_entity_decode_numeric($text);

    // Checking again in case another process rendered it non UTF-8.
    $is_utf8 = mb_check_encoding($text, 'UTF-8');

    if (!$is_utf8) {
      $text = StringCleanUp::fixEncoding($text);
    }

    // Remove specific strings.
    // Strings to remove must be sorted by complexity.  More complex must come
    // before smaller or less complex things.
    $strings_to_remove = array(
      'updated:',
      'updated',
    );
    foreach ($strings_to_remove as $string_to_remove) {
      $text = str_ireplace($string_to_remove, '', $text);
    }

    // Remove white space-like things from the ends and decodes html entities.
    $text = StringCleanUp::superTrim($text);

    return $text;
  }
}
