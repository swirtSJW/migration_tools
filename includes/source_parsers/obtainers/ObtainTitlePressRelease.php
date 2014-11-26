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

    // Made it this far.  Send it to the parent for further validations.
    return parent::validatePossibleText();
  }
}
