<?php
/**
 * @file
 * Remove files with badly encoded names.
 */

/**
 * Recursive helper to process the images in a folder.
 *
 * We are just collecting all the files that match our regex and have a weird
 * encoding.
 */
function do_things($dir) {
  $bad_files = array();
  $files = scandir($dir);
  foreach ($files as $file) {
    $full_file = "{$dir}/{$file}";
    if (is_dir($full_file) && ($file != "." && $file != "..")) {
      $bad_files = array_merge($bad_files, do_things($full_file));
    }
    elseif (!is_dir($full_file)) {
      $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
      if (preg_match($regex, $full_file)) {
        $enc = mb_detect_encoding($full_file);
        if ($enc != 'ASCII') {
          print_r("{$full_file}\n");
          $bad_files[] = $full_file;
        }
      }
    }
  }
  return $bad_files;
}

// The only argument needed is the directory to process without a / at the end.
if (!empty($argv[1]) && is_dir($argv[1])) {
  $dir = $argv[1];
}
else {
  throw new Exception("Need a directory as the first argument for this script");
}
$bad_files = do_things($dir);

$done = FALSE;
while (!$done) {
  $delete = readline("Do you want to delete these files? (Y/N)");

  if ($delete == "Y") {
    foreach ($bad_files as $file) {
      unlink($file);
      print_r("Deleted {$file}\n");
    }
    $done = TRUE;
  }
  elseif ($delete == "N") {
    $done = TRUE;
    print_r("I understand, deleting stuff is dangerous!!\n");
  }
  else {
    print_r("Invalid option. Try again..\n");
  }
}
