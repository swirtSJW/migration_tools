<?php
/**
 * @file
 * ObtainTitlePressRelease.
 */

class ObtainTitlePressRelease extends ObtainTitle {

  /**
   * {@inheritdoc}
   */
  public function validatePossibleText() {
    $text = $this->getPossibleText();
    // If the text it grabbed was 'News And Press Releases' then try again.
    if (strcasecmp(trim($text), "News And Press Releases") == 0) {
      return FALSE;
    }
    // If the text it grabbed was 'FOR IMMEDIATE RELEASE' then try again.
    if (stristr($text, "for immediate release")) {
      return FALSE;
    }
    // Make sure we didn't grab a date.
    $possible_date = strtotime($text);
    if ($possible_date) {
      return FALSE;
    }

    // Made it this far.  Send it to the parent for further validations.
    return parent::validatePossibleText();
  }

  /**
   * Cleans $text and returns it.
   *
   * @param string $text
   *   Text to clean and return.
   *
   * @return string
   *   The cleaned text.
   */
  public static function cleanPossibleText($text = '') {
    // Pass it to the parent, then do any additional processing.
    $text = parent::cleanPossibleText($text);
    $text = StringCleanUp::makeWordsFirstCapital($text);

    return $text;
  }
}
