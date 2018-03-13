<?php

/**
 * @file
 * Class ObtainLinkFile
 *
 * Contains logic for parsing file links in HTML.
 */

namespace Drupal\migration_tools\Obtainer;

/**
 * {@inheritdoc}
 */
class ObtainLinkFile extends ObtainLink {
  /**
   * Find file links in selector contents, put each element in an array.
   *
   * @param string $selector
   *   The selector to find.
   * @param array $file_extensions
   *   (optional) Array of file extensions to include, defaults to all.
   * @param array $domains_to_include
   *   (optional) Array of domains to include.
   *
   * @return array
   *   The array of elements found.
   */
  protected function findFileLinks($selector, $file_extensions = [], $domains_to_include = []) {
    return self::pluckFileLinks($selector, $file_extensions, $domains_to_include, FALSE);
  }

  /**
   * Pluck file links in selector contents, put each element in an array.
   *
   * @param string $selector
   *   The selector to find.
   * @param array $file_extensions
   *   (optional) Array of file extensions to include, defaults to all.
   * @param array $domains_to_include
   *   (optional) Array of domains to include.
   *
   * @param bool $pluck
   *   If TRUE, will pluck elements.
   *
   * @return array
   *   The array of elements found.
   */
  protected function pluckFileLinks($selector, $file_extensions = [], $domains_to_include = [], $pluck = TRUE) {
    $links = parent::findLinks($selector);
    $valid_links = $links;

    if (!empty($links) && (!empty($int_ext) || !empty($file_extensions))) {
      $valid_links = $this->validateLinks($links, $file_extensions, $domains_to_include);
      if ($pluck) {
        foreach ($valid_links as $valid_link) {
          $this->setElementToRemove($valid_link['element']);
        }
      }
    }

    return $valid_links;
  }

  /**
   * Validate links array.
   *
   * @param array $links
   *   Array of links
   * @param array $file_extensions
   *   (optional) Array of file extensions to include, defaults to all.
   * @param array $domains_to_include
   *   (optional) Array of domains to include.
   *
   * @return array
   *   Array containing only valid links.
   */
  protected function validateLinks(&$links, $file_extensions = [], $domains_to_include = []) {
    if ($links) {
      foreach ($links as $key => $link) {
        $extension = strtolower(pathinfo($link['href'], PATHINFO_EXTENSION));
        $link_domain = strtolower(parse_url($link['base_uri'], PHP_URL_HOST));
        if (!in_array($extension, $file_extensions)) {
          unset($links[$key]);
        }
        elseif (!empty($domains_to_include)) {
          if (!in_array($link_domain, $domains_to_include)) {
            unset($links[$key]);
          }
        }
      }
    }

    return $links;
  }
}
