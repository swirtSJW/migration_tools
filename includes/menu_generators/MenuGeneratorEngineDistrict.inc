<?php

/**
 * @file
 * Contains menu generator engine for USAO districts.
 */

/**
 * Class MenuGeneratorEngineDistrict.
 */
class MenuGeneratorEngineDistrict extends MenuGeneratorEngineDefault {
  /**
   * {@inheritdoc}
   */
  protected function recurse($css_selector = NULL, $prefix = "") {
    module_load_include("inc", "doj_migration", "includes/doj_migration");
    if (!isset($css_selector)) {
      $css_selector = $this->initialCssSelector;
    }
    $pre = $prefix;
    $css_selector .= ">li";

    drush_doj_migration_debug_output("CSS INITIAL: $css_selector \n");
    drush_doj_migration_debug_output("PRE: $pre \n");

    $query = $this->getQueryPath();

    // In the districts, inside the ul all we have are lis.
    $elements = $query->find($css_selector . ">*");

    // And inside the lis, we might have other uls to recurse on,
    // or anchor elements, lets deal with both those cases.
    foreach ($elements as $elem) {
      if ($elem->is("ul")) {
        drush_doj_migration_debug_output('Im in a ul');
        $class_name = doj_migration_random_string();
        $elem->attr('class', $class_name);
        $this->recurse("{$css_selector}>ul.{$class_name}", "{$pre}-");
      }
      elseif ($elem->is("a")) {
        drush_doj_migration_debug_output('Im in a anchor');
        $a = $elem;

        $al = $a->text();
        $uri = $this->normalizeUri($a->attr("href"));
        $line = "{$pre}{$al}{\"url\":\"{$uri}\"}\n";
        drush_doj_migration_debug_output("CSS INNER $al: $css_selector \n");
        drush_doj_migration_debug_output("PRE INNER: $pre \n");
        drush_doj_migration_debug_output("LINE: $line");
        $this->content .= $line;
      }
    }
  }
}
