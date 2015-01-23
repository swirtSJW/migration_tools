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

    if (array_key_exists('component_tid', $arguments)) {
      $component_tid = $arguments['component_tid'];
    }
    else {
      throw new Exception("DistrictPressReleaseMigration requires the component_tid argument.");
    }

    $options = array(
      'recurse' => TRUE,
    );
    if (empty($arguments['source_parser_class'])) {
      $arguments['source_parser_class'] = "DistrictPressReleaseSourceParser";
    }
    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);

    $this->dependencies = array('UsaoDistrict');
    $this->description = t('Migrates Press Releases from Districts.');
    $this->addFieldMapping('og_group_ref')->defaultValue($district)->sourceMigration("UsaoDistrict");

    $this->removeFieldMapping("field_pr_component");
    $this->removeFieldMapping("field_pr_component:source_type");
    $this->addFieldMapping("field_pr_component")->defaultValue(array($component_tid));
    $this->addFieldMapping("field_pr_component:source_type")->defaultValue('tid');
  }
}
