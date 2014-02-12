<?php

/**
 * @file
 * Includes CareersSourceParser, which parses static HTML for careers section.
 */

/**
 * Class CareersSourceParser.
 *
 * @package doj_migration
 */
class CareersSourceParser extends SourceParser {

  /**
   * Returns and removes an inline title from the <body>.
   *
   * Matches only <p><u><strong>Label</strong></u> Text</p> or
   * <p><strong><u>Label</u></strong>: Text</p>.
   *
   * @param mixed $labels
   *   The label text for which to search, or a flat array of labels. The first
   *   label matched will be used.
   *
   * @return mixed
   *   FALSE if no match was found, otherwise, value of labeled <p>.
   */
  public function extractInlineTitle($labels) {

    if (is_string($labels)) {
      $labels = array($labels);
    }

    foreach ($labels as $label) {
      // Process body markup.
      $parent = $this->queryPath
        ->xpath("//strong[contains(text(), '$label')] | //u[contains(text(), '$label')]")
        ->parent('p');
      if ($parent) {
        $parent->remove('u');
        $parent->remove('strong');

        // Remove leading ':'. Double trim is intentional.
        $value = trim($parent->innerHTML());
        if (strpos($value, ':') === 0) {
          $value = substr($value, 1);
        }
        $value = trim($value);

        // Remove parent element from <body>.
        $parent->remove();

        return $value;
      }
    }

    return NULL;
  }
}
