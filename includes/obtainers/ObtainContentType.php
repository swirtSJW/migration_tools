<?php
/**
 * @file
 * ObtainContentType.
 */

class ObtainContentType extends Obtainer {
  /**
   * Find IMMEDIATE RELEASE for Press Release.
   */
  protected function findPRImmediateRelease() {
    $body = $this->queryPath->find('body')->first();
    $text = $body->text();
    $needle = 'IMMEDIATE RELEASE';
    return (strpos($text, $needle) !== FALSE) ? 'press_release' : '';
  }

  /**
   * Find IMMEDIATE RELEASE for Press Release.
   */
  protected function findPRClassBreadcrumbPressRelease() {
    $body = $this->queryPath->find('.breadcrumb')->first();
    $text = $body->text();
    $needle = 'Press Release';
    return (strpos($text, $needle) !== FALSE) ? 'press_release' : '';
  }

}
