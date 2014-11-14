<?php

/**
 * @file
 * MenuGeneration related classes.
 */

class MenuGenerationParameters {
  private $organization;
  private $uriMenuLocation;
  private $uriLocalBase;
  private $justiceUrl = "http://www.justice.gov";

  /**
   * Constructor.
   */
  public function __construct($organization) {
    $this->organization = $organization;

    // Defaults.
    $this->uriMenuLocation = $this->justiceUrl . "/" . $this->organization;
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
  public function setUriLocalBase($uri_local_base) {
    $this->uriLocalBase = $uri_local_base;
  }

  /**
   * Setter.
   */
  public function setUriMenuLocation($uri_menu_location) {
    $this->uriMenuLocation = $this->justiceUrl . "/" . $uri_menu_location;
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
  public function getUriMenuLocation() {
    return $this->uriMenuLocation;
  }
}

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

    // Set defaults.
    $this->fileName = $this->parameters->getOrganization() . "-menu.txt";
    $this->fileOutputDirectory = DRUPAL_ROOT . "/sites/all/modules/custom/doj_migration/sources";
  }


  /**
   * Generate.
   */
  public function generate() {
    // Generate the file's content.
    $content = $this->engine->generate();
    // drush_doj_migration_debug_output($content);

    $file = $this->fileOutputDirectory . "/" . $this->fileName;

    $fh = fopen($file, 'w') or die("can't open file");
    fwrite($fh, $content);
    fclose($fh);
    drush_doj_migration_debug_output($content);

    return $file;
  }
}

class MenuGeneratorEngineDefault {
  private $parameters;

  private $queryPath;
  private $initialCssSelector = "div.leftnav>ul";

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
   */
  private function getQueryPath() {
    if (!$this->queryPath) {
      require DRUPAL_ROOT . '/sites/all/vendor/querypath/querypath/src/qp.php';
      $html = file_get_contents($this->parameters->getUriMenuLocation());
      $this->queryPath = htmlqp($html);
    }

    return $this->queryPath;
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
   */
  private function recurse($css_selector = NULL, $prefix = "") {
    module_load_include("inc", "doj_migration", "includes/doj_migration");
    if (!isset($css_selector)) {
      $css_selector = $this->initialCssSelector;
    }
    $pre = $prefix;

    drush_doj_migration_debug_output("CSS INITIAL: $css_selector \n");
    drush_doj_migration_debug_output("PRE INTITIAL: $pre \n");

    $query = $this->getQueryPath();

    $elements = $query->find("{$css_selector}>*");
    foreach ($elements as $elem) {
      if ($elem->is("ul")) {
        drush_doj_migration_debug_output('Im in a ul');
        $class_name = doj_migration_random_string();
        $elem->attr('class', $class_name);
        $this->content .= $this->recurse("{$css_selector}>ul.{$class_name}", "{$pre}-");
      }
      if ($elem->is("div")) {
        drush_doj_migration_debug_output('Im in a div');
        $class = $elem->attr('class');
        $this->content .= $this->recurse("{$css_selector}>div.{$class}>ul", "{$pre}-");
      }
      elseif ($elem->is("li")) {
        drush_doj_migration_debug_output('Im in a li');
        $li = $elem;
        $anchors = $li->find('a:nth-child(1)');
        foreach ($anchors as $a) {
          $al = $a->text();
          $uri = $this->normalizeUri($a->attr("href"));
          $line = "{$pre} {$al} {\"url\":\"{$uri}\"}\n";
          drush_doj_migration_debug_output("CSS INNER $al: $css_selector \n");
          drush_doj_migration_debug_output("PRE INNER $al: $pre \n");
          drush_doj_migration_debug_output("LINE: $line");
          $this->content .= $line;
        }
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
   *   The local uri to which the legacy uri is being redirected.
   */
  public function normalizeUri($uri) {
    // @todo This is cheating, this will work with districts, but not
    // generally.
    $base = $this->parameters->getJusticeUrl();

    module_load_include('inc', 'doj_migration', 'includes/doj_migration');
    $legacy_uri = doj_migration_relative_to_absolute_url($uri, $base);

    $legacy_uri = str_replace($this->parameters->getJusticeUrl() . "/", "", $legacy_uri);

    // If the url is pointing to a directory and no an html doc,
    // let's fix that.
    if (strcmp(substr($legacy_uri, -1), "/") == 0) {
      $legacy_uri = "{$legacy_uri}index.html";
    }

    // The index.(html|htm) pages are aliased to the org name.
    if (strcmp($legacy_uri, "{$this->parameters->getOrganization()}/index.html") == 0 ||
      strcmp($legacy_uri, "{$this->parameters->getOrganization()}/index.htm") == 0) {

      $uri = $this->parameters->getOrganization();
    }
    else {
      $uri = doj_migration_legacy_to_uri($legacy_uri);
    }

    return $uri;
  }

  /**
   * Generate.
   */
  public function generate() {
    $this->recurse();
    return $this->content;
  }
}
