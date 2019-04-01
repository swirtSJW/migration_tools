<?php

namespace Drupal\migration_tools;

use Drupal\migrate\MigrateException;


abstract class Redirect {
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
    // @todo D8 Refactor
    // Gather existing redirects from legacy.
    $row->redirects = \Database::getConnection($db_reference_name, $source_connection)
      ->select('path_redirect', 'r')
      ->fields('r', ['source'])
      ->condition('redirect', "node/$row->nid")
      ->execute()
      ->fetchCol();
  }


  /**
   * Generates a drupal-centric URI based in the redirect corral.
   *
   * @param string $pathing_legacy_directory
   *   (optional) The directory housing the migration source.
   *   ex: If var/www/migration-source/oldsite, then 'oldsite' is the directory.
   * @param string $pathing_redirect_corral
   *   (optional) The fake directory used for corralling the redirects.
   *   ex: 'redirect-oldsite'.
   *
   * @var string $this->corralledUri
   *   Created property.
   *   ex: redirect-oldsite/section/blah/index.html
   */
  public function generateCorralledUri($pathing_legacy_directory = '', $pathing_redirect_corral = '') {
    // Allow the parameters to override the property if provided.
    $pathing_legacy_directory = (!empty($pathing_legacy_directory)) ? $pathing_legacy_directory : $this->legacyDirectory;
    $pathing_redirect_corral = (!empty($pathing_redirect_corral)) ? $pathing_redirect_corral : $this->redirectCorral;
    $uri = ltrim($this->fileId, '/');
    // Swap the pathing_legacy_directory for the pathing_redirect_corral.
    $uri = str_replace($pathing_legacy_directory, $pathing_redirect_corral, $uri);
    $this->corralledUri = $uri;
  }



  /**
   * Creates a redirect from a legacy path if one does not exist.
   *
   * @param string $source_path
   *   The path or url of the legacy source. MUST be INTERNAL to this site.
   *   Ex: redirect-oldsite/section/blah/index.html,
   *   https://www.this-site.com/somepage.htm
   *   http://external-site.com/somepate.htm [only if external-site.com is in
   *   the allowed hosts array].
   * @param string $destination
   *   The destination of the redirect Ex:
   *   node/123
   *   swapped-section-a/blah/title-based-thing
   *   http://www.some-other-site.com.
   * @param string $destination_base_url
   *   Destination base URL.
   * @param array $allowed_hosts
   *   If passed, this will limit redirect creation to only urls that have a
   *   domain present in the array. Others will be rejected.
   *
   * @return bool
   *   FALSE if error.
   */
  public static function createRedirect($source_path, $destination, $destination_base_url, array $allowed_hosts = []) {
    // @todo D8 Refactor
    $alias = $destination;

    // We can not create a redirect for a URL that is not part of the domain
    // or subdomain of this site.
    if (!self::isAllowedDomain($source_path, $allowed_hosts, $destination_base_url)) {
      $message = "A redirect was NOT built for @source_path because it is not an allowed host.";
      $variables = [
        '@source_path' => $source_path,
      ];
      Message::make($message, $variables, FALSE, 2);
      return FALSE;
    }

    if (!empty($source_path)) {
      // Alter source path to remove any externals.
      $source_path = self::fixSchemelessInternalUrl($source_path, $destination_base_url);
      $source = parse_url($source_path);
      $source_path = (!empty($source['path'])) ? $source['path'] : '';
      // A path should not have a preceeding /.
      $source_path = ltrim($source['path'], '/');
      $source_options = [];
      // Check for fragments (after #hash ).
      if (!empty($source['fragment'])) {
        $source_options['fragment'] = $source['fragment'];
      }
      // Check for query parameters (after ?).
      if (!empty($source['query'])) {
        parse_str($source['query'], $query);
        $source_options['query'] = $query;
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
          $variables = [
            '@source' => $source_path,
            '@destination' => $redirect->redirect,
          ];
          Message::make($message, $variables, FALSE, 1);
        }
        else {
          // The redirect already exists.
          $message = 'The redirect of @legacy already exists pointing to @alias. A new one was not created.';
          $variables = [
            '@legacy' => $source_path,
            '@alias' => $redirect->redirect,
          ];
          Message::make($message, $variables, FALSE, 1);
        }
      }
      else {
        // The source and destination are the same. So no redirect needed.
        $message = 'The redirect of @source have idential source and destination. No redirect created.';
        $variables = [
          '@source' => $source_path,
        ];
        Message::make($message, $variables, FALSE, 1);
      }
    }
    else {
      // The is no value for redirect.
      $message = 'The source path is missing. No redirect can be built.';
      $variables = [];
      Message::make($message, $variables, FALSE, 1);
    }
    return TRUE;
  }

  /**
   * Creates multiple redirects to the same destination.
   *
   * This is typically called within the migration's complete().
   *
   * @param array $redirects
   *   The paths or URIs of the legacy source. MUST be INTERNAL to this site.
   *   Ex: redirect-oldsite/section/blah/index.html,
   *   https://www.this-site.com/somepage.htm
   *   http://external-site.com/somepate.htm [only if external-site.com is in
   *   the allowed hosts array].
   * @param string $destination
   *   The destination of the redirect Ex:
   *   node/123
   *   swapped-section-a/blah/title-based-thing
   *   http://www.some-other-site.com.
   * @param string $destination_base_url
   *   Destination base URL.
   * @param array $allowed_hosts
   *   If passed, this will limit redirect creation to only urls that have a
   *   domain present in the array. Others will be rejected.
   */
  public static function createRedirectsMultiple(array $redirects, $destination, $destination_base_url, array $allowed_hosts = []) {
    foreach ($redirects as $redirect) {
      if (!empty($redirect)) {
        self::createRedirect($redirect, $destination, $destination_base_url, $allowed_hosts);
      }
    }
  }

  /**
   * Deletes any redirects associated files attached to an entity's file field.
   *
   * @param object $entity
   *   The fully loaded entity.
   * @param string $field_name
   *   The machine name of the attachment field.
   * @param string $language
   *   Optional. Defaults to LANGUAGE_NONE.
   */
  public static function rollbackAttachmentRedirect($entity, $field_name, $language = '') {
    // @todo D8 Refactor
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
   * @param array $source_urls
   *   A flat array of source urls that should redirect to the attachments
   *   on this entity. $source_urls[0] will redirect to the first attachment,
   *   $entity->$field_name[$language][0], and so on.
   * @param string $field_name
   *   The machine name of the attachment field.
   * @param string $language
   *   Optional. Defaults to LANGUAGE_NONE.
   */
  public static function createAttachmentRedirect($entity, array $source_urls, $field_name, $language = LANGUAGE_NONE) {
    // @todo D8 Refactor
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
        // $destination = ltrim($parsed_url['path'], '/');.
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
   * Retrieves server or html redirect of the page if it the destination exists.
   *
   * @param object $row
   *   A row object as delivered by migrate.
   * @param object $query_path
   *   The current QueryPath object.
   * @param array $redirect_texts
   *   (Optional) array of human readable strings that preceed a link to the
   *   New location of the page ex: "this page has move to".
   *
   * @return mixed
   *   string - full URL of the validated redirect destination.
   *   string 'skip' if there is a redirect but it's broken.
   *   FALSE - no detectable redirects exist in the page.
   *
   * @throws \Drupal\migrate\MigrateException
   */
  public static function hasValidRedirect($row, $query_path, array $redirect_texts = []) {
    if (empty($row->pathing->legacyUrl)) {
      throw new MigrateException('$row->pathing->legacyUrl must be defined to look for a redirect.');
    }
    else {
      // Look for server side redirect.
      $server_side = self::hasServerSideRedirects($row->pathing->legacyUrl);
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
   * @param object $query_path
   *   The current QueryPath object.
   * @param array $redirect_texts
   *   (Optional) array of human readable strings that preceed a link to the
   *   New location of the page ex: "this page has move to".
   *
   * @return mixed
   *   string - full URL of the validated redirect destination.
   *   string 'skip' if there is a redirect but it's broken.
   *   FALSE - no detectable redirects exist in the page.
   */
  public static function hasValidHtmlRedirect($row, $query_path, array $redirect_texts = []) {
    $destination = self::getRedirectFromHtml($row, $query_path, $redirect_texts);
    if ($destination) {
      // This page is being redirected via the page.
      // Is the destination still good?
      $real_destination = self::urlExists($destination);
      if ($real_destination) {
        // The destination is good. Message and return.
        $message = "Found redirect in html -> !destination";
        $variables = ['!destination' => $real_destination];
        Message::make($message, $variables, FALSE, 2);

        return $destination;
      }
      else {
        // The destination is not functioning. Message and bail with 'skip'.
        $message = "Found broken redirect in html-> !destination";
        $variables = ['!destination' => $destination];
        Message::make($message, $variables, Message::ERROR, 2);

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
   *   The full URL to a live page.
   *   Ex: https://www.oldsite.com/section/blah/index.html,
   *   https://www.oldsite.com/section/blah/.
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
   * @param object $query_path
   *   The current QueryPath object.
   * @param array $redirect_texts
   *   (Optional) array of human readable strings that preceed a link to the
   *   New location of the page ex: "this page has move to".
   *
   * @return mixed
   *   string - full URL of the redirect destination.
   *   FALSE - no detectable redirects exist in the page.
   */
  public static function getRedirectFromHtml($row, $query_path, array $redirect_texts = []) {
    // Hunt for <meta> redirects via refresh and location.
    // These use only full URLs.
    $metas = $query_path->top()->find('meta');
    foreach (is_array($metas) || is_object($metas) ? $metas : [] as $meta) {
      $attributes = $meta->attr();
      $http_equiv = (!empty($attributes['http-equiv'])) ? strtolower($attributes['http-equiv']) : FALSE;
      if (($http_equiv === 'refresh') || ($http_equiv === 'location')) {
        // It has a meta refresh or meta location specified.
        // Grab the url from the content attribute.
        if (!empty($attributes['content'])) {
          $content_array = preg_split('/url=/i', $attributes['content'], -1, PREG_SPLIT_NO_EMPTY);
          // The URL is going to be the last item in the array.
          $url = trim(array_pop($content_array));
          if (filter_var($url, FILTER_VALIDATE_URL)) {
            // Seems to be a valid URL.
            return $url;
          }
        }
      }
    }

    // Hunt for Javascript redirects.
    // Checks for presence of Javascript. <script type="text/javascript">.
    $js_scripts = $query_path->top()->find('script');
    foreach (is_array($js_scripts) || is_object($js_scripts) ? $js_scripts : [] as $js_script) {
      $script_text = $js_script->text();
      $url = self::extractUrlFromJS($script_text);
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
      $url = self::extractUrlFromJS($content_array[1]);
      if ($url) {
        return $url;
      }
    }

    // Check for human readable text redirects.
    foreach (is_array($redirect_texts) ? $redirect_texts : [] as $i => $redirect_text) {
      // Array of starts and ends to try locating.
      $wrappers = [];
      // Provide two elements: the begining and end wrappers.
      $wrappers[] = ['"', '"'];
      $wrappers[] = ["'", "'"];
      foreach ($wrappers as $wrapper) {
        $body_html = $query_path->top()->find('body')->innerHtml();
        $url = self::peelUrl($body_html, $redirect_text, $wrapper[0], $wrapper[1]);
        if ($url) {
          return $url;
        }
      }
    }
  }

  /**
   * Checks if given URL matches a list of candidates for a default document.
   *
   * @param string $url
   *   The URL to be tested.
   * @param string $destination_base_url
   *   Destination base URL.
   * @param array $candidates
   *   A list of potential document names that could be indexes.
   *   Defaults to "default" and "index".
   *
   * @return mixed
   *   string - The base path if a matching document is found.
   *   bool - FALSE if no matching document is found.
   */
  public static function getRedirectIfIndex($url, $destination_base_url, array $candidates = ["default", "index"]) {
    // Filter through parse_url to separate out querystrings and etc.
    $path = parse_url($url, PHP_URL_PATH);

    // Pull apart components of the file and path that we'll need to compare.
    $filename = strtolower(pathinfo($path, PATHINFO_FILENAME));
    $extension = pathinfo($path, PATHINFO_EXTENSION);
    $root_path = pathinfo($path, PATHINFO_DIRNAME);

    // Test parsed URL.
    if (!empty($filename) && !empty($extension) && in_array($filename, $candidates)) {
      // Build the new implied route (base directory plus any arguments).
      $new_url = self::reassembleURL([
        'path' => $root_path,
        'query' => parse_url($url, PHP_URL_QUERY),
        'fragment' => parse_url($url, PHP_URL_FRAGMENT),
      ], $destination_base_url, FALSE);

      return $new_url;
    }
    // Default to returning FALSE if we haven't exited already.
    return FALSE;
  }
}
