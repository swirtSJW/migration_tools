<?php
/**
 * @file
 * MenuGeneratorEngineCsv methods for converting a csv to an menu import file.
 */

/**
 * Class MenuGeneratorEngineCsv
 *
 * Parses a CSV file and coverts it to a menu import file for use by the
 * menu-import module.  Correctly connecting legacy links to migrated URI's
 */
class MenuGeneratorEngineCsv extends MenuGeneratorEngine {
  /**
   * Constructor.
   */
  public function __construct(MenuGenerationParametersCsvParse $parameters) {
    $this->parameters = $parameters;
  }


  /**
   * {@inheritdoc}
   */
  public function generate() {
    $this->parseCSVFile();

    return $this->menuFileContents;
  }

  /**
   * Parses a CSV file to extract each line as a menu item.
   */
  private function parseCSVFile() {
    $file = $this->parameters->getSourceUri();
    drush_print_r("Parsing CSV file: $file");
    $lines = array_map('str_getcsv', file($file));
    drush_print("Creating menu import with " . count($lines) . " items.");
    foreach ($lines as $line) {
      $this->menuFileContents .= $this->parseCSVLine($line);
    }
  }

  /**
   * Parses a line of csv and creates a line in the menu import file.
   *
   * @param string $line
   *   The line from the csv file in the form of
   *   "-, Menu Item Title, http://mysite.com"
   *
   * @return string
   *   The line for a menu item in a menu_import format.
   *
   * @throws MigrateException
   *   In the event there is a malformed line
   */
  private function parseCSVLine($line) {
    if (count($line) > 3) {
      // Data is likey malformed, throw an exception.
      $message = "Error with CSV file: more than three items detected per line.  Each line should only contain 'depth, title, link'. \n";
      throw new MigrateException($message);
    }
    $depth = (!empty($line[0])) ? $line[0] : '';
    $title = (!empty($line[1])) ? $line[1] : '';
    $link = (!empty($line[2])) ? $line[2] : '';
    if (empty($link)) {
      // This menu item is a non-link.
      // Non-links should look like '-My Title'.
      $menu_item = "{$depth}{$title}\n";
    }
    else {
      // Links should be like '-My Title {"url":"https://www.site.com/blah"}'.
      $url = $this->normalizeUri($link);
      $menu_item = "{$depth}{$title} {\"url\":\"{$url}\"}\n";
    }

    return $menu_item;
  }
}
