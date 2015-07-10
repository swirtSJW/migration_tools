<?php

/**
 * @file
 * Defines migration classes for Distrito Sur De Ohio.
 */

/**
 * Migrates .html files from /usao-sdoh-espanol to page nodes.
 *
 * @package doj_migration
 * @subpackage usao-sdoh-espanol
 */
class UsaoSdohEspanolPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'usao/ohs/spanish',
    );
    $options = array('recurse' => FALSE);
    $extension_regex = '/.*\.(html|htm)$/i';

    $arguments['source_parser_class'] = "UsaoSdohEspanolPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "UsaoSdohEspanolObtainTitlePage");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('findBreadcrumbLastNonAnchor', array('.breadcrumb'));
    $title->addMethod('findBreadcrumbLastAnchor', array('.breadcrumb'));
    $title->addMethod('findSubBannerAlt');

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $extension_regex, $options);
    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates pages from the Distrito Sur De Ohio.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('usao-sdoh-espanol')
    ->sourceMigration('UsaoDistrict');
    $this->addFieldMapping('language')->defaultValue('es');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/usao/ohs/spanish/index.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * SourceParser for /usao-sdoh-espanol pages.
 *
 * @package doj_migration
 * @subpackage usao-sdoh-espanol
 */
class UsaoSdohEspanolPageSourceParser  extends NGDistrictPageSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * Obtainer for title property for /usao-sdoh-espanol pages.
 *
 * @package doj_migration
 * @subpackage usao-sdoh-espanol
 * @see Obtainer.api.php
 */
class UsaoSdohEspanolObtainTitlePage extends ObtainTitle {
}
