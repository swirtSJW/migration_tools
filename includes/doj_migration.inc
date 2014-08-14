<?php

/**
 * @file
 * Miscellaneous utility functions.
 */

/**
 * Prepares a taxonomy term for the topic vocabulary.
 *
 * @param string $legacy_term
 *   The legacy term from the migration source.
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

          watchdog('doj_migration', 'Creating new topic term "@topic".', array('@topic' => $term));
          $vocab = taxonomy_vocabulary_machine_name_load('topic');
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
function doj_migration_redirect_check($org_id, $print_redirects = FALSE) {
  // @todo Integrate this kind of functionality with the migrate message
  // system.
  // Get all org nodes.
  $query = new EntityFieldQuery();
  $query->entityCondition("entity_type", "node");
  $query->entityCondition("bundle", "page");
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
