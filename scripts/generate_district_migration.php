<?php
/**
 * @file
 * Generate the basic setup for a district migration.
 */

include "../../../../../../vendor/autoload.php";

$district = readline("District abbreviation: ");
$full_name = readline("District full name: ");
$directory = readline("Path to migration files: ");
$press_sub = readline("Press release subdirectory: ");
$drush_alias = readline("Drush local alias (without the @): ");

$abbr_pieces = explode("-", $district);
$class = ucfirst($abbr_pieces[0]) . ucfirst($abbr_pieces[1]);
$class_file_name = $abbr_pieces[0] . "_" . $abbr_pieces[1] . ".inc";

// First, let's add the file to doj_migration.info
$file_name = str_replace("-", "_", $district);
file_put_contents("../doj_migration.info", "files[] = organizations/{$district}.inc\n", FILE_APPEND);

// Now, Let's add the necessary info to doj_migration.migrate.inc
$content = file_get_contents("../doj_migration.migrate.inc");
$cut1 = "'groups' => array(";
$cut2 = "),\n    'migrations' => array(";
$cut3 = "),\n  );\n  return \$api;\n}";
// Lets get the api and migrations sections.
$pieces = explode($cut1, $content);
$stuff = array_pop($pieces);
$pieces2 = explode($cut2, $stuff);
$stuff = array_pop($pieces2);
$pieces3 = explode($cut3, $stuff);

$final = array_merge($pieces, $pieces2, $pieces3);

// Add the group definition.
$final[1] .= "  '{$district}' => array(\n        'title' => t('{$full_name}'),\n        'disable_hooks' => \$disable_hooks,\n      ),\n    ";
// Add the migration definitions.
$final[2] .=
  "  '{$class}File' => array(\n        'group_name' => '{$district}',\n        'class_name' => '{$class}FileMigration',\n      ),\n" .
  "      '{$class}Page' => array(\n        'group_name' => '{$district}',\n        'class_name' => '{$class}PageMigration',\n      ),\n" .
  "      '{$class}PressRelease' => array(\n        'group_name' => '{$district}',\n        'class_name' => '{$class}PressReleaseMigration',\n      ),\n    ";
// Put the file back together.
$migrate = $final[0] . $cut1 . $final[1] . $cut2 . $final[2] . $cut3 . "\n";
file_put_contents("../doj_migration.migrate.inc", $migrate);

// Generate the classes with twig.
$loader = new Twig_Loader_Filesystem('.');
$twig = new Twig_Environment($loader, array(
  'cache' => '.',
));

$html_array_data = shell_exec("drush @{$drush_alias} doj-migrate-html-folders {$directory}");
$data = explode("\n", $html_array_data);

foreach ($data as $line) {
  // Ignore array things.
  if (substr_count($line, "(") == 0 && substr_count($line, ")") == 0 && !empty($line)) {
    if (substr_count($line, "{$press_sub}/") > 0) {
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
  'district' => $district,
  'full_name' => $full_name,
  'class' => $class,
  'directory' => $directory,
  'page' => $page,
  'press' => $press,
);

$classes_file_data = $twig->render('template.php.twig', array('info' => $info));

file_put_contents("../organizations/{$class_file_name}", $classes_file_data . "\n");
