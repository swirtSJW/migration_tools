<?php
/**
 * @file
 * HtmlToSpeechSpanishSourceParser.
 */

class HtmlToSpeechSpanishSourceParser extends HtmlToSpeechSourceParser {
  /**
   * Dates in spanish speeches are a little different.
   */
  protected function setSpeechDate() {
    try {
      $sds = HtmlCleanUp::extractFirstElement($this->queryPath, '.speechdate');
      if (!empty($sds)) {
        $this->speechDate = JusticeBaseMigration::dojMigrationESDateConvertWDMY($sds);
      }
      else {
        watchdog("migration_tools", "{$this->fileId} failed to acquire a date");
      }
    }
    catch(Exception $e) {
      watchdog("migration_tools", "{$this->fileId} failed to acquire a date :error {$e->getMessage()}");
    }
  }
}
