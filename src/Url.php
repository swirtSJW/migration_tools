<?php
/**
 * @file
 * Functions for handling urls.
 */

namespace MigrationTools;

class Url {

  /**
   * Grabs legacy redirects for this node from D6 and adds $row->redirects.
   *
   * This function needs to be called in prepareRow() of your migration.
   *
   * @param object $row
   *   The object of this row.
   * @param string $db_reference_name
   *   The Drupal name/identifier of the legacy database.
   * @param object $source_connection
   *   Database source connection from migration.
   */
  public static function collectD6RedirectsToThisNode($row, $db_reference_name, $source_connection) {
    // Gather existing redirects from legacy.
    $row->redirects = \Database::getConnection($db_reference_name, $source_connection)
      ->select('path_redirect', 'r')
      ->fields('r', array('source'))
      ->condition('redirect', "node/$row->nid")
      ->execute()
      ->fetchCol();
  }

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
    // Most common drupal paths have no ending / so start with that.
    $legacy_uri_no_end = rtrim($legacy_uri, '/');
    $redirect = redirect_load_by_source($legacy_uri_no_end);
    if (empty($redirect) && ($legacy_uri != $legacy_uri_no_end)) {
      // There is no redirect found, lets try looking for one with the path/.
      $redirect = redirect_load_by_source($legacy_uri);
    }
    if ($redirect) {
      $nid = str_replace("node/", "", $redirect->redirect);
      $node = node_load($nid);

      if ((!empty($node)) && (!empty($node->path)) && (!empty($node->path['alias']))) {
        return $node->path['alias'];
      }

      // Check for language other than und, because the aliases are
      // intentionally saved with language undefined, even for a spanish node.
      // A spanish node, when loaded does not find an alias.
      if (!empty($node->language) && ($node->language != LANGUAGE_NONE)) {
        // Some other language in play, so lookup the alias directly.
        $path = url($redirect->redirect);
        $path = ltrim($path, '/');
        return $path;
      }

      if ($node) {
        $uri = entity_uri("node", $node);
        if (!empty($uri['path'])) {
          return $uri['path'];
        }
      }
    }
    $message = "legacy uri @legacy_uri does not have a node associated with it";
    Message::make($message, array('@legacy_uri' => $legacy_uri), WATCHDOG_NOTICE, 1);
    // Without legacy path yet migrated in, leave the link to source url
    // so that the redirects can handle it when that content is migrate/created.
    $base = variable_get('migration_tools_base_domain', '');
    if (!empty($base)) {
      return "{$base}/{$legacy_uri}";
    }
    else {
      throw new Exception("The base domain is needed, but has not been set. Visit /admin/config/migration_tools");
    }

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
    $row->url_path = substr($row->fileId, 1);
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
   *   The path or url of the legacy source. MUST be INTERNAL to this site.
   * @param string $destination
   *   The destination of the redirect examples:
   *     * path-a/path-b/the-node-title
   *     * http://www.somesite.com
   * @param object $destination_node
   *   (required if the redirect is a node) Node object of the destination node.
   * @param array $allowed_hosts
   *   If passed, this will limit redirect creation to only urls that have a
   *   domain present in the array. Others will be rejected.
   */
  public static function createRedirect($source_path, $destination, $destination_node = '', $allowed_hosts = array()) {
    $alias = $destination;

    // We can not create a redirect for a URL that is not part of the domain
    // or subdomain of this site.
    if (!self::isAllowedDomain($source_path, $allowed_hosts)) {
      $message = "A redirect was NOT built for @source_path because it is not an allowed host.";
      $variables = array(
        '@source_path' => $source_path,
      );
      Message::make($message, $variables, FALSE, 2);
      return FALSE;
    }

    if (!empty($source_path)) {
      // Alter source path to remove any externals.
      $source_path = self::fixSchemelessInternalUrl($source_path);
      $source = parse_url($source_path);
      $source_path = (!empty($source['path'])) ? $source['path'] : '';
      // A path should not have a preceeding /.
      $source_path = ltrim($source['path'], '/');
      $source_options = array();
      // Check for fragments (after #hash ).
      if (!empty($source['fragment'])) {
        $source_options['fragment'] = $source['fragment'];
      }
      // Check for query parameters (after ?).
      if (!empty($source['query'])) {
        parse_str($source['query'], $query);
        $source_options['query'] = $query;
      }

      if (!empty($destination_node)) {
        if (!empty($destination_node->nid)) {
          $destination = 'node/' . $destination_node->nid;
        }
      }
      // Check to see if the source and destination or alias are the same.
      if (($source_path !== $destination) && ($source_path !== $alias)) {
        // The source and destination are different, so make the redirect.
        $redirect = redirect_load_by_source($source_path);
        if (!$redirect) {
          // The redirect does not exists so create it.
          $redirect = new \stdClass();
          redirect_object_prepare($redirect);
          $redirect->source = $source_path;
          $redirect->source_options = $source_options;
          $redirect->redirect = $destination;

          redirect_save($redirect);
          $message = 'Redirect created: @source ---> @destination';
          $variables = array(
            '@source' => $source_path,
            '@destination' => $redirect->redirect,
          );
          Message::make($message, $variables, FALSE, 2);
        }
        else {
          // The redirect already exists.
          $message = 'The redirect of @legacy already exists pointing to @alias. A new one was not created.';
          $variables = array(
            '@legacy' => $source_path,
            '@alias' => $redirect->redirect,
          );
          Message::make($message, $variables, FALSE, 2);
        }
      }
      else {
        // The source and destination are the same. So no redirect needed.
        $message = 'The redirect of @source have idential source and destination. No redirect created.';
        $variables = array(
          '@source' => $source_path,
        );
        Message::make($message, $variables, FALSE, 2);
      }
    }
    else {
      // The is no value for redirect.
      $message = 'The source path is missing. No redirect can be built.';
      $variables = array();
      Message::make($message, $variables, FALSE, 2);
    }
  }

