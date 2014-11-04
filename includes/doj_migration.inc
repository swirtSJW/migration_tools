<?php

/**
 * @file
 * Miscellaneous utility functions.
 */

/**
 * Prepares taxonomy term(s) for the specified vocabulary and adds new ones.
 *
 * @param string $legacy_term
 *   The legacy term(s) from the migration source comma delimited if multiple.
 *
 * @param string $vocabulary
 *   The machine name of the vocabulary to save the term to.
 *
 * @return string
 *   The term to be saved at the destination. Multiple terms are separated by
 *   "|" characters.
 */
function doj_migrate_prepare_vocab_terms($legacy_term = '', $vocabulary = '') {
  $new_terms = '';
  $vocabulary = trim($vocabulary);

  if ($legacy_term && $vocabulary) {
    $terms = explode(',', $legacy_term);

    foreach ($terms as $key => $term) {
      // If the term does not exist in $vocabulary, create the new term.
      // Clean the term.
      $term = trim($term);
      $term = html_entity_decode($term, ENT_QUOTES, 'UTF-8');
      // Replace multiple spaces with a space.
      $term = preg_replace('!\s+!', ' ', $term);
      $term = preg_replace('/Ã³/', 'ó', $term);
      $term = mb_convert_encoding($term, "UTF-8", mb_detect_encoding($term, "UTF-8, ISO-8859-1, ISO-8859-15", TRUE));

      if (!empty($term)) {
        if ($vocabulary == 'component') {
          $term = doj_migrate_standardize_component_terms($term);
        }

        if (!taxonomy_get_term_by_name($term, $vocabulary)) {
          watchdog('doj_migration', 'Creating new topic term "@topic" in vocabulary: @vocabulary.', array('@topic' => $term, '@vocabulary' => $vocabulary));
          $vocab = taxonomy_vocabulary_machine_name_load($vocabulary);
          $term_obj = new stdClass();
          $term_obj->name = $term;
          $term_obj->vid = $vocab->vid;
          taxonomy_term_save($term_obj);

          $terms[$key] = $term;
        }
      }
    }

    $new_terms = implode('|', $terms);
  }

  return $new_terms;
}

/**
 * Prepares a string of taxonomy terms for the topic vocabulary.
 *
 * @param string $legacy_term
 *   The legacy term from the migration source. Terms may be separated by
 *   commas.
 *
 * @return string
 *   The term to be saved at the destination. Multiple terms are separated by
 *   "|" characters.
 */
function doj_migrate_prepare_topic_term($legacy_term) {
  $new_terms = '';

  if ($legacy_term) {
    $terms = explode(',', $legacy_term);

    foreach ($terms as $key => $term) {

      // If the term does not exist in topic or component vocabularies, create
      // a new topic term.
      $term = trim($term);

      if (!empty($term)) {
        $term = doj_migrate_standardize_component_terms($term);
        if (!taxonomy_get_term_by_name($term, 'topic')
          && !taxonomy_get_term_by_name($term, 'component')) {
          doj_migration_create_term($term, 'topic');
          $terms[$key] = $term;
        }
      }
    }

    $new_terms = implode('|', $terms);
  }

  return $new_terms;
}


/**
 * Creates a new taxonomy term for a given vocabulary.
 *
 * @param string $term
 *   The human readable value fo the term.
 *
 * @param string $vocab_name
 *   The name of the vocabulary.
 */
function doj_migration_create_term($term, $vocab_name) {
  if (!taxonomy_get_term_by_name($term, $vocab_name)) {
    watchdog('doj_migration', 'Creating new topic term "@term".', array('@term' => $term));
    $vocab = taxonomy_vocabulary_machine_name_load($vocab_name);
    $term_obj = new stdClass();
    $term_obj->name = $term;
    $term_obj->vid = $vocab->vid;
    taxonomy_term_save($term_obj);
  }
}

/**
 * Converts a geocoded point returned by geocoder into an address array.
 *
 * @param obj $point
 *   A geocoded point returned by geocoder.
 *
 * @return array
 *   An associative array containing the formatter address.
 */
