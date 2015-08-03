<?php

/**
 * @file
 * MenuGeneration related classes.
 */

class MenuGenerationParameters {
  private $organization;
  private $uriMenuLocation;
  private $uriMenuLocationBasePath;
  private $uriLocalBase;
  private $justiceUrl = "http://www.justice.gov";
  public $recurse;
  public $menuCounter;
  public $initialMenuLocation;

  /**
   * Constructor.
   */
  public function __construct($organization) {
    $this->organization = $organization;

    // Defaults.
    $this->uriMenuLocation = $this->justiceUrl . "/" . $this->organization;
    $this->setUriMenuLocationBasePath();

    $this->uriLocalBase = $this->organization;
  }

  /**
   * Setter.
   */
  public function setJusticeUrl($justice_url) {
    $this->justiceUrl = $justice_url;
  }

  /**
   * Setter.
   */
  public function setRecurse($recurse) {
    $this->recurse = ($recurse == 'TRUE' || $recurse === TRUE) ? TRUE : FALSE;
  }

  /**
   * Setter.
   */
  public function setUriLocalBase($uri_local_base) {
    $this->uriLocalBase = $uri_local_base;
  }

  /**
   * Setter.
   */
  public function setUriMenuLocation($uri_menu_location) {
    // If it is not an absolute path, make it one.
    if (stripos($uri_menu_location, '://') === FALSE) {
      $this->uriMenuLocation = $this->justiceUrl . "/" . $uri_menu_location;
    }
    else {
      $this->uriMenuLocation = $uri_menu_location;
    }
    $this->setUriMenuLocationBasePath();
  }

  /**
   * Setter.
   */
  public function setUriMenuLocationBasePath($url = NULL) {
    $url = (empty($url)) ? $this->getUriMenuLocation() : $url;
    // Pick apart the url and remove the file name.
    $parsed_url = parse_url($url);
    if (!empty($parsed_url['path'])) {
      // It has a path, so check for an extension.
      $parsed_path = pathinfo($parsed_url['path']);
      if (!empty($parsed_path['extension'])) {
        // It has an extention so get rid of the filename.extension.
        $needle = $parsed_path['filename'] . '.' . $parsed_path['extension'];
        $url = str_replace($needle, '', $url);
      }
    }

    $this->uriMenuLocationBasePath = $url;
  }

  /**
   * Getter.
   */
  public function getJusticeUrl() {
    return $this->justiceUrl;
  }

  /**
   * Getter.
   */
  public function getOrganization() {
    return $this->organization;
  }

  /**
   * Getter.
   */
  public function getUriLocalBase() {
    return $this->uriLocalBase;
  }

  /**
   * Getter.
   */
  public function getUriMenuLocationBasePath() {
    return $this->uriMenuLocationBasePath;
  }

  /**
   * Getter.
   */
  public function getUriMenuLocation() {
    return $this->uriMenuLocation;
  }
}

/**
 * Class MenuGenerator.
 */
class MenuGenerator {
  private $parameters;
  private $engine;

  private $fileName;
  private $fileOutputDirectory;


  /**
   * Constructor.
   */
  public function __construct(MenuGenerationParameters $parameters, MenuGenerationEngineDefault $engine) {
    $this->parameters = $parameters;
    $this->engine = $engine;
    $this->menu = array();

    // Set defaults.
    $this->fileName = $this->parameters->getOrganization() . "-menu.txt";
    $this->fileOutputDirectory = DRUPAL_ROOT . "/sites/all/modules/custom/migration_tools/sources";
  }


  /**
   * Generate the menu import file.
   */
  public function generate() {
    // Generate the file's content.
    $content = $this->engine->generate();

    $file = $this->fileOutputDirectory . "/" . $this->fileName;
    drush_migration_tools_debug_output($content);

    try {
      $fh = fopen($file, 'w');
      if ($fh) {
        fwrite($fh, $content);
        fclose($fh);
        drush_print("File {$file} was generated.\n");
      }
      else {
        $message = "Error (likely permissions) creating the file: $file\n";
        throw new MigrateException($message);
      }
    }
    catch (Exception $e) {
      drush_print("File creation failed.  Caught exception: " . $e->getMessage() . "\n");
      // Output file to terminal so it is available to use.
      drush_print("The menu file was not generated. Outputting menu to terminal.\n\n");
      drush_print($content);
    }

    return $file;
  }
}

