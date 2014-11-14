<?php

/**
 * @file
 * Class DistrictPressReleaseMigration
 */

class DistrictPressReleaseMigration extends JusticeHtmlToPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $source_dirs) {
    if (array_key_exists('district', $arguments)) {
      $district = $arguments['district'];
    }
    else {
      throw new Exception("DistrictPressReleaseMigration requires the district argument.");
    }

    $options = array(
      'recurse' => TRUE,
    );

    $arguments['source_parser_class'] = "DistrictPressReleaseSourceParser";

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);

    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates Press Releases from Districts.');
    $this->addFieldMapping('og_group_ref')->defaultValue($district)->sourceMigration("UsaoDistrict");
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $row->field_pr_date = $this->sourceParser->getDate();
    $row->field_pr_subtitle = $this->sourceParser->getSubtitle();
    $row->field_pr_number = $this->sourceParser->getNumber();
  }
}
