<?php

/**
 * @file
 * Contains definitions of doj migrations.
 *
 * @package migration_tools
 */

/**
 * Implements hook_migrate_api().
 */
function migration_tools_migrate_api() {

  $disable_hooks = array(
    'redirect_insert' => array(
      'doj_purge',
    ),
    'redirect_update' => array(
      'doj_purge',
    ),
    'redirect_delete' => array(
      'doj_purge',
    ),
    'node_update' => array(
      'akamai',
      'doj_purge',
      'expire',
    ),
    'node_delete' => array(
      'akamai',
      'doj_purge',
      'expire',
    ),
  );

  $api = include __DIR__ . "/includes/migration_tools_migrations.inc";

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
