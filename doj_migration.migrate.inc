<?php

/**
 * @file
 * Contains definitions of doj migrations.
 *
 * @package doj_migration
 */

/**
 * Implements hook_migrate_api().
 */
function doj_migration_migrate_api() {

  $disable_hooks = array(
    'node_update' => array(
      'akamai',
      'expire',
    ),
    'node_delete' => array(
      'akamai',
      'expire',
    ),
  );

  $api = include __DIR__ . "/includes/doj_migration_migrations.inc";

  foreach ($api['groups'] as $key => $info) {
    // Run the title through t().
    $title = $info['title'];
    $info['title'] = t("@title", array('@title' => $title));

    // Add the disable_hooks settings to each group.
    $info['disable_hooks'] = $disable_hooks;
    $api['groups'][$key] = $info;
  }

  return $api;
}
