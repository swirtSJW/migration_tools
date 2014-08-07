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

  $items['doj-migrate-html-folders'] = array(
    'description' => "Find all folders that contain html files.",
    'aliases' => array('dmhf'),
    'arguments' => array(
      'organization' => 'The organization to migrate',
    ),
  );

  $items['doj-migrate-file-folders'] = array(
    'description' => "Migrate all image files from a source directory to the public files directory.",
    'aliases' => array('dmff'),
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

/**
 * Drush command callback.
 */
function drush_doj_migration_doj_migrate_html_folders($organization) {
  drush_print_r(doj_migrate_find_folders_with_files($organization, array("htm", "html")));
}

/**
 * Drush command callback.
 */
function drush_doj_migration_doj_migrate_file_folders($organization) {

  drush_print_r(doj_migrate_find_folders_with_files($organization,
    array("pdf", "txt", "rtf", "doc", "docx", "xls", "xlsx", "csv", "mp3",
      "mp4", "wpd", "wp", "qpw", "xml", "ppt", "pptx")));
}

/**
 * Find all the folders inside of org migraton souce that match the extensions.
 *
 * @param string $organization
 *   The org abbreviations for the org we want to process.
 * @param array $exts
 *   The extension of the type of files that we want to find.
 *
 * @return string
 *   A JSON string of an array containing all of the folders that contain files
 *   of the given extensions.
 */
function doj_migrate_find_folders_with_files($organization, $exts) {
  $source_directory = variable_get("doj_migration_base_dir");
  if (!$source_directory) {
    throw new Exception("the doj_migration_base_dir variable has not been defined");
  }
  $organization_dir = $source_directory . "/{$organization}";

  $file_types = $exts;

  $files = array();

  foreach ($file_types as $ft) {
    $output = shell_exec("find {$organization_dir} -type f -name '*.{$ft}'");
    $files = array_merge($files, explode("\n", $output));
  }

  $folders = array();
  // Let's get all the folders.
  foreach ($files as $f) {
    $file = str_replace($source_directory . "/", "", $f);
    $pieces = explode("/", $file);

    // Remove the file part.
    $pieces = array_slice($pieces, 0, count($pieces) - 1);
    $key = implode("/", $pieces);
    if (!empty($key)) {
      $folders[$key] = TRUE;
    }
  }

  return drupal_json_encode(array_keys($folders));
}
