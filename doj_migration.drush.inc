<?php

/**
 * @file
 * Drush commands for DOJ Deploy.
 */

/**
 * Implements hook_drush_command().
 */
function doj_migration_drush_command() {
  $items = array();

  $items['doj-migrate-images'] = array(
    'description' => "Migrate all image files from a source directory to the public files directory.",
    'aliases' => array('dmi'),
    'arguments' => array(
      'organization' => 'The organization to migrate',
    ),
  );

  return $items;
}

/**
 * Drush command callback.
 */
function drush_doj_migration_doj_migrate_images($organization) {
  // Let's get our source directory, and our output directory.
  $source_directory = variable_get("doj_migration_base_dir");
  if (!$source_directory) {
    throw new Exception("the doj_migration_base_dir variable has not been defined");
  }

  if ($wrapper = file_stream_wrapper_get_instance_by_uri('public://')) {
    $output_directory = $wrapper->realpath();
  }
  else {
    throw new Exception("We couldn't get the absolute path for public://");
  }

  doj_migration_move_images($source_directory, $organization, $output_directory);
}
