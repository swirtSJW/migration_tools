<?php

/**
 * @file
 * Defines migration classes for Antitrust Division.
 */

/**
 * Migrates .html files from /atr to page nodes.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      'atr',
    );

    $options = array('recurse' => TRUE);

    $arguments['source_parser_class'] = "AtrPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "AtrObtainTitlePage");
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('findTitleTag');
    $title->addMethod('findH1First');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Antitrust Division.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('atr')
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
 * Migrates files (*.pdf) from /atr.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the atr subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'atr',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///atr/docs';

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
    return "atr";
  }
}


/**
 * SourceParser for /atr pages.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /atr press releases.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrPressSourceParser extends NGPressReleaseSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /atr pages.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /atr press releases.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrObtainTitlePress extends ObtainTitlePressRelease {
}

/**
 * Custom obtainer for date property for /atr.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrObtainDate extends ObtainDate {
}