function doj_migrate_convert_geocoded_point_to_address($point) {
  $address = array();

  if (!empty($point->data['geocoder_address_components'])) {
    $address_parts = $point->data['geocoder_address_components'];

    foreach ($address_parts as $address_part) {
      if (!empty($address_part->types[0])) {
        $address[$address_part->types[0]] = $address_part->short_name;
      }
    }
  }

  return $address;
}

/**
 * Standardizes terms in the component vocabulary by replacing legacy terms.
 *
 * @param string $legacy_term
 *   A legacy taxonomy term.
 *
 * @return mixed
 *   The new taxonomy term (which may not be different from the legacy term).
 */
function doj_migrate_standardize_component_terms($legacy_term) {
  $replacements = array(
    'Attorney General' => 'Office of the Attorney General',
    'U.S. National Central Bureau (INTERPOL)' => 'Interpol Washington',
    'Executive Office of United States Attorneys' => 'Executive Office of U.S. Attorneys',
    'Office of Community Policing Services' => 'Community Oriented Policing Services (COPS)',
  );

  if (!empty($replacements[$legacy_term])) {
    return $replacements[$legacy_term];
  }

  return $legacy_term;
}

/**
 * Helper function to audit thre redirects of an organization's content.
 *
 * This function checks all of the content for a given organization. The
 * default report (a bunch of print statements) let us know when there is a
 * change during the audit. When a redirect fails it will be logged, and if
 * the redirects are successfully created again that will be logged too. The
 * report might look something like this:
 *
 * 26236:TRUE:1
 * 28191:FALSE:392
 * 29626:FALSE:679
 *
 * The report always shows were we are starting, so node/26236 is the first
 * that is part of the given org, and TRUE tell us that it has a redirect.
 *
 * Then we see that node/28191 does not have a redirect, and that it is the
 * 392nd node in the org.
 *
 * finally, the last event is logged, since we never went back to TRUE, this
 * means that all nodes from the 392nd to the 679th all are missing
 * redirects.
 *
 * If the print_redirect flas is set to TRUE, all of the redirects for the
 * nodes that have them will be printed.
 *
 * @param int $org_id
 *   The nid of an organization node.
 * @param bool $print_redirects
 *   Whether we want to show the redirects or not.
 */
function doj_migration_redirect_check($org_id, $print_redirects = FALSE, $content_type = 'page') {
  // @todo Integrate this kind of functionality with the migrate message
  // system.
  // Get all org nodes.
  $query = new EntityFieldQuery();
  $query->entityCondition("entity_type", "node");
  $query->entityCondition("bundle", $content_type);
  $query->fieldCondition("og_group_ref", "target_id", $org_id);
  $results = $query->execute();

  $r = NULL;
  $print = TRUE;
  $count = 0;
  foreach ($results['node'] as $nid => $info) {
    $count++;
    $node = node_load($nid);
    $uri = entity_uri("node", $node);
    $path = $uri['path'];
    $redirects = redirect_load_multiple(FALSE, array('redirect' => $path));

    if ($print_redirects) {
      foreach ($redirects as $redirect) {
        print_r($redirect->source . "\n");
      }
    }

    if ($r == TRUE && empty($redirects)) {
      $print = TRUE;
    }
    elseif ($r == FALSE && !empty($redirects)) {
      $print = TRUE;
    }

    if ($print) {
      $r = empty($redirects) ? FALSE : TRUE;
      print_r("{$node->nid}:");
      if ($r) {
        print_r("TRUE:{$count}\n");
      }
      else {
        print_r("FALSE:{$count}\n");
      }
      $print = FALSE;
    }
  }
  $r = empty($redirects) ? FALSE : TRUE;
  print_r("{$node->nid}:");
  if ($r) {
    print_r("TRUE:{$count}\n");
  }
  else {
    print_r("FALSE:{$count}\n");
  }
}

