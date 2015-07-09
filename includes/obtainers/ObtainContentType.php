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
    return (stripos($text, $needle) !== FALSE) ? 'press_release' : '';
  }

  /**
   * Find IMMEDIATE RELEASE for Press Release.
   */
  protected function findPRClassBreadcrumbPressRelease() {
    $body = $this->queryPath->find('.breadcrumb')->first();
    $text = $body->text();
    $needle = 'Press Release';
    return (stripos($text, $needle) !== FALSE) ? 'press_release' : '';
  }

  /**
   * Find 'Speeches By The U.S. Attorney' for Speech.
   */
  protected function findClassBreadcrumbSpeech() {
    $body = $this->queryPath->find('.breadcrumb')->first();
    $text = $body->text();
    $needle = 'Speeches By The U.S. Attorney';

    return (stripos($text, $needle) !== FALSE) ? 'speech' : '';
  }
}
