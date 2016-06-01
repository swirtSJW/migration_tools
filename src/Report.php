<?php
/**
 * @file
 * Tools for handling reports on migration.
 */

namespace MigrationTools;

class Report {


  /**
   * Output a report of the newly created entity alias.
   *
   * @param object $entity
   *   A node entity to report on.
   */
  public static function entityAlias($entity) {
    if (!empty($entity->nid) && is_numeric($entity->nid)) {
      // Pathing is not updated during entity save,  Need to load it again.
      $temp_entity = node_load($entity->nid);
      if (!empty($temp_entity->path)) {
        if (empty($temp_entity->path['pathauto']) && !empty($temp_entity->path['alias'])) {
          \MigrationTools\Message::make("Alias (custom): @alias", array('@alias' => $temp_entity->path['alias']), FALSE, 1);
        }
        elseif (!empty($temp_entity->path['pathauto']) && !empty($temp_entity->path['alias'])) {
          \MigrationTools\Message::make("Alias (pathauto): @alias", array('@alias' => $temp_entity->path['alias']), FALSE, 1);
        }
        else {
          \MigrationTools\Message::make("Alias: none created", array(), FALSE, 1);
        }
      }
    }
  }
}