  /**
   * Adds multiple redirects to the same destination.
   *
   * This is typically called within the migration's complete().
   *
   * @param array $redirects
   *   An internal paths to build redirects FROM (ex: array('path/blah', 'foo'))
   * @param string $destination
   *   The destination of where the redirect should go TO (ex: 'node/123')
   */
  public static function createRedirectsMultiple(array $redirects, $destination) {
    foreach ($redirects as $redirect) {
      self::createRedirect($redirect, $destination);
    }
  }

  /**
   * Deletes any redirects associated files attached to an entity's file field.
   *
   * @param object $entity
   *   The fully loaded entity.
   *
   * @param string $field_name
   *   The machine name of the attachment field.
   *
   * @param string $language
   *   Optional. Defaults to LANGUAGE_NONE.
   */
  public static function rollbackAttachmentRedirect($entity, $field_name, $language = LANGUAGE_NONE) {
    $field = $entity->$field_name;
    if (!empty($field[$language])) {
      foreach ($field[$language] as $delta => $item) {
        $file = file_load($item['fid']);
        $url = file_create_url($file->uri);
        $parsed_url = parse_url($url);
        $destination = ltrim($parsed_url['path'], '/');
        redirect_delete_by_path($destination);
      }
    }
  }

  /**
   * Creates redirects for files attached to a given entity's field field.
   *
   * @param object $entity
   *   The fully loaded entity.
   *
   * @param array $source_urls
   *   A flat array of source urls that should redirect to the attachments
   *   on this entity. $source_urls[0] will redirect to the first attachment,
   *   $entity->$field_name[$language][0], and so on.
   *
   * @param string $field_name
   *   The machine name of the attachment field.
   *
   * @param string $language
   *   Optional. Defaults to LANGUAGE_NONE.
   */
  public static function createAttachmentRedirect($entity, $source_urls, $field_name, $language = LANGUAGE_NONE) {
    if (empty($source_urls)) {
      // Nothing to be done here.
      $json_entity = json_encode($entity);
      watchdog("migration_tools", "redirect was not created for attachment in entity {$json_entity}");
      return;
    }

    $field = $entity->$field_name;
    if (!empty($field[$language])) {
      foreach ($field[$language] as $delta => $item) {
        // $file = file_load($item['fid']);
        // $url = file_create_url($file->uri);
        // $parsed_url = parse_url($url);
        // $destination = ltrim($parsed_url['path'], '/');
        $source = $source_urls[$delta];

        // Create redirect.
        $redirect = redirect_load_by_source($source);
        if (!$redirect) {
          $redirect = new \stdClass();
          redirect_object_prepare($redirect);
          $redirect->source = $source;
          $redirect->redirect = "file/{$item['fid']}/download";
          redirect_save($redirect);
        }
      }
    }
  }

