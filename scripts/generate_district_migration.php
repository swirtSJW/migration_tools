<?php
/**
 * @file
 * Generate the basic setup for a district migration.
 */

include "../../../../../../vendor/autoload.php";
use Symfony\Component\Yaml\Parser;

// Check for global configuration.
if (!file_exists("global.yml")) {
  die("A global.yml file is required in the same directory as this script");
}

// Check for specific configuration.
if (!array_key_exists(1, $argv)) {
  die("The path to a yaml configuration file is required as the first argument");
}

$config_file = $argv[1];

// Check that the file exists.
if (!file_exists($config_file)) {
  die("The given path to a yaml file does not exist");
}

// Initialize the yaml parser.
$yaml = new Parser();

// Validate the global configuration.
$global = $yaml->parse(file_get_contents('global.yml'));
if (!array_key_exists("drush_local_alias", $global)) {
  die("'drush_local_alias' key missing from global.yml");
}

// Validate the local configuration.
$config = $yaml->parse(file_get_contents($config_file));
$keys = array(
  'district_abbreviation',
  'district_full_name',
  'migration_directory',
  'press_release_subdirectory',
);

foreach ($keys as $key) {
  if (!array_key_exists($key, $config)) {
    die("Missing '{$key}' from the yaml configuration file");
  }
}

// Set all our variables.
$district_abbreviation = $config['district_abbreviation'];
$district_full_name = $config['district_full_name'];
$migration_directory = $config['migration_directory'];
$press_release_subdirectory = $config['press_release_subdirectory'];
$drush_alias = $global['drush_local_alias'];

doj_migration_info($district_abbreviation);
print_r("Information have been added to doj_migration.info.\n");

doj_migrate_migrate_inc($district_abbreviation, $district_full_name);
print_r("The group and migrations have been registered in the api array.\n");

migration_file($district_abbreviation, $district_full_name, $migration_directory, $press_release_subdirectory, $drush_alias);
print_r("File with migrations was created.");

/**
 * Adding the file info to the .info file.
 */
function doj_migration_info($district_abbreviation) {
  // let's add the file to doj_migration.info
  $file_name = str_replace("-", "_", $district_abbreviation);
  file_put_contents("../doj_migration.info", "files[] = organizations/{$file_name}.inc\n", FILE_APPEND);
}

/**
 * Add the group and the migrations to the api array.
 */
function doj_migrate_migrate_inc($district_abbreviation, $district_full_name) {
  // Now, Let's add the necessary info to doj_migration.migrate.inc
  $api = include "../includes/doj_migration_migrations.inc";

  // Add the group definition.
  $api['groups'][$district_abbreviation] = array(
    'title' => $district_full_name,
  );

  $class_base = class_base($district_abbreviation);

  // Add the migration definitions.
  $migrations = array("File", "Page", "PressRelease");

  foreach ($migrations as $migration) {
    $api['migrations']["{$class_base}{$migration}"] = array(
      'group_name' => $district_abbreviation,
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
function migration_file($district_abbreviation, $district_full_name, $migration_directory, $press_release_subdirectory, $drush_alias) {
  // Generate the classes with twig.
  $loader = new Twig_Loader_Filesystem('.');
  $twig = new Twig_Environment($loader, array(
    'cache' => '.',
  ));

  $html_array_data = shell_exec("drush @{$drush_alias} doj-migrate-html-folders {$migration_directory}");
  $data = explode("\n", $html_array_data);

  $page = array();
  $press = array();
  foreach ($data as $line) {
    // Ignore array things.
    if (substr_count($line, "(") == 0 && substr_count($line, ")") == 0 && !empty($line)) {
      if (substr_count($line, "{$press_release_subdirectory}/") > 0) {
        if (substr_count($line, "2013") > 0 || substr_count($line, "2014")) {
          $press[] = trim($line);
        }
      }
      else {
        $page[] = trim($line);
      }
    }
  }

  $info = array(
    'district' => $district_abbreviation,
    'full_name' => $district_full_name,
    'class' => class_base($district_abbreviation),
    'directory' => $migration_directory,
    'page' => $page,
    'press' => $press,
  );

  $classes_file_data = $twig->render('template.php.twig', array('info' => $info));
  $class_file_name = class_file_name($district_abbreviation);
  file_put_contents("../organizations/{$class_file_name}", $classes_file_data . "\n");
}

/**
 * The camel case version of the district abbreviation.
 */
function class_base($district_abbreviation) {
  $abbr_pieces = explode("-", $district_abbreviation);
  return ucfirst($abbr_pieces[0]) . ucfirst($abbr_pieces[1]);
}

/**
 * Make the migration file name from the district abbreviation.
 */
function class_file_name($district_abbreviation) {
  $abbr_pieces = explode("-", $district_abbreviation);
  return $abbr_pieces[0] . "_" . $abbr_pieces[1] . ".inc";
}
