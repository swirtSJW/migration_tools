<?php

/**
 * @file
 * Defines migration classes for Tax Division.
 */

/**
 * Migrates .html files from /tax to page nodes.
 *
 * @package doj_migration
 * @subpackage tax
 */
class TaxPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'tax',
      1 => 'tax/DiverseCommittee',
      2 => 'tax/hr_new_employee',
      3 => 'tax/readingroom/2001ctm',
      4 => 'tax/readingroom/2008ctm',
      5 => 'tax/readingroom/foia',
      6 => 'tax/readingroom/foia/criminalorders/2013',
      7 => 'tax/readingroom/foia/criminalorders/2014',
      8 => 'tax/readingroom/foia/criminalorders/2015',
      9 => 'tax/readingroom/foia/foia_publications',
      10 => 'tax/readingroom/JCM2004',
      11 => 'tax/readingroom/jcmanhtm',
      12 => 'tax/readingroom/jcpdf',
      13 => 'tax/readingroom/settlemn',
      14 => 'tax/readingroom/settlemn_OLD_01',
      15 => 'tax/readingroom/settlpdf',
      16 => 'tax/usaopress',
      17 => 'tax/componentpress',
    );

    $options = array('recurse' => FALSE);

    $arguments['source_parser_class'] = "TaxPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "TaxObtainTitlePage");
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findSelector', array("center > font[size='+2'] > b"));
    $title->addMethod('findSelector', array("title", 1));
    $title->addMethod('findSelector', array("h1", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Tax Division.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('tax')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array();
    if (parent::prepareRow($row) === FALSE || doj_migration_skip_file($row->fileid, $skip_these)) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /tax.
 *
 * @package doj_migration
 * @subpackage tax
 */
class TaxFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the tax subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'tax',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///tax/docs';

    $options = array('recurse' => TRUE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "tax";
  }
}

/**
 * Migrates press releases from /tax.
 *
 * @package doj_migration
 * @subpackage tax
 */
class TaxPressReleaseMigration extends NGJusticeHtmlToPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['component'] = "Tax Division";
    $arguments['source_parser_class'] = "TaxPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "TaxObtainTitlePressRelease");
    $title->addMethod('findSelector', array('.title', 1));
    $title->addMethod('findH1Any');
    $title->addMethod('findSelector', array("p[align='center'] > strong > u"));
    $title->addMethod('findSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('findSelector', array("h2", 1));
    $title->addMethod('findSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('findSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('findSelector', array("#headline", 1));
    $title->addMethod('findSelector', array("p > strong > em", 1));
    $title->addMethod('findSelector', array("#contentstart > div > h2", 1));
    $title->addMethod('findPSecond');

    $subtitle = new ObtainerInfo('subtitle', "TaxObtainTitlePressRelease");
    $subtitle->addMethod('findSelector', array('.title'));
    $subtitle->addMethod('findSelector', array("p[align='center'] > strong > em"));

    $date = new ObtainerInfo('date', "TaxObtainDate");
    $date->addMethod('findTableRow1Col2');
    $date->addMethod('findTableRow1Col1');
    $date->addMethod('findTable2Row2Col2');
    $date->addMethod('findSelector', array("p[align='center']", 1));
    $date->addMethod('findSelector', array("#contentstart > p", 1));
    $date->addMethod('findSelector', array(".newsRight > p", 1));
    $date->addMethod('findSelector', array(".BottomLeftContent", 1));
    $date->addMethod('findProbableDate');
    $date->addMethod('findTableRowLines');

    $type = new ObtainerInfo('content_type', "ObtainContentType");
    $type->addMethod('findPRImmediateRelease');

    $pr_number = new ObtainerInfo('prNumber', "TaxObtainPrNumber");
    $pr_number->addMethod('findMetaPr');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $subtitle;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;
    $arguments['obtainers_info'][] = $type;

    $source_dirs = array(
      1 => 'tax/2009',
      2 => 'tax/2010',
      3 => 'tax/2011',
      4 => 'tax/2012',
      5 => 'tax/2013',
      6 => 'tax/2014',
      7 => 'tax/2015',
      19 => 'tax/usaopress/2009',
      20 => 'tax/usaopress/2010',
      21 => 'tax/usaopress/2011',
      22 => 'tax/usaopress/2012',
      23 => 'tax/usaopress/2013',
      24 => 'tax/usaopress/2014',
      25 => 'tax/usaopress/2015',
    );

    parent::__construct($arguments, $source_dirs);

    $this->addFieldMapping('og_group_ref')
    ->defaultValue('tax')
    ->sourceMigration('Organization');
  }
}

/**
 * SourceParser for /tax pages.
 *
 * @package doj_migration
 * @subpackage tax
 */
class TaxPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    $selectors = array(
      'center > form',
      'pre > center',
      'hr',
      'center > a',
    );
  }
}

/**
 * SourceParser for /tax press releases.
 *
 * @package doj_migration
 * @subpackage tax
 */
class TaxPressSourceParser extends NGPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();

    $this->queryPath->find('table')->first()->remove();
    // Press release numbers are often unwrapped text, so search for the string.
    if (!empty($this->prNumber)) {
      $element = HtmlCleanUp::matchText($this->queryPath, 'p', $this->prNumber);
      if (is_object($element)) {
        $element->remove();
      }
    }
  }
}

/**
 * Obtainer for title property for /tax pages.
 *
 * @package doj_migration
 * @subpackage tax
 * @see Obtainer.api.php
 */
class TaxObtainTitlePage extends ObtainTitle {
  /**
   * Finder  first center font size2+ b on the page.
   *
   * @return string
   *   The text found.
   */
  protected function findCenterB() {
    $element = $this->queryPath->find("center > font[size='+2'] > b")->first();
    $this->setElementToRemove($element);

    return $element->text();
  }
}

/**
 * Obtainer for title property for /tax press releases.
 *
 * @package doj_migration
 * @subpackage tax
 * @see Obtainer.api.php
 */
class TaxObtainTitlePressRelease extends ObtainTitlePressRelease {
  /**
   * Finder method to find the content of the second p on the page.
   * @return string
   *   The text found.
   */
  protected function findPSecond() {
    $elements = $this->queryPath->find("p");
    foreach ($elements as $key => $elem) {
      if ($key == 1) {
        $this->setElementToRemove($elem);

        return $elem->text();
      }
    }
  }
}

/**
 * Obtainer for prNumber property for /tax press releases.
 *
 * @package doj_migration
 * @subpackage tax
 * @see Obtainer.api.php
 */
class TaxObtainPrNumber extends ObtainId {

  /**
   * Grabs a PR number from a the rnumber meta.
   *
   * @return string
   *   The text found.
   */
  protected function findMetaPr() {
    $element = $this->queryPath->find('meta[name="rnumber"]');
    $this->setElementToRemove($element);
    $text = $element->attr("content");

    return $text;
  }
}

/**
 * Custom obtainer for date property for /tax.
 *
 * @package doj_migration
 * @subpackage tax
 * @see Obtainer.api.php
 */
class TaxObtainDate extends ObtainDate {
  /**
   * Finds specific table entry and parses the lines.
   *
   * @return string
   *   Text for evaluation.
   */
  protected function findTableRowLines() {
    $element = $this->queryPath->find("td[align='left'] > b")->first();
    $lines = SELF::splitOnNewline($element->text());
    foreach ($lines as $line) {
      $line = $this->cleanString($line);
      $valid = $this->validateString($line);
      if ($valid) {
        $this->setElementToRemove($element);
        return $line;
      }
    }

    return '';
  }

}
