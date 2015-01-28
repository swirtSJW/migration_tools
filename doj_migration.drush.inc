<?php

/**
 * @file
 * Drush commands for DOJ Migration.
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

  $items['doj-generate-migrate-commands'] = array(
    'description' => "Generates a list of migrate commands for one or more organic groups.",
    'aliases' => array('dgmc'),
    'arguments' => array(
      'groups' => 'The abbreviation of the groups to migrate, separated by commas',
    ),
    'examples' => array(
      'drush dgmc usao-als,tax' => 'Generate commands for the usao-als and tax groups.',
    ),
    'options' => array(
      'brackets' => 'Prefixes each command with a bracketed checkbox. E.g., "[] command".',
    ),
  );

  $items['doj-migrate-html-folders'] = array(
    'description' => "Find all folders that contain html files.",
    'aliases' => array('dmhf'),
    'arguments' => array(
      'organization' => 'The organization to migrate',
    ),
    'options' => array(
      'ignore' => 'Comma delimited list of strings in folder to ignore',
    ),
  );

  $items['doj-migrate-file-folders'] = array(
    'description' => "Find all folders that contain binary/not-image files.",
    'aliases' => array('dmff'),
    'arguments' => array(
      'organization' => 'The organization to migrate',
    ),
    'options' => array(
      'ignore' => 'Comma delimited list of strings in folder to ignore',
    ),
  );

  $items['doj-generate-menu'] = array(
    'description' => "Generate a file to import a menu for an organization.",
    'aliases' => array('dgm'),
    'arguments' => array(
      'organization' => 'The abbreviation of the organization to migrate (ex. oarm, ola, etc.)',
    ),
    'options' => array(
      'menu-location-uri' => "If the menu is in www.justice.gov/oarm/division/index.html then this option should be set to 'oarm/division/index.html'. If no option is given the default will be www.justice.gov/<organization>",
      'local-base-uri' => "When pages are migrated into drupal they will usually be at <organization>/page-title-alias, if for some reason, <organization> is not the base path of all the pages relevant to the menu that is being generated this option can be used to override that.",
      'css-selector' => "The generation algorithm starts evaluating a menu hierarchy at the outer most ul of the menu.If that ul is not at the default css-selector 'div.leftnav>ul', then you can use this option to point the algorithm to another ul.",
    ),
  );

  $items['doj-import-group-menu'] = array(
    'description' => "Imports a menu for a specific organic group.",
    'aliases' => array('digm'),
    'arguments' => array(
      'file' => 'Required. menu text file name. Assumed that file exists in sites/all/module/custom/doj_migration/sources',
      'organization' => 'The organization to migrate',
    ),
    'examples' => array(
      'drush digm oip-menu.txt oip' => 'Import menu for organic group with group abbreviation oip',
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
 * Drush command callback for doj-generate-migrate-commands.
 *
 * @param string $groups
 *   A string of group abbreviations, separated by commas.
 */
function drush_doj_migration_doj_generate_migrate_commands($groups) {
  drush_print("");
  drush_print('drush migrate-register');
  drush_print("");

  $groups = explode(',', $groups);
  $brackets = drush_get_option('brackets');
  $prefix = '';
  if ($brackets) {
    $prefix = '[] ';
  }

  foreach ($groups as $group) {
    $group = trim($group);
    drush_print($prefix . "drush mi --group=$group");
    drush_print($prefix . "drush digm $group-menu.txt $group");

    $directory = $group;
    if (strpos($group, 'usao-') === 0) {
      $directory = str_replace('usao-', 'usao/', $group);
    }
    drush_print($prefix . "drush dmi $directory");
    drush_print("");
  }
}

/**
 * Drush command callback.
 */
function drush_doj_migration_doj_migrate_html_folders($organization) {
  $ignore = drush_get_option('ignore');
  $ignore = explode(",", $ignore);
  drush_print_r(doj_migrate_find_folders_with_files($organization, array("htm", "html"), $ignore));
}

/**
 * Drush command callback.
 */
function drush_doj_migration_doj_migrate_file_folders($organization) {
  $ignore = drush_get_option('ignore');
  $ignore = explode(",", $ignore);
  drush_print_r(doj_migrate_find_folders_with_files($organization,
    array("pdf", "txt", "rtf", "doc", "docx", "xls", "xlsx", "csv", "mp3",
      "mp4", "wpd", "wp", "qpw", "xml", "ppt", "pptx"), $ignore));
}

/**
 * Find all the folders inside of org migraton souce that match the extensions.
 *
 * @param string $organization
 *   The org abbreviations for the org we want to process.
 * @param array $exts
 *   The extension of the type of files that we want to find.
 * @param array $ignore
 *   A list of words that if found in a folder path will ignore it from the
 *   list.
 *
 * @return string
 *   A JSON string of an array containing all of the folders that contain files
 *   of the given extensions.
 *
 * @throws Exception
 *   If migrate_html_base_dir variable is not set.
 */
