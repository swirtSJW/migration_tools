<?php

/**
 * @file
 * Defines migration classes for U.S. Trustee Program Regions and Offices.
 */

/**
 * Migrates .html files from /ust-regions to page nodes.
 *
 * @package doj_migration
 * @subpackage ust-regions
 */
class UstRegionsPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      1 => 'ust/r01',
      2 => 'ust/r02',
      3 => 'ust/r03',
      4 => 'ust/r04',
      5 => 'ust/r05',
      6 => 'ust/r06',
      7 => 'ust/r07',
      8 => 'ust/r08',
      9 => 'ust/r09',
      10 => 'ust/r10',
      11 => 'ust/r11',
      12 => 'ust/r12',
      13 => 'ust/r13',
      14 => 'ust/r14',
      15 => 'ust/r15',
      16 => 'ust/r16',
      17 => 'ust/r17',
      18 => 'ust/r18',
      19 => 'ust/r19',
      20 => 'ust/r20',
      21 => 'ust/r21',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "UstRegionsPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UstRegionsObtainTitlePage");
    $title->addMethod('findTitleWithPrependedRegion1');
    $title->addMethod('findTitleTag');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the U.S. Trustee Program Regions and Offices.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('ust-regions')
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
 * Migrates files (*.pdf) from /ust-regions.
 *
 * @package doj_migration
 * @subpackage ust-regions
 */
class UstRegionsFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the ust subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      1 => 'ust/r01',
      2 => 'ust/r02',
      3 => 'ust/r03',
      4 => 'ust/r04',
      5 => 'ust/r05',
      6 => 'ust/r06',
      7 => 'ust/r07',
      8 => 'ust/r08',
      9 => 'ust/r09',
      10 => 'ust/r10',
      11 => 'ust/r11',
      12 => 'ust/r12',
      13 => 'ust/r13',
      14 => 'ust/r14',
      15 => 'ust/r15',
      16 => 'ust/r16',
      17 => 'ust/r17',
      18 => 'ust/r18',
      19 => 'ust/r19',
      20 => 'ust/r20',
      21 => 'ust/r21',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///ust-regions/docs';

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
    return "ust-regions";
  }
}


/**
 * SourceParser for /ust-regions pages.
 *
 * @package doj_migration
 * @subpackage ust-regions
 */
class UstRegionsPageSourceParser  extends NGNodeSourceParser {

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    $selectors = array(
      '.maintitle-header',
      '.tnytxt',
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);

    parent::cleanHtml();
  }
}


/**
 * Obtainer for title property for /ust-regions pages.
 *
 * @package doj_migration
 * @subpackage ust-regions
 */
class UstRegionsObtainTitlePage extends ObtainTitle {

  /**
   * Finder for title in h4 and region in h1 concatinated.
   *
   * @return string
   *   The text found.
   */
  protected function findTitleWithPrependedRegion1() {
    $region = $this->findH1Any();
    $element = $this->queryPath->find("h4")->first();
    $this->setElementToRemove($element);
    if (!empty($region)) {
      $concat = "{$region}: {$element->text()}";
    }
    else {
      $concat = $element->text();
    }
    return $concat;
  }
}
