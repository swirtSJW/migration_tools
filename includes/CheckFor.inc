<?php

/**
 * @file
 * Contains static methods for checking on elements of a migration document.
 */

class CheckFor {
  /**
   * Checks to see if a date comes after a cutoff.
   *
   * @param string $date
   *   A date to evaluate as can be used by strtotime().
   * @param string $date_cutoff
   *   A date representing the low cut-off mm/dd/yyyy.
   * @param bool $default
   *   What should be returned if the date is invalid or unavailable.
   *
   * @return bool
   *   TRUE if the $date > $date_cutoff, or uncheckable.
   *   FALSE if $date < $date_cutoff
   */
  public static function isDateAfter($date, $date_cutoff, $default = TRUE) {
    $date_cutoff = strtotime($date_cutoff);
    $date = strtotime($date);
    if (($date !== FALSE) && ($date_cutoff !== FALSE)) {
      // Both the $date and $date_cutoff are valid.
      if ($date > $date_cutoff) {
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    // If the comparison is invalid or fails, call it TRUE.
    // Risky but safer than throwing away a file just because it has a bad date.
    return $default;

  }

  /**
   * Determines if the current row is a duplicate using redirects as reference.
   *
   * Legacy paths from site, should not be pointing to more than one node,
   * If this is happening, it is a good sign that we are bringing in duplicate
   * content.
   *
   * @param string $legacy_path
   *   The legacy path that would be registered as a redirect.
   *
   * @return bool
   *   Whether this row is a duplicate or not.
   */
  public static function isDuplicateByRedirect($legacy_path) {
    $parsed = redirect_parse_url($legacy_path);
    $source = isset($parsed['path']) ? ltrim($parsed['path'], '/') : '';
    $redirect = redirect_load_by_source($source);

    if ($redirect) {
      $message = "- @source  -> Skipped: Already redirected to '@redirect'.";
      new MigrationMessage($message, array('@source' => $source, '@redirect' => $redirect->redirect), WATCHDOG_WARNING, 1);
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Determine if a given file is within any of an array of paths.
   *
   * @param array $paths
   *   Array of paths to check.
   * @param object $row
   *   A row object as delivered by migrate.
   *
   * @return bool
   *   -TRUE if the file is one of the paths.
   *   -FALSE if the file is not within one of the paths.
   */
  public static function isInPath(array $paths, $row) {
    foreach ($paths as $path) {
      // Is the file in one of the paths?
      if (stripos($row->fileid, $path) !== FALSE) {
        // The file is in the path.
        return TRUE;
      }
    }
    // This file is not in any of the paths.
    return FALSE;
  }

  /**
   * Determine if a given file should be excluded from the current migration.
   *
   * @param string $file_id
   *   The unique id for the current row. Typically legacy_path or fileid.
   *
   * @param array $files_to_skip
   *   Array of values to skip and not migrate.
   *
   * @return bool
   *   -TRUE if the row should be skipped.
   *   -FALSE if the row should not be skipped.
   */
  public static function isSkipFile($file_id, $files_to_skip) {
    if (in_array($file_id, $files_to_skip)) {
      // This page should be skipped.
      $message = '- @fileid  -> Skipped: intentionally.';
      watchdog('migration_tools', $message, array('@fileid' => $file_id), WATCHDOG_WARNING);

      return TRUE;
    }

    // This page should not be skipped.
    return FALSE;
  }

  /**
   * Check the file to see if it is the desired type. Msg watchdog if it is not.
   *
   * @param string $desired_type
   *   The content type machine name that should be kept / not skipped.
   * @param object $row
   *   A row object as delivered by migrate.
   *
   * @return bool
   *   TRUE - the file is the desired type or can't be evaluated.
   *   FALSE - if is definitely not that type.
   */
  public static function isType($desired_type, $row) {
    // In order to have $row->content_type populated, add a find method(s) to
    // ObtainContentType.php or an extension of it and add it to the find stack.
    if (property_exists($row, 'content_type') && ($row->content_type != $desired_type)) {
      // This page does not match to $target_type.
      $message = "- @fileid -- Is type '@content_type_obtained' NOT '@desired_type'";
      $vars = array(
        '@desired_type' => $desired_type,
        '@content_type_obtained' => $row->content_type,
        '@fileid' => $row->fileid,
      );

      return FALSE;
    }
    return TRUE;
  }
}