/**
 * Class MenuGeneratorEngineDefault.
 */
class MenuGeneratorEngineDefault {
  protected $parameters;

  protected $queryPath;
  protected $initialCssSelector = "div.leftnav>ul";
  public $menu;

  /**
   * Constructor.
   */
  public function __construct(MenuGenerationParameters $parameters) {
    $this->parameters = $parameters;
  }

  /**
   * Setter.
   */
  public function setInitialCssSelector($initial_css_selector) {
    $this->initialCssSelector = $initial_css_selector;
  }

  /**
   * Get a qp() object.
   *
   * @param string $url
   *   Optional url to load as the new document.
   *
   * @return QueryPath
   *   The query path object.
   */
  protected function getQueryPath($url = NULL) {
    if (!empty($url)) {
      $this->parameters->setUriMenuLocation($url);
      $this->queryPath = NULL;

    }
    if (!$this->queryPath) {
      require_once DRUPAL_ROOT . '/sites/all/vendor/querypath/querypath/src/qp.php';
      $page_uri = $this->parameters->getUriMenuLocation();
      $ch = curl_init($page_uri);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
      curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)");
      $html = curl_exec($ch);
      $response = curl_getinfo($ch, CURLINFO_HTTP_CODE);
      if ($response != 200) {
        drush_migration_tools_debug_output("The page at '$page_uri' returned a status of $response.  You might need to adjust '--menu-location-uri'.");
      }

      curl_close($ch);
      $this->queryPath = htmlqp($html);
      $this->cleanQueryPathHtml();
    }