/**
 * Decodes all HTML entities, including numeric and hexadecimal ones.
 *
 * @param mixed $string
 *   A string.
 *
 * @return string
 *   decoded HTML.
 */
function doj_migration_html_entity_decode_numeric($string, $quote_style = ENT_COMPAT, $charset = "utf-8") {
  $string = html_entity_decode($string, $quote_style, $charset);
  $string = preg_replace_callback('/&#x([0-9a-fA-F]+)/', 'doj_migration_hex_chr_utf8_callback', $string);
  $string = preg_replace_callback('/&#([0-9]+)/', 'doj_migration_chr_utf8_callback', $string);
  return $string;
}

/**
 * Callback helper.
 */
function doj_migration_hex_chr_utf8_callback($matches = array()) {
  return doj_migration_chr_utf8(hexdec($matches[1]));
}

/**
 * Callback helper.
 */
function doj_migration_chr_utf8_callback($matches = array()) {
  return doj_migration_chr_utf8($matches[1]);
}

/**
 * Multi-byte chr(): Will turn a numeric argument into a UTF-8 string.
 *
 * @param mixed $num
 *   A number.
 *
 * @return string
 *   The char represented by the number.
 */
function doj_migration_chr_utf8($num) {
  if ($num < 128) {
    return chr($num);
  }
  if ($num < 2048) {
    return chr(($num >> 6) + 192) . chr(($num & 63) + 128);
  }
  if ($num < 65536) {
    return chr(($num >> 12) + 224) . chr((($num >> 6) & 63) + 128) . chr(($num & 63) + 128);
  }
  if ($num < 2097152) {
    return chr(($num >> 18) + 240) . chr((($num >> 12) & 63) + 128) . chr((($num >> 6) & 63) + 128) . chr(($num & 63) + 128);
  }
  return '';
}

/**
 * Generate a file to import a menu for an org, by parsing html.
 *
 * The html is acquired from www.justice.gov/org.
 *
 * @param string $organization
 *   The abbreviation of the organization.
 */
function doj_migration_create_menu_file($organization) {
  drush_print_r($organization);

  // Organization can be a specific url to, so lets filenamize it.
  $file_name = str_replace("/", "|", $organization);

  // Get html from the organization in justice.
  $base = "http://www.justice.gov";
  $url = "{$base}/{$organization}";
  $content = file_get_contents($url);

  // Get the html for the left navbar, and parse the menu.
  require DRUPAL_ROOT . '/sites/all/vendor/querypath/querypath/src/qp.php';
  $menu = htmlqp($content);
  $content = doj_migration_generate_menu_file_content($organization, $menu, "div.leftnav>ul");

  // Generate the file.
  $file = DRUPAL_ROOT . "/sites/all/modules/custom/doj_migration/sources/{$file_name}-menu.txt";
  $fh = fopen($file, 'w') or die("can't open file");
  fwrite($fh, $content);
  fclose($fh);
  drush_print_r("Created menu import file at $file");
}

/**
 * Take a ul and parse it to generate a string.
 *
 * The string generated maps to the format needed to import a menu into
 * Drupal.
 *
 * @param string $organization
 *   The organization that this menu will belong to.
 * @param object $tier
 *   An object as returned by htmlqp() that contains a ul representing a menu.
 * @param string $root
 *   The root selector to the outermost ul of the menu.
 * @param string $prefix
 *   This is use during the recursive call to keep track of which level we are
 *   in.
 *
 * @return string
 *   A string with the menu to import.
 */
