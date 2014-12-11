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


  /**
   * Finder method ".some class > u" first on the page.
   *
   * This is created custom for usao-sdwv.
   *
   * @return string
   *   The text found.
   */
  protected function findClassMultiU() {
    $classes = array(
      '.style27',
      '.style17',
      '.style15',
      '.style28',
      '.style8',
      '.style5',
      '.style12',
      '.style26',
      '.style7',
      '.style10',
      '.style14',
      '.style22',
      '.style25',
      '.style13',
      '.style20',
      '.style23',
      '.style24',
      '.style6',
      '.style19',
      '.style21',
      '.style1',
      '.style16',
      '.style2',
      '.style3',
      '.style4',
      '.style9',
      '.style11',
    );
    foreach ($classes as $class) {
      $this->setJustFound($this->queryPath->find("$class > u")->first());
      $text = $this->getJustFound()->text();
      $this->setPossibleText($text);
      $this->cleanThisPossibleText();
      if ($this->validatePossibleText()) {
        $this->setCurrentFindMethod("findClassMultiU-class={$class}");
        // Return the original string to avoid double cleanup causing issues.
        return $text;
      }
    }

    return '';
  }

  /**
   * Finder method ".some class > strong >u" first on the page.
   *
   * This is created custom for usao-sdwv.
   *
   * @return string
   *   The text found.
   */
  protected function findClassMultiStrongU() {
    $classes = array(
      '.style28',
      '.style12',
      '.style29',
      '.style14',
      '.style4',
      '.style17',
    );
    foreach ($classes as $class) {
      $this->setJustFound($this->queryPath->find("$class > strong > u")->first());
      $text = $this->getJustFound()->text();
      $this->setPossibleText($text);
      $this->cleanThisPossibleText();
      if ($this->validatePossibleText()) {
        $this->setCurrentFindMethod("findClassMultiU-class={$class}");
        // Return the original string to avoid double cleanup causing issues.
        return $text;
      }
    }

    return '';
  }


}