    return $this->queryPath;
  }

  /**
   * Performs any cleaning on QueryPath object once, upon querypath creation.
   */
  protected function cleanQueryPathHtml() {
    // Intentionally empty, to be overidden.
  }

  /**
   * Returns the text content representing the menu import file.
   *
   * @return string
   *   The text content representing the menu import file.
   * @throws Exception
   *   If there is no menu to be processd.
   */
  protected function getContent() {
    if (empty($this->content)) {
      if (empty($this->menu)) {
        throw new Exception("No menu was found.  You might be targetting the wrong page or selector.");
      }

      // Go get each level 1 page,  process its menu.
      $this->processNextLevelPages();

      // Apply prepended links.
      $this->menu = array_merge($this->getLinkstoPrepend(), $this->menu);

      // Apply appended links.
      $this->menu = array_merge($this->getLinkstoAppend(), $this->menu);
      $menu_elements = count($this->menu);
      $this->content = '';
      $this->menuCounter = 0;
      $this->collapseMenu();
      $this->elementsToContent($this->menu);
    }
    drush_migration_tools_debug_output("Built a menu with {$this->menuCounter} items from $menu_elements elements.");

    return $this->content;
  }

  /**
   * Processes an array of menu elements recursing into the children.
   *
   * Each menu element is written as a line in the menu.txt file to import.
   *
   * @param array $elements
   *   Any array of menu elements.
   */
  protected function elementsToContent(array $elements) {
    foreach ($elements as $element) {
      $this->content .= $element['prefix'] . $element['title'] . "{\"url\":\"" . $element['url'] . "\"}\n";
      $this->menuCounter++;
      if (!empty($element['children'])) {
        $this->elementsToContent($element['children']);
      }
    }
  }

  /**
   * Crawls pages present the primary menu and adds second level menu items.
   */
  protected function processNextLevelPages() {
    if (is_array($this->menu) && $this->parameters->recurse) {
      $org_url = $this->parameters->getUriMenuLocationBasePath();
      $org_path = parse_url($org_url, PHP_URL_PATH);
      foreach ($this->menu as $uri => $item) {
        $in_group = (stripos($uri, $org_path) === FALSE) ? FALSE : TRUE;
        if (empty($item->prefix) && $in_group) {
          // This is a first tier page with a path in this group so process it.
          $query = $this->getQueryPath($uri);
          drush_migration_tools_debug_output("PROCESSING: " . $this->parameters->getUriMenuLocation());
          $this->recurse();
        }
      }
    }
  }

  /**
   * Moves any child elements into the children array of the parent.
   */
  protected function collapseMenu() {
    $prefix_level = $this->getPrefixMax();
    // Must process the deepest children first.
    while ($prefix_level >= 0) {
      foreach ($this->menu as $uri => &$item) {
        if ((strlen($item['prefix']) == $prefix_level) && !empty($item['parent_uri'])) {
          // Copy the item to its parent if the parent exists and
          // it is not already there.
          if ((!empty($this->menu[$item['parent_uri']])) && (empty($this->menu[$item['parent_uri']]['children'][$uri]))) {
            $this->menu[$item['parent_uri']]['children'][$uri] = $item;
            // Remove the original item.
            unset($this->menu[$uri]);
          }
          elseif (empty($this->menu[$item['parent_uri']])) {
            // The parent does not exist.
            // Copy it to the end of the menu as an orphan for hand adjustment.
            if (empty($this->menu['orphans'])) {
              $this->menu['orphans'] = array();
            }
            $this->menu['orphans'][$uri] = $item;
            // Remove the original item.
            unset($this->menu[$uri]);
          }

        }
      }
      // Break the reference.
      unset($item);
      $prefix_level--;
    }
    drush_migration_tools_debug_output('Menu object after collapsing:');
    drush_migration_tools_debug_output($this->menu);
    if (!empty($this->menu['orphans'])) {
      drush_migration_tools_debug_output('Menu orphans present:');
      drush_migration_tools_debug_output($this->menu['orphans']);
    }
    else {
      drush_migration_tools_debug_output('Menu collapsed without orphans.');
    }
  }

  /**
   * Gets the maximum number of prefixes found in the menu.
   *
   * @return int
   *   The maximum number of prefixes found in the menu.
   */
  private function getPrefixMax() {
    $max = 0;
    foreach ($this->menu as $items) {
      $count = strlen($items['prefix']);
      $max = ($count > $max) ? $count : $max;
    }
    return $max;
  }

  /**
   * Recursive function that processes a menu level.
   *
   * @param string $css_selector
   *   The css selector to get the ul we are to process.
   *
   * @param string $prefix
   *   The level of depth we are into represented by dashes. "" level 0, "-"
   *   level 1, and so on.
   * @param string $parent_uri
   *   The uri of the item's parent (optional).
   */
  protected function recurse($css_selector = NULL, $prefix = '', $parent_uri = '') {
    module_load_include("inc", "migration_tools", "includes/migration_tools");
    $last_uri = '';
    if (!isset($css_selector)) {
      // This is the first run through a subpage so get the menu selector.
      $css_selector = $this->initialCssSelector;
    }

    drush_migration_tools_debug_output("{$prefix}CSS SELECTOR: $css_selector child of $parent_uri");
    $query = $this->getQueryPath();
    $elements = $query->find($css_selector)->children();

    foreach ($elements as $elem) {
      $tag = $elem->tag();
      switch ($tag) {
        case 'li':
          drush_migration_tools_debug_output("$prefix I'm in $tag $css_selector child of $parent_uri");
          $children = $elem->children();
          foreach ($children as $child) {
            // Might be an anchor or might be a cluster of items.
            if ($child->tag() == 'a') {
              $this->addMenuElement($child->attr("href"), $child->text(), $prefix, $parent_uri);
              $last_uri = $child->attr("href");
            }
            else {
              $class_name = self::generaterandomstring();
              $elem->attr('class', $class_name);
              $this->recurse(".{$class_name}", $prefix, $last_uri);
            }
          }
          break;

        case 'ul':
        case 'div':
        default:
          $last_uri = (empty($last_uri)) ? $parent_uri : $last_uri;
          $new_prefix = ($tag == 'ul') ? $prefix . "-" : $prefix;
          $class_name = self::generaterandomstring();
          $elem->attr('class', $class_name);
          drush_migration_tools_debug_output("$new_prefix I'm in $tag $class_name child of $last_uri");
          $this->recurse(".{$class_name}", $new_prefix, $last_uri);
          break;
      }
    }
  }

  /**
   * Take uris from justice, and map them to the local uris from migrated pages.
   *
   * @param string $uri
   *   the legacy uri coming from the menu in justice.gov.
   *
   * @return string
   *   The local uri to which the legacy uri is being redirected or full url.
   */
  public function normalizeUri($uri) {
    $uri = $this->cleanPath($uri);

    $schema_and_domain = $this->parameters->getJusticeUrl();
    $local_base_path = $this->parameters->getUriLocalBase();
    $url_group_destination_path = $schema_and_domain . $local_base_path;
    $parsed_url_destination = parse_url($url_group_destination_path);
    $url_menu_source_path = $this->parameters->getUriMenuLocationBasePath();
    $parsed_url_source = parse_url($url_menu_source_path);
    $parsed_url = parse_url($uri);

    // Determine type of path. (relative, root relative, absolute, external).
    $type = '';
    if ((!empty($parsed_url['host'])) && ($parsed_url['host'] != $parsed_url_destination['host'])) {
      $type = 'external';
    }
    elseif (!empty($parsed_url['path'])) {
      if (strpos($parsed_url['path'], '/') === 0) {
        $type = 'root relative';
      }
      else {
        // Covers the case of either './somefile.htm' or 'somefile.htm'.
        $type = 'self-relative';
      }

      if (strpos($parsed_url['path'], '../') === 0) {
        $type = 'relative';
      }
    }

    switch ($type) {
      case 'external':
        // It is external, no further processing is needed.
        return $uri;

      case 'self-relative':
        // Make it relative by removing the inconsequential './'.
        if (strpos($parsed_url['path'], './') === 0) {
          $parsed_url['path'] = substr($parsed_url['path'], 0, 2);
        }
      case 'relative':
        // Make it root relative by prepending the base path of the source.
        $subpath = trim($parsed_url_source['path'], '/');
        $move_ups = substr_count($parsed_url['path'], '../');
        if ($move_ups) {
          $path_array = explode('/', $subpath);
          $items = count($path_array);
          // Remove one path element for each '../' present.
          for ($i = 1; $i <= $move_ups; $i++) {
            $index = $items - $i;
            unset($path_array[$index]);
          }
          if (!empty($path_array)) {
            $subpath = '/' . implode('/', $path_array) . '/';
          }
          else {
            $subpath = '/';
          }
          $parsed_url['path'] = str_replace('../', '', $parsed_url['path']);
        }
        $parsed_url['path'] = rtrim($subpath, '/') . '/' . ltrim($parsed_url['path'], '/');

      case 'root relative':
      case 'absolute':
      default:
        // These cases need no special handling.
        break;
    }

    // If the url is pointing to a directory and not a file, let's fix that.
    if (strcmp(substr($parsed_url['path'], -1), "/") == 0) {
      $parsed_url['path'] = "{$parsed_url['path']}index.html";
      // @TODO Seems brittle, hardcoding just the index.html by default.
      // It would be more reliable attempt to load the variants and use
      // whichever one exists, but slower, so not used for now.
    }

    $group_path = rtrim($this->parameters->initialMenuLocation, '/');
    if (!empty($parsed_url['fragment'])) {
      // The uri contains a # which is not supported by the menu import, so
      // return a full path to ensure no drupal processing.
      $uri = $this->reassembleURL($parsed_url, TRUE);
    }
    elseif (stripos($parsed_url['path'], "{$group_path}/index.html") !== FALSE ||
      stripos($parsed_url['path'], "{$group_path}/index.htm") !== FALSE) {
      // The index.(html|htm) pages are aliased to the group page, so adjust.
      $uri = $this->parameters->getOrganization();
    }
    else {
      // This is an internal uri so see what it redirects to.
      module_load_include('inc', 'migration_tools', 'includes/migration_tools');
      $uri = migration_tools_legacy_to_uri($this->reassembleURL($parsed_url, FALSE));
    }

    return $uri;
  }

  /**
   * Menu getter.
   *
   * @return array
   *   The menu array.
   */
  public function getMenu() {
    return $this->menu;
  }

  /**
   * Generate.
   */
  public function generate() {
    $this->recurse();
    $this->getContent();

    return $this->content;
  }

  /**
   * Any menu items returned will be placed at the top of the menu import.
   *
   * @return array
   *   Menu elements to add to the top of the menu
   */
  public function getLinkstoPrepend() {
    return array();
  }

  /**
   * Any menu items returned will be placed at the bottom of the menu import.
   *
   * @return array
   *   Menu element array to add to the bottom of the menu.
   */
  public function getLinksToAppend() {
    return array();
  }

  /**
   * Cleans the path to make sure paths are consistent.
   *
   * @param string $uri
   *   The uri from the source file.
   *
   * @return string
   *   The cleaned uri.
   */
  public function cleanPath($uri) {
    $uri = trim($uri);
    // If the uri is a path, not ending in a file, make sure it ends in a '/'.
    if (!empty($uri) && !pathinfo($uri, PATHINFO_EXTENSION)) {
      $uri = rtrim($uri, '/');
      $uri .= '/';
    }
    return $uri;
  }

  /**
   * Adds a single menu item to $this->menu.
   *
   * @param string $link_path
   *   The url or uri of the link.
   * @param string $link_title
   *   The text to be used as the link.
   * @param string $prefix
   *   The prefix the title with that indicates the item's depth (optional).
   * @param string $parent_uri
   *   The uri of the menu items parent (optional).
   */
  public function addMenuElement($link_path, $link_title, $prefix = '', $parent_uri = '') {
    $link_path = $this->normalizeUri($link_path);

    if (empty($this->menu[$link_path])) {
      // This menu item does not exist yet so add it.
      $parent_uri = (empty($parent_uri)) ? '' : $this->normalizeUri($parent_uri);
      $this->menu[$link_path] = array(
        'prefix' => $prefix,
        'url' => $link_path,
        'parent_uri' => $parent_uri,
        'title' => $link_title,
        'children' => array(),
      );
      drush_migration_tools_debug_output("ADDED: $link_path to menu");
    }
    else {
      drush_migration_tools_debug_output("ALREADY HAVE: $link_path in menu");
    }
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
  public function reassembleURL($parsed_url, $return_url = TRUE) {
    $url = '';
    if ($return_url) {
      $url .= (!empty($parsed_url['scheme'])) ? $parsed_url['scheme'] . '://' : 'http://';
      $url .= (!empty($parsed_url['host'])) ? $parsed_url['host'] . '/' : $this->parameters->getJusticeUrl() . '/';
    }
    // Trim the initial '/' to be Drupal friendly in the event of no host.
    $url .= (!empty($parsed_url['path'])) ? ltrim($parsed_url['path'], '/') : '';
    $url .= (!empty($parsed_url['query'])) ? '?=' . $parsed_url['query'] : '';
    $url .= (!empty($parsed_url['fragment'])) ? '#' . $parsed_url['fragment'] : '';

    return $url;
  }

  /**
   * Generate a random string.
   *
   * @return string
   *   Random string.
   */
  public static function generateRandomString() {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $randstring = '';
    for ($i = 0; $i < 10; $i++) {
      $randstring .= $characters[rand(0, strlen($characters))];
    }
    return $randstring;
  }
}
