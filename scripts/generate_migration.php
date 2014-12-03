<?php
/**
 * @file
 * Generate the basic setup for a district migration.
 */

include "../../../../../../vendor/autoload.php";
use Symfony\Component\Yaml\Parser;

$global_keys = array(
  'twig',
);

$specific_keys = array(
  'abbreviation',
  'full_name',
  'directory',
);

$global = get_configuration(get_config_path($argv, 1, "global"), $global_keys);
// The specific twig template we are using might have specific requirements.
if (!empty($global['required_keys'])) {
  $specific_keys = array_merge($specific_keys, $global['required_keys']);
}
$specific = get_configuration(get_config_path($argv, 2, "specific"), $specific_keys);

doj_migration_info($specific['abbreviation']);
print_r("Information have been added to doj_migration.info.\n");

$pr_subdirectory = !empty($global['pr_subdirectory']) ? $global['pr_subdirectory'] : NULL;
$pr = isset($pr_subdirectory) ? TRUE : FALSE;

doj_migrate_migrate_inc($specific['abbreviation'], $specific['full_name'], $pr);
print_r("The group and migrations have been registered in the api array.\n");

$drush_alias = !empty($global['drush_local_alias']) ? $global['drush_local_alias'] : NULL;

migration_file($specific['abbreviation'], $specific['full_name'], $specific['directory'], $global['twig'], $pr_subdirectory, $drush_alias);
print_r("File with migrations was created.");

/**
 * Get and validate the config paths given.
 */
function get_config_path($argv, $index, $config_name) {
  // Check the argument.
  if (!array_key_exists($index, $argv)) {
    die("The path to a {$config_name} yaml configuration file is required as the argument {$index}");
  }

  $path = $argv[$index];

  // Validate the path to the config file.
  if (!file_exists($path)) {
    die("The given {$config_name} path to a yaml file does not exist");
  }

  return $path;
}

/**
 * Get the yaml configuration.
 */
function get_configuration($path, $required) {
  // Initialize the yaml parser.
  $yaml = new Parser();

  // Validate the local configuration.
  $config = $yaml->parse(file_get_contents($path));

  foreach ($required as $key) {
    if (!array_key_exists($key, $config)) {
      die("Missing '{$key}' from {$path} configuration file");
    }
  }
  return $config;
}

/**
 * Adding the file info to the .info file.
 */
function doj_migration_info($abbreviation) {
  // let's add the file to doj_migration.info
  $file_name = str_replace("-", "_", $abbreviation);
  file_put_contents("../doj_migration.info", "files[] = organizations/{$file_name}.inc\n", FILE_APPEND);
}

/**
 * Add the group and the migrations to the api array.
 */
function doj_migrate_migrate_inc($abbreviation, $full_name, $pr = FALSE) {
  // Now, Let's add the necessary info to doj_migration.migrate.inc
  $api = include "../includes/doj_migration_migrations.inc";

  // Add the group definition.
  $api['groups'][$abbreviation] = array(
    'title' => $full_name,
  );

  $class_base = class_base($abbreviation);

  // Add the migration definitions.
  $migrations = array("File", "Page");

  if ($pr) {
    $migrations[] = "PressRelease";
  }

  foreach ($migrations as $migration) {
    $api['migrations']["{$class_base}{$migration}"] = array(
      'group_name' => $abbreviation,
      'class_name' => "{$class_base}{$migration}Migration",
    );
  }

  // Save the modifications.
  $header = "<?php\n/**\n * @file\n * Migrations.\n */\n\n// @codingStandardsIgnoreStart\nreturn ";
  file_put_contents("../includes/doj_migration_migrations.inc", $header . var_export($api, TRUE) . ";\n// @codingStandardsIgnoreEnd\n");
}

/**
 * Generate the migrations file.
 */
function migration_file($abbreviation, $full_name, $directory, $twig, $pr_subdirectory = NULL, $drush_alias = NULL) {
  // Generate the classes with twig.
  $loader = new Twig_Loader_Filesystem('.');
  $twig_env = new Twig_Environment($loader, array(
    'cache' => '.',
  ));

  if ($pr_subdirectory) {
    if ($drush_alias) {
      $html_array_data = shell_exec("drush @{$drush_alias} doj-migrate-html-folders {$directory}");
    }
    else {
      $html_array_data = shell_exec("drush doj-migrate-html-folders {$directory}");
    }
    $data = explode("\n", $html_array_data);

    $page = array();
    $press = array();
    foreach ($data as $line) {
      // Ignore array things.
      if (substr_count($line, "(") == 0 && substr_count($line, ")") == 0 && !empty($line)) {
        if (substr_count($line, "{$pr_subdirectory}/") > 0) {
          if (substr_count($line, "2013") > 0 || substr_count($line, "2014")) {
            $press[] = trim($line);
          }
        }
        else {
          $page[] = trim($line);
        }
      }
    }
  }

  $info = array(
    'abbreviation' => $abbreviation,
    'full_name' => $full_name,
    'class' => class_base($abbreviation),
    'directory' => $directory,
  );

  if (isset($page)) {
    $info['page'] = $page;
    $info['press'] = $press;
  }

  $classes_file_data = $twig_env->render("templates/{$twig}", array('info' => $info));
  $class_file_name = class_file_name($abbreviation);
  file_put_contents("../organizations/{$class_file_name}", $classes_file_data . "\n");
}

/**
 * The camel case version of the abbreviation.
 */
function class_base($abbreviation) {
  $abbr_pieces = explode("-", $abbreviation);
  $class = '';
  foreach ($abbr_pieces as $p) {
    $class .= ucfirst($p);
  }
  return $class;
}

/**
 * Make the migration file name from the district abbreviation.
 */
function class_file_name($abbreviation) {
  $abbr_pieces = explode("-", $abbreviation);
  $file = '';
  $counter = 0;
  foreach ($abbr_pieces as $p) {
    $file .= ($counter >= 1) ? "_" : "";
    $file .= $p;
    $counter++;
  }
  return $file . ".inc";
}
