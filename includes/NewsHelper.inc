<?php
/**
 * @file
 * News (Press releases and speeches) common functionality.
 */

class NewsHelper {

  /**
   * The string does not contain a year between the year given and 2005.
   *
   * @param int $year
   *   The max year this string can not contain.
   * @param string $string
   *   A string that could contain a year.
   *
   * @return bool
   *   If the string contains a year between 2005 and the year given, reject it.
   */
  public static function rejectNewsOlderThan($year, $string) {
    $oldest_news_year = 2005;

    for ($y = $oldest_news_year; $y < $year; $y++) {
      if (substr_count($string, "{$y}") > 0) {
        return TRUE;
      }
    }
    return FALSE;
  }
}
