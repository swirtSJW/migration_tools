<?php
/**
 * @file
 * Functions for handling urls.
 */

class UrlTools {
  /**
   * Take a legacy uri, and map it to an alias.
   *
   * @param string $legacy_uri
   *   A legacy uri gets redirected to a node.
   *
   * @return string
   *   The alias matching the legacy uri, or an empty sting.
   */
  public static function convertLegacyToAlias($legacy_uri) {
   $redirect = redirect_load_by_source($legacy_uri);
   if ($redirect) {
     $nid = str_replace("node/", "", $redirect->redirect);
     $node = node_load($nid);

     if ($node && array_key_exists('alias', $node->path) && !empty($node->path['alias'])) {

       return $node->path['alias'];
     }

     // Check for language other than und, because the aliases are intentionally
     // saved with language undefined, even for a spanish node.  A spanish node,
     // when loaded does not find an alias.
     if (!empty($node->language) && ($node->language != LANGUAGE_NONE)) {
       // Some other language in play, so lookup the alias directly.
       $path = url($redirect->redirect);
       $path = ltrim($path, '/');
       return $path;
     }

     if ($node) {
       $uri = entity_uri("node", $node);
       if (array_key_exists('path', $uri)) {
         return $uri['path'];
       }
     }
   }
   $message = "legacy uri @legacy_uri does not have a node associated with it";
   new MigrationMessage($message, array('@legacy_uri' => $legacy_uri), WATCHDOG_ALERT, 1);
   // Without legacy path yet migrated in, leave the link to source content
   // so that the redirects can handle it when that content is migrate/created.
   return MIGRATION_BASE_URL . "/{$legacy_uri}";
  }

  /**
   * Generates a legacy file path based on a row's original path.
   *
   * @param object $row
   *   The row being imported.
   */
  public static function generateLegacyPath($row) {
    // $row->url_path can be used as an identifer, whereas $row->legacy_path
    // may have multiple values.

    // @TODO Need to alter connection to old path but it won't come from fileid.
    $row->url_path = substr($row->fileid, 1);
    $row->legacy_path = $row->url_path;
  }


  /**
   * Convert a relative url to absolute.
   *
   * @param string $rel
   *   Relative url.
   * @param string $base
   *   Base url.
   * @param string $subpath
   *   An optional sub-path to check for when translating relative URIs that are
   *   not root based.
   *
   * @return string
   *   The relative url transformed to absolute.
   */
  public static function convertRelativeToAbsoluteUrl($rel, $base, $subpath = '') {
    // Return if already absolute URL.
    if (parse_url($rel, PHP_URL_SCHEME) != '') {
      return $rel;
    }

    // Check for presence of subpath in $rel to see if a subpath is missing.
    if ((!empty($subpath)) && (!stristr($rel, $subpath))) {
      // The subpath is not present, so add it.
      $rel = $subpath . '/' . $rel;
    }

    // Queries and anchors.
    if ($rel[0] == '#' || $rel[0] == '?') {
      return $base . $rel;
    }

    // Parse base URL and convert to local variables:
    // $scheme, $host, $path.
    extract(parse_url($base));

    // Remove non-directory element from path.
    $path = preg_replace('#/[^/]*$#', '', $path);

    // Destroy path if relative url points to root.
    if ($rel[0] == '/') {
      $path = '';
    }

    // Dirty absolute URL.
    $abs = "$host$path/$rel";

    // Replace '//' or '/./' or '/foo/../' with '/'.
    $re = array('#(/\.?/)#', '#/(?!\.\.)[^/]+/\.\./#');
    for ($n = 1; $n > 0; $abs = preg_replace($re, '/', $abs, -1, $n)) {
    }

    // Absolute URL is ready.
    return $scheme . '://' . $abs;
  }

  /**
   * Creates a redirect from a legacy path if one does not exist.
   *
   * @param string $source_path
   *   The path or url of the legacy source.
   * @param string $destination
   *   The drupal destination of the redirect (example: node/123).
   * @param object $destination_node
   *   (required if the redirect is a node) Node object of the destination node.
   */
  public static function createRedirect($source_path, $destination, $destination_node = '') {
    $alias = $destination;
    if (!empty($source_path)) {
      if (!empty($destination_node)) {
        if (!empty($destination_node->nid)) {
          $destination = $destination_node->nid;
        }
        if ((!empty($destination_node->path)) && (!empty($destination_node->path['alias']))) {
          $alias = $destination_node->path['alias'];
        }
      }
      // Check to see if the source and destination or alias are the same.
      if (($source_path !== $destination) && ($source_path !== $alias) ) {
        // The source and destination are different, so make the redirect.
        $redirect = redirect_load_by_source($source_path);
        if (!$redirect) {
          // The redirect does not exists so create it.
          $redirect = new stdClass();
          redirect_object_prepare($redirect);
          $redirect->source = $source_path;
          $redirect->redirect = $destination;

          redirect_save($redirect);
          $message = 'Redirect created: @source ---> @destination';
          $variables = array(
              '@source' => $source_path,
              '@destination' => $redirect->redirect,
            );
          new MigrationMessage($message, $variables, FALSE, 2);
        }
        else {
          // The redirect already exists.
          $message = 'The redirect of @legacy already exists pointing to @alias. A new one was not created.';
          $variables = array(
              '@legacy' => $source_path,
              '@alias' => $redirect->redirect,
            );
          new MigrationMessage($message, $variables, FALSE, 2);
        }
      }
      else {
        // The source and destination are the same. So no redirect needed.
        $message = 'The redirect of @source have idential source and destination. No redirect created.';
        $variables = array(
            '@source' => $source_path,
          );
        new MigrationMessage($message, $variables, FALSE, 2);
      }
    }
    else {
      // The is no value for redirect.
      $message = 'The source path is missing. No redirect can be built.';
      $variables = array();
      new MigrationMessage($message, $variables, FALSE, 2);
    }
  }
}