function doj_migration_generate_menu_file_content($organization, $tier, $root, $prefix = '') {
  $content = "";
  // Get all the li.
  $elements = $tier->find("{$root}>*");
  foreach ($elements as $elem) {
    if ($elem->is("ul")) {
      $class_name = doj_migration_random_string();
      $elem->attr('class', $class_name);
      $content .= doj_migration_generate_menu_file_content($organization, $tier,
        "{$root}>ul.{$class_name}", "{$prefix}-");
    }
    if ($elem->is("div")) {
      $class = $elem->attr('class');
      $content .= doj_migration_generate_menu_file_content($organization, $tier,
        "{$root}>div.{$class}>ul", "{$prefix}-");
    }
    elseif ($elem->is("li")) {
      $li = $elem;
      $anchors = $li->find('a:nth-child(1)');
      foreach ($anchors as $a) {
        $al = $a->text();

        $legacy_uri = $a->attr("href");

        $base = "http://www.justice.gov";
        $url = "{$base}/{$organization}";

        if (substr_count($organization, "/") <= 0) {
          $url .= "/index.html";
        }

        $legacy_uri = doj_migration_relative_to_absolute_url($legacy_uri, $url);
        $legacy_uri = str_replace("http://www.justice.gov/", "", $legacy_uri);

        // If the url is pointing to a directory and no an html doc,
        // let's fix that.
        if (strcmp(substr($legacy_uri, -1), "/") == 0) {
          $legacy_uri = "{$legacy_uri}index.html";
        }

        // The index.(html|htm) pages are aliased to the org name.
        if (strcmp($legacy_uri, "{$organization}/index.html") == 0 ||
          strcmp($legacy_uri, "{$organization}/index.htm") == 0) {

          $uri = $organization;
        }
        else {
          $uri = doj_migration_legacy_to_uri($legacy_uri);
        }
        $content .= "{$prefix} {$al} {\"url\":\"{$uri}\"}\n";
      }
    }
  }
  return $content;
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
function doj_migration_legacy_to_uri($legacy_uri) {
  $redirect = redirect_load_by_source($legacy_uri);
  if ($redirect) {
    $nid = str_replace("node/", "", $redirect->redirect);
    $node = node_load($nid);
    if ($node && array_key_exists('alias', $node->path) && !empty($node->path['alias'])) {
      return $node->path['alias'];
    }

    if ($node) {
      $uri = entity_uri("node", $node);
      if (array_key_exists('path', $uri)) {
        return $uri['path'];
      }
    }
  }
  watchdog("doj_migration", "legacy uri @legacy_uri does not have a node associated with it", array('legacy_uri' => $legacy_uri), WATCHDOG_ALERT);
  return "";
}

/**
 * Generate a random string.
 *
 * @return string
 *   Random string.
 */
function doj_migration_random_string() {
  $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
  $randstring = '';
  for ($i = 0; $i < 10; $i++) {
    $randstring = $characters[rand(0, strlen($characters))];
  }
  return $randstring;
}

/**
 * Convert a relative url to absolute.
 *
 * @param string $rel
 *   Relative url.
 * @param string $base
 *   Base url.
 *
 * @return string
 *   The relative url transformed to absolute.
 */
function doj_migration_relative_to_absolute_url($rel, $base) {
  // Return if already absolute URL.
  if (parse_url($rel, PHP_URL_SCHEME) != '') {
    return $rel;
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
 * Transform the abbreviation into the equivalent one in justice.gov.
 *
 * In the Drupal version of justice, all of the abbreviations for district
 * have been transformed to containt the usao- prefix.
 * Some have even been transformed further. For example can (Northern District
 * of California) has become usao-ndca in Drupal.
 * This function should allow us to easily map the transformed abbreviations
 * to the original ones in justice.gov.
 *
 * @param string $abbreviation
 *   The abbreviation of a district without the
 *
 * @return string
 *   Aliased abbreviation.
 */
function doj_migration_district_abbreviation_alias($abbreviation) {
  // Get rid of the usao- prefix we added to all district abbreviations.
  $abbreviation = str_replace("usao-", "", $abbreviation);

  // This map should contain all of the Drupal abbreviations that have
  // changed beyond the prefix.
  // key: Drupal version of the abbreviation.
  // value: Justice.gov version of the abbreviation.
  $map = array(
    "ndca" => "can",
  );

  // If this abbreviation is in the map, let's return the original version
  // from justice.gov
  if (array_key_exists($abbreviation, $map)) {
    return $map[$abbreviation];
  }

  // Otherwise, removing the prefix is good enough.
  return $abbreviation;
}