  /**
   * Examines an uri and evaluates if it is an image.
   *
   * @param string $uri
   *   A uri.
   *
   * @return bool
   *   TRUE if this is an image uri, FALSE if it is not.
   */
  public static function isImageUri($uri) {
    if (preg_match('/.*\.(jpg|gif|png|jpeg)$/i', $uri) !== 0) {
      // Is an image uri.
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Fixes anchor links to PDFs so that they work in IE.
   *
   * Specifically replaces anchors like #_PAGE2 and #p2 with #page=2.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   *
   * @see http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_open_parameters.pdf
   */
  public static function fixPdfLinkAnchors($query_path) {
    $anchors = $query_path->find('a');
    foreach ($anchors as $anchor) {
      $url = $anchor->attr('href');
      $contains_pdf_anchor = preg_match('/\.pdf#(p|_PAGE)([0-9]+)/i', $url, $matches);
      if ($contains_pdf_anchor) {
        $old_anchor = $matches[1];
        $page_num = $matches[3];
        $new_anchor = 'page=' . $page_num;
        $new_url = str_replace($old_anchor, $new_anchor, $url);
        $anchor->attr('href', $new_url);
      }
    }
  }

  /**
   * Removes the host if the url is intarnal but malformed.
   *
   * A url of 'mysite.com/path1/path2' is malformed because parse_url() will
   * not recognise 'mysite.com' as the host without the scheme (http://) being
   * present.  This method will remove the host if it is for this site and make
   * the url a proper root relative path.
   *
   * @param string $url
   *   A url.
   *
   * @return string
   *   A url or path correctly modified for this site.
   */
  public static function fixSchemelessInternalUrl($url) {
    if (!empty($url)) {
      $parsed_url = parse_url($url);
      if (empty($parsed_url['scheme'])) {
        // It has no scheme, so check if it is a malformed internal url.
        $host = self::getSiteHost();
        $pos = stripos($url, $host);
        if ($pos === 0) {
          // The url is starting with a site's host.  Remove it.
          $url = substr_replace($url, '', $pos, strlen($host));
        }
      }
    }
    return $url;
  }

  /**
   * Returns the defined site host.
   *
   * @return string
   *   The site host for this site example: 'mysite.com'.
   *
   * @throws MigrateException
   *   If the base domain has not been defined in /admin/config/migration_tools.
   */
  public static function getSiteHost() {
    // Obtain the designated url of the site.
    $base_url = variable_get('migration_tools_base_domain', '');
    $site_host = parse_url($base_url, PHP_URL_HOST);
    if (!empty($site_host)) {
      return $site_host;
    }
    else {
      // There is no site host defined.
      $message = "The base domain is needed, but has not been set. Visit /admin/config/migration_tools \n";
      throw new MigrateException($message);
    }
  }

  /**
   * Examines an url to see if it is within a allowed list of domains.
   *
   * @param string $url
   *   A url.
   * @param array $allowed_hosts
   *   A flat array of allowed domains. ex:array('www.site.com', 'site.com').
   *
   * @return bool
   *   TRUE if the host is within the array of allowed.
   *   TRUE if the array of allowed is empty (nothing to compare against)
   *   FALSE if the domain is not with the array of allowed.
   */
  public static function isAllowedDomain($url, $allowed_hosts) {
    $url = self::fixSchemelessInternalUrl($url);
    $host = parse_url($url, PHP_URL_HOST);
    // Treat it as allowed until evaluated otherwise.
    $allowed = TRUE;
    if (!empty($allowed_hosts) && (is_array($allowed_hosts)) && (!empty($host))) {
      // See if the host is allowed (case insensitive).
      $allowed = in_array(strtolower($host), array_map('strtolower', $allowed_hosts));
    }
    return $allowed;
  }


  /**
   * Examines an url to see if it is internal to this site.
   *
   * @param string $url
   *   A url.
   *
   * @param array $allowed_hosts
   *   Optional:  A flat array of allowed domains. Uses base url admin setting.
   *   ex:array('www.site.com', 'site.com').
   *
   * @return bool
   *   TRUE if the host is within the site.
   *   TRUE if there is no host (relative link).
   *   FALSE if the domain is not with this site.
   */
  public static function isInternalUrl($url, $allowed_hosts = array()) {
    if (empty($allowed_hosts)) {
      // Use the defined site host.
      $site_host = self::getSiteHost();
      $allowed_hosts = array($site_host);
    }

    if (!empty($allowed_hosts)) {
      return self::isAllowedDomain($url, $allowed_hosts);
    }
    else {
      // There is insufficient information to determine whether host is allowed.
      $message = "Unable to determine if this is internal link as no allowed hosts are specified.\n";
      throw new MigrateException($message);
    }
  }

  /**
   * Normalize the path to make sure paths are consistent.
   *
   * @param string $uri
   *   A uri.
   *
   * @return string
   *   The cleaned uri. with path ending in / if not a file.
   */
  public static function normalizePathEnding($uri) {
    $uri = trim($uri);
    // If the uri is a path, not ending in a file, make sure it ends in a '/'.
    if (!empty($uri) && !pathinfo($uri, PATHINFO_EXTENSION)) {
      $uri = rtrim($uri, '/');
      $uri .= '/';
    }
    return $uri;
  }

  /**
   * Take parse_url formatted url and return the url/uri as a string.
   *
   * @param array $parsed_url
   *   An array in the format delivered by php php parse_url().
   * @param bool $return_url
   *   Toggles return of full url if TRUE, or uri if FALSE (defaults: TRUE)
   *
   * @return string
   *   URL or URI.
   */
  public static function reassembleURL($parsed_url, $return_url = TRUE) {
    $url = '';
    if ($return_url) {
      $default_base = variable_get('migration_tools_base_domain', '');
      $default_scheme = parse_url($default_base, PHP_URL_SCHEME);
      $default_host = parse_url($default_base, PHP_URL_HOST);
      $url .= (!empty($parsed_url['scheme'])) ? $parsed_url['scheme'] . '://' : $default_scheme . '://';

      if ((empty($default_base)) && (empty($parsed_url['host']))) {
        throw new Exception("The base domain is needed, but has not been set. Visit /admin/config/migration_tools");
      }
      else {
        // Append / after the host to account for it being removed from path.
        $url .= (!empty($parsed_url['host'])) ? $parsed_url['host'] . '/' : $default_host . '/';
      }

    }
    // Trim the initial '/' to be Drupal friendly in the event of no host.
    $url .= (!empty($parsed_url['path'])) ? ltrim($parsed_url['path'], '/') : '';
    $url .= (!empty($parsed_url['query'])) ? '?=' . $parsed_url['query'] : '';
    $url .= (!empty($parsed_url['fragment'])) ? '#' . $parsed_url['fragment'] : '';

    return $url;
  }

  /**
   * Retrieves server or html redirect of the page if it the destination exists.
   *
   * @param object $row
   *   A row object as delivered by migrate.
   * @param QueryPath $query_path
   *   The current QueryPath object.
   * @param array $redirect_texts
   *   (optional) array of human readable strings that preceed a link to the
   *   new location of the page ex: "this page has move to"
   *
   * @return mixed
   *   string - full URL of the validated redirect destination.
   *   string 'skip' if there is a redirect but it's broken.
   *   FALSE - no detectable redirects exist in the page.
   */
  public static function hasValidRedirect($row, $query_path, $redirect_texts = array()) {
    if (empty($row->urlLegacy)) {
      throw new \MigrateException('$row->urlLegacy must be defined to look for a redirect.');
    }
    else {
      // Look for server side redirect.
      $server_side = self::hasServerSideRedirects($row->urlLegacy);
      if ($server_side) {
        // A server side redirect was found.
        return $server_side;
      }
      else {
        // Look for html redirect.
        return self::hasValidHtmlRedirect($row, $query_path, $redirect_texts);
      }
    }
  }


  /**
   * Retrieves redirects from the html of the page if it the destination exists.
   *
   * @param object $row
   *   A row object as delivered by migrate.
   * @param QueryPath $query_path
   *   The current QueryPath object.
   * @param array $redirect_texts
   *   (optional) array of human readable strings that preceed a link to the
   *   new location of the page ex: "this page has move to"
   *
   * @return mixed
   *   string - full URL of the validated redirect destination.
   *   string 'skip' if there is a redirect but it's broken.
   *   FALSE - no detectable redirects exist in the page.
   */
  public static function hasValidHtmlRedirect($row, $query_path, $redirect_texts = array()) {
    $destination = self::getRedirectFromHtml($row, $query_path, $redirect_texts);
    if ($destination) {
      // This page is being redirected via the page.
      // Is the destination still good?
      $real_destination = self::urlExists($destination);
      if ($real_destination) {
        // The destination is good. Message and return.
        $message = "Found redirect in html -> !destination";
        $variables = array('!destination' => $real_destination);
        \MigrationTools\Message::make($message, $variables, FALSE, 2);

        return $destination;
      }
      else {
        // The destination is not functioning. Message and bail with 'skip'.
        $message = "Found broken redirect in html-> !destination";
        $variables = array('!destination' => $destination);
        \MigrationTools\Message::make($message, $variables, \WATCHDOG_ERROR, 2);

        return 'skip';
      }
    }
    else {
      // No redirect destination found.
      return FALSE;
    }
  }


  /**
   * Check for server side redirects.
   *
   * @param string $url
   *   The full url to a live page.
   *
   * @return mixed
   *   string Url of the final desitnation if there was a redirect.
   *   bool FALSE if there was no redirect.
   */
  public static function hasServerSideRedirects($url) {
    $final_url = self::urlExists($url, TRUE);
    if ($final_url && ($url === $final_url)) {
      // The initial and final urls are the same, so no redirects.
      return FALSE;
    }
    else {
      // The $final_url is different, so it must have been redirected.
      return $final_url;
    }
  }

  /**
   * Retrieves redirects from the html of the page (meta, javascrip, text).
   *
   * @param object $row
   *   A row object as delivered by migrate.
   * @param QueryPath $query_path
   *   The current QueryPath object.
   * @param array $redirect_texts
   *   (optional) array of human readable strings that preceed a link to the
   *   new location of the page ex: "this page has move to"
   *
   * @return mixed
   *   string - full URL of the redirect destination.
   *   FALSE - no detectable redirects exist in the page.
   */
  public static function getRedirectFromHtml($row, $query_path, $redirect_texts = array()) {
    // Hunt for <meta> redirects via refresh and location.
    // These use only full URLs.
    $metas = $query_path->find('meta');
    foreach (is_array($metas) || is_object($metas) ? $metas : array() as $meta) {
      $attributes = $meta->attr();
      if (!empty($attributes['http-equiv']) && (($attributes['http-equiv'] === 'refresh') || ($attributes['http-equiv'] === 'location'))) {
        // It has a meta refresh or meta location specified.
        // Grab the url from the content attribute.
        if (!empty($attributes['content'])) {
          $content_array = preg_split('/url=/i', $attributes['content'], -1, PREG_SPLIT_NO_EMPTY);
          // The URL is going to be the last item in the array.
          $url = array_pop($content_array);
          if (filter_var($url, FILTER_VALIDATE_URL)) {
            // Seems to be a valid URL.
            return $url;
          }
        }
      }
    }

    // Hunt for Javascript redirects.
    // Checks for presence of Javascript. <script type="text/javascript">
    $js_scripts = $query_path->top()->find('script');
    foreach (is_array($js_scripts) || is_object($js_scripts) ? $js_scripts : array() as $js_script) {
      $script_text = $js_script->text();
      $url = \MigrationTools\Url::extractUrlFromJS($script_text);
      if ($url) {
        return $url;
      }
    }

    // Try to account for jQuery redirects like:
    // onLoad="setTimeout(location.href='http://www.newpage.com', '0')".
    // So many variations means we can't catch them all.  But try the basics.
    $body_html = $query_path->top()->find('body')->html();
    $search = 'onLoad=';
    $content_array = preg_split("/$search/", $body_html, -1, PREG_SPLIT_NO_EMPTY);
    // If something was found there will be > 1 element in the array.
    if (count($content_array) > 1) {
      // It had an onLoad, now check it for locations.
      $url = \MigrationTools\Url::extractUrlFromJS($content_array[1]);
      if ($url) {
        return $url;
      }
    }

    // Check for human readable text redirects.
    foreach (is_array($redirect_texts) ? $redirect_texts : array() as $i => $redirect_text) {
      // Array of starts and ends to try locating.
      $wrappers = array();
      // Provide two elements: the begining and end wrappers.
      $wrappers[] = array('"', '"');
      $wrappers[] = array("'", "'");
      foreach ($wrappers as $wrapper) {
        $body_html = $query_path->top()->find('body')->innerHtml();
        $url = \MigrationTools\Url::peelUrl($body_html, $redirect_text, $wrapper[0], $wrapper[1]);
        if ($url) {
          return $url;
        }
      }
    }
  }

  /**
   * Checks if a URL actually resolves to a 'page' on the internet.
   *
   * @param string $url
   *   A full destination URI.
   * @param bool $follow_redirects
   *   TRUE (default) if you want it to track multiple redirects to the end.
   *   FALSE if you want to only evaluate the first page request.
   *
   * @return mixed
   *   string url - http response is valid (2xx or 3xx) and has a destination.
   *   bool FALSE - https response is invalid, either 1xx, 4xx, or 5xx
   */
  public static function urlExists($url, $follow_redirects = TRUE) {
    $handle = curl_init();
    curl_setopt($handle, CURLOPT_URL, $url);
    curl_setopt($handle, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($handle, CURLOPT_FOLLOWLOCATION, $follow_redirects);
    curl_setopt($handle, CURLOPT_HEADER, 0);
    // Get the HTML or whatever is linked in $redirect_url.
    $response = curl_exec($handle);

    // Get status code.
    $http_code = curl_getinfo($handle, CURLINFO_HTTP_CODE);
    $last_location = curl_getinfo($handle, CURLINFO_EFFECTIVE_URL);

    $url = ($follow_redirects) ? $last_location : $url;

    // Check that http code exists.
    if ($http_code) {
      // Determines first digit of http code.
      $first_digit = substr($http_code, 0, 1);
      // Filters for 2 or 3 as first digit.
      if ($first_digit == 2 || $first_digit == 3) {
        return $url;
      }
      else {
        // Invalid url.
        return FALSE;
      }
    }
  }


  /**
   * Pull a URL destination from a Javascript script.
   *
   * @param string $string
   *   $string of the script contents.
   *
   * @return mixed
   *   string - the validated URL if found.
   *   bool - FALSE if no valid URL was found.
   */
  public static function extractUrlFromJS($string) {
    // Array of items to search for.
    $searches = array(
      'location.replace',
      'location.href',
      'location.assign',
      'location.replace',
      "'location'",
      'location',
      "'href'",
    );

    // Array of starts and ends to try locating.
    $wrappers = array();
    // Provide two elements: the begining and end wrappers.
    $wrappers[] = array('"', '"');
    $wrappers[] = array("'", "'");

    foreach ($searches as $search) {
      foreach ($wrappers as $wrapper) {
        $url = self::peelUrl($string, $search, $wrapper[0], $wrapper[1]);
        if (!empty($url)) {
          return $url;
        }
      }
    }
    return FALSE;
  }

  /**
   * Searches $haystack for a prelude string then returns the next url found.
   *
   * @param string $haystack
   *   The html string to search through.
   * @param string $prelude_string
   *   The text that appears before the url for a redirect.
   * @param string $wrapper_start
   *   The first part that the url is wrapped in: " ' [ (.
   * @param string $wrapper_end
   *   The last part that the url is wrapped in: " ' } ).
   *
   * @return mixed
   *   string - The valid URL found.
   *   bool - FALSE if no valid URL is found.
   */
  public static function peelUrl($haystack, $prelude_string, $wrapper_start, $wrapper_end) {
    $wrapped = preg_split("/{$prelude_string}/i", $haystack, -1, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);
    // If something was found there will be > 1 element in the array.
    if (count($wrapped) > 1) {
      $found = $wrapped[1];
      $start_location = stripos($found, $wrapper_start);
      // Lets set a limit to how far this will search from the $prelude_string.
      // Anything more than 75 characters ahead is risky.
      $start_location = ($start_location < 75) ? $start_location : FALSE;
      // Account for the length of the start wrapper.
      $start_location = ($start_location !== FALSE) ? $start_location + strlen($wrapper_start) : FALSE;
      // Offset the search for the end, so the start does not get found x2.
      $end_location = ($start_location !== FALSE) ? stripos($found, $wrapper_end, $start_location) : FALSE;
      // Need both a start and end to grab the middle.
      if (($start_location !== FALSE) && ($end_location !== FALSE) && ($end_location > $start_location)) {
        $url = substr($found, $start_location, $end_location - $start_location);
        $url = \MigrationTools\StringTools::superTrim($url);
        // Make sure we have a valid URL.
        if (!empty($url) && filter_var($url, FILTER_VALIDATE_URL)) {
          return $url;
        }
      }
    }
    return FALSE;
  }
}