function doj_migrate_find_folders_with_files($organization, $exts, $ignore = array()) {
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
    $ignore_folder = doj_migration_ignore_folder($key, $ignore);
    if (!empty($key) && !$ignore_folder) {
      $folders[$key] = TRUE;
    }
  }

  return var_export(array_keys($folders));
}

/**
 * Ignore this folder, or not.
 *
 * @param string $folder
 *   a strin representing a folder.
 * @param array $ignore
 *   A list of string to fine in folders to ignore.
 *
 * @return bool
 *   Should this folder be ignored.
 */
function doj_migration_ignore_folder($folder, $ignore = array()) {
  $ignore_it = FALSE;

  foreach ($ignore as $string) {
    if (!empty($string) && substr_count($folder, $string) > 0) {
      $ignore_it = TRUE;
      break;
    }
  }

  return $ignore_it;
}

/**
 * Move all the images from a directory tree to an output directory.
 *
 * The directory strucuture in the output directory will be an exact match of
 * the directory structury from the source directory.
 *
 * @param string $parent_input_directory
 *   The parent directory where anothe directory with images is located.
 * @param string $directory
 *   The directory that contains images.
 * @param string $parent_output_directory
 *   The directory where we want to save all the images.
 */
function doj_migration_move_images($parent_input_directory, $directory, $parent_output_directory) {

  // Make the directory where our files will go.
  $final_output_directory = "$parent_output_directory/$directory";

  // Get all files from the source directory.
  $final_input_directory = "$parent_input_directory/$directory";
  $files = scandir($final_input_directory);
  $count = 0;
  foreach ($files as $file) {
    $ext = pathinfo("$final_input_directory/$file", PATHINFO_EXTENSION);
    $ext = strtoupper($ext);
    // If any of the files are images, copy them.
    if ($ext == "GIF" || $ext == "PNG" || $ext == "JPG" || $ext == "JPEG") {
      // We only want to create the output directory if we have images.
      if (!file_exists($final_output_directory)) {
        mkdir($final_output_directory, 0777, TRUE);
      }

      watchdog("doj_migration", "FILE: @file EXT: @ext \n", array('@file' => $file, 'ext' => $ext));

      if (copy("$final_input_directory/$file", "$final_output_directory/$file")) {
        $count++;
        watchdog("doj_migration", "File @file was copied to $final_output_directory \n", array('@file' => $file));
      }
      else {
        watchdog("doj_migration", "There was an error copying @file \n", array('@file' => $file), WATCHDOG_ERROR);
      }
    }
    // If we are dealing with a directory, let's recurse.
    elseif (is_dir("$final_input_directory/$file") && "{$file}" != "." && "{$file}" != "..") {
      watchdog("doj_migration", "Found a directory {$file} \n");
      doj_migration_move_images($final_input_directory, $file, $final_output_directory);
    }
  }
  if ($count > 0) {
    $message = dt('Moved @count images into -> @final_output_directory.', array('@count' => $count, '@final_output_directory' => $final_output_directory));
    drush_log($message, 'success');
  }
}

/**
 * Drush command callback.
 */
function drush_doj_migration_doj_generate_menu($organization) {
  $css_selector = drush_get_option('css-selector');
  $local_base_uri = drush_get_option('local-base-uri');
  $menu_location_uri = drush_get_option('menu-location-uri');

  $parameters = new MenuGenerationParameters($organization);

  if ($local_base_uri) {
    $parameters->setUriLocalBase($local_base_uri);
  }

  if ($menu_location_uri) {
    $parameters->setUriMenuLocation($menu_location_uri);
  }

  $engine = new MenuGeneratorEngineDistrict($parameters);

  if ($css_selector) {
    $engine->setInitialCssSelector($css_selector);
  }

  $generator = new MenuGenerator($parameters, $engine);
  $file_name = $generator->generate();
  drush_print_r("File {$file_name} was generated.");
}

/**
 * Drush command callback.
 */
function drush_doj_migration_doj_generate_menu_district($organization) {
  module_load_include('inc', 'doj_migration', 'includes/doj_migration');
  doj_migration_create_menu_file("usao|" . $organization, TRUE);
}

/**
 * Drush command callback.
 */
function drush_doj_migration_doj_import_group_menu($file_name, $group_abbrev) {
  module_load_include('inc', 'doj_migration', 'includes/doj_migration');

  $group = doj_sitewide_load_org_by_abbrev($group_abbrev);
  $group_menu_name = 'menu-og-' . $group->nid;
  $file_path = DRUPAL_ROOT . '/' . drupal_get_path('module', 'doj_migration') . '/sources/' . $file_name;

  drush_menu_import_import($file_path, $group_menu_name);
}

/**
 * Wrapper function to output info if doj_migration_drush_debug = TRUE.
 *
 * @param mixed $output
 *   Thing to pass to drush_print_r().
 */
function drush_doj_migration_debug_output($output = '') {
  if (variable_get('doj_migration_drush_debug', FALSE)) {
    drush_print_r($output);
  }
}
