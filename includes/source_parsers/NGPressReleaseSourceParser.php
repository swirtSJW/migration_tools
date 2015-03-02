<?php
/**
 * @file
 * Includes NGPressReleaseSourceParser class.
 *
 * This class contains customization to parse press releases.
 */

/**
 * Class NGPressReleaseSourceParser.
 *
 * @package doj_migration
 */

abstract class NGPressReleaseSourceParser extends NGNodeSourceParser {
  protected $date;
  protected $subTitle;
  protected $prNumber;

  /**
   * Getter.
   */
  public function getDate() {
    $date_string = $this->getProperty('date');
    $this->sourceParserMessage("Raw Date: @date_string", array('@date_string' => $date_string), WATCHDOG_DEBUG, 2);

    if (empty($date_string)) {
      $date = '';
    }
    else {
      $date = date('n/d/Y', strtotime($date_string));
    }

    // Output to show progress to aid debugging.
    $this->sourceParserMessage("Formatted Date: @date", array('@date' => $date), WATCHDOG_DEBUG, 2);
    return $date;
  }

  /**
   * Getter.
   */
  public function getSubTitle() {
    // @todo set default obtainer methods stack.
    return "";
  }

  /**
   * Gets $this->prNumber property.
   */
  public function getPrNumber() {
    $pr_number = $this->getProperty('prNumber');
    $this->sourceParserMessage("Press Release Number: @pr_number", array('@pr_number' => $pr_number), WATCHDOG_DEBUG, 2);

    return $pr_number;
  }

  /**
   * Clean the html beforing pulling the body.
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    // If the first paragraph in the content div says archive, lets remove it.
    $elem = HtmlCleanUp::matchText($this->queryPath, ".contentSub > div > p", "Archives");
    if ($elem) {
      $elem->remove();
    }

    $elem = HtmlCleanUp::matchText($this->queryPath, "table", "FOR IMMEDIATE RELEASE");
    if ($elem) {
      $elem->remove();
    }

    // Build selectors to remove.
    $selectors = array(
      "#PRhead1",
      "#navWrap",
      "#Layer3",
      "#Layer4",
      "img",
      "h1",
      ".breadcrumb",
      ".newsLeft",
      "#widget",
      "#footer",
      "a[title='Printer Friendly']",
      "a[href='#top']",
      "a[href='http://www.justice.gov/usao/wvn']",
      "a[href='https://www.justice.gov/usao/wvn']",
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
    parent::setDefaultObtainersInfo();

    $title = new ObtainerInfo("title", 'ObtainTitlePressRelease');
    $title->addMethod('findH1Any');
    $title->addMethod('findIdContentstartDivH2Sec');
    $title->addMethod('findH2First');
    $title->addMethod('findClassContentSubDivPCenterStrong');
    $title->addMethod('findClassContentSubDivDivPStrong');
    $title->addMethod('findIdHeadline');
    $title->addMethod('findPStrongEm');
    $title->addMethod('findIdContentstartDivH2');
    $title->addMethod('findDivClassContentSubDivDivCenter');
    $this->addObtainerInfo($title);

    $date = new ObtainerInfo("date");
    $date->addMethod('findTableRow1Col2');
    $date->addMethod('findTableRow1Col1');
    $date->addMethod('findTable2Row2Col2');
    $date->addMethod('findPAlignCenter');
    $date->addMethod('findIdContentstartFirst');
    $date->addMethod('findClassNewsRight');
    $date->addMethod('findClassBottomLeftContent');
    $date->addMethod('findProbableDate');
    $this->addObtainerInfo($date);

    $pr_number = new ObtainerInfo('prNumber', "ObtainID");
    $pr_number->addMethod("findTable3y1x");
    $this->addObtainerInfo($pr_number);
  }
}
