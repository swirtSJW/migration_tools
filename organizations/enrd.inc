<?php

/**
 * @file
 * Defines migration classes for Environment and Natural Resources Division.
 */

/**
 * Migrates .html files from /enrd to page nodes.
 *
 * @package doj_migration
 * @subpackage enrd
 */
class EnrdPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      'enrd',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "EnrdPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "EnrdObtainTitlePage");
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findTitleTag');
    $title->addMethod('findH1First');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Environment and Natural Resources Division.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('enrd')
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
 * Migrates files (*.pdf) from /enrd.
 *
 * @package doj_migration
 * @subpackage enrd
 */
class EnrdFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the enrd subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'enrd',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///enrd/docs';

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
    return "enrd";
  }
}

/**
 * Migrates press releases from /enrd.
 *
 * @package doj_migration
 * @subpackage enrd
 */
class EnrdPressReleaseMigration extends NGJusticeHtmlToPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['component'] = "Environment and Natural Resources Division";
    $arguments['source_parser_class'] = "EnrdPressSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "EnrdObtainTitlePress");
    $title->addMethod('findH1Any');
    $title->addMethod('findIdContentstartDivH2Sec');
    $title->addMethod('findH2First');
    $title->addMethod('findClassContentSubDivPCenterStrong');
    $title->addMethod('findClassContentSubDivDivPStrong');
    $title->addMethod('findIdHeadline');
    $title->addMethod('findPStrongEm');
    $title->addMethod('findIdContentstartDivH2');
    $title->addMethod('findDivClassContentSubDivDivCenter');

    $date = new ObtainerInfo('date', "EnrdObtainDate");
    $date->addMethod('findTableRow1Col2');
    $date->addMethod('findTableRow1Col1');
    $date->addMethod('findTable2Row2Col2');
    $date->addMethod('findPAlignCenter');
    $date->addMethod('findIdContentstartFirst');
    $date->addMethod('findClassNewsRight');
    $date->addMethod('findClassBottomLeftContent');
    $date->addMethod('findProbableDate');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;

    $source_dirs = array();
    parent::__construct($arguments, $source_dirs);
  }
}

class EnrdPageSourceParser  extends NGNodeSourceParser {

  /**
   * {@inheritdoc}
   */
  public function getTitle() {
    $title = parent::getTitle();
    $title = str_replace("USDOJ: Environment And Natural Resources Division 100th Anniversary :", "", $title);
    $title = str_replace("USDOJ: Environment And Natural Resources Division :", "", $title);
    $title = str_replace("USDOJ:", "", $title);
    $title = str_replace("A A ", "", $title);
    $this->sourceParserMessage('title cleaned --> @text', array('@text' => $title), WATCHDOG_DEBUG, 2);
    return $title;
  }

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    $this->queryPath->find("font[color='#999999']")->remove();
    $this->queryPath->find(".headline")->remove();

    // Remove the first 2 uls they are menus.
    $uls = $this->queryPath->find("ul");
    $counter = 0;
    foreach ($uls as $ul) {
      $ul->remove();
      $counter++;
      if ($counter == 2) {
        break;
      }
    }
  }
}

class EnrdPressSourceParser extends NGPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

class EnrdObtainTitlePage extends ObtainTitle {
}

class EnrdObtainTitlePress extends ObtainTitlePressRelease {
}

class EnrdObtainDate extends ObtainDate {
}
