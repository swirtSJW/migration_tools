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
   * Getter $this->subtitle property.
   */
  public function getSubTitle() {
    $subtitle = $this->getProperty('subtitle');

    return $subtitle;
  }

  /**
   * Gets $this->prNumber property.
   */
  public function getPrNumber() {
    $pr_number = $this->getProperty('prNumber');

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
    $title->addMethod('findSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('findSelector', array("h2", 1));
    $title->addMethod('findSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('findSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('findSelector', array("#headline", 1));
    $title->addMethod('findSelector', array("p > strong > em", 1));
    $title->addMethod('findSelector', array("#contentstart > div > h2", 1));
    $this->addObtainerInfo($title);

    $subtitle = new ObtainerInfo('subtitle', "ObtainSubTitle");
    // Intentionally has no methods defined so it will not run unless a
    // migration specifies methods.
    $this->addObtainerInfo($subtitle);

    $date = new ObtainerInfo("date");
    $date->addMethod('findTableRow1Col2');
    $date->addMethod('findTableRow1Col1');
    $date->addMethod('findTable2Row2Col2');
    $date->addMethod('findSelector', array("p[align='center']", 1));
    $date->addMethod('findSelector', array("#contentstart > p", 1));
    $date->addMethod('findSelector', array(".newsRight > p", 1));
    $date->addMethod('findSelector', array(".newsRight", 1));
    $date->addMethod('findSelector', array(".BottomLeftContent", 1));
    $date->addMethod('findProbableDate');
    $this->addObtainerInfo($date);

    $pr_number = new ObtainerInfo('prNumber', "ObtainID");
    $pr_number->addMethod("findTable3y1x");
    $this->addObtainerInfo($pr_number);
  }
}
