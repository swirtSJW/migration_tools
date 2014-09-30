<?php

/**
 * @file
 * EspanolSpeechSourceParser.
 */

class EspanolSpeechSourceParser extends HtmlToSpeechSpanishSourceParser {
  /**
   * {@inheritdoc}
   */
  public function setTitle() {
    // Get title from .presscontenttitle.
    $title = $this->queryPath->find('.presscontenttitle')->text();
    $title = StringCleanUp::superTrim($title);
    $title = mb_strimwidth($title, 0, 255, "...");
    $title = html_entity_decode($title, ENT_QUOTES, 'UTF-8');

    $this->title = $title;
  }

  /**
   * {@inheritdoc}
   */
  public function setBody() {
    // Get body from .presscontenttext.
    $body = $this->queryPath->find('.presscontenttext')->html();
    $body = StringCleanUp::stripCmsLegacyMarkup($body);

    // Remove common mso style string.
    $bad_string1 = 'style="margin: 0in 0in 0pt; text-indent: 0in; line-height: normal; mso-layout-grid-align: none; mso-outline-level: 1;"';
    $body = str_replace($bad_string1, '', $body);

    $this->body = $body;
  }
}
