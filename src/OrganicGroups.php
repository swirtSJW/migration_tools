<?php
/**
 * @file
 * Tools for handling Organic Groups in migrations.
 */

namespace MigrationTools;

class OrganicGroups {

  /**
   * Set the og_ref field_mode to admin so the node will save.
   *
   * @param object $entity
   *   An entity, like  node or term.
   */
  public static function setOgToAdmin(&$entity) {
    // Set OG field_mode to 'admin' so it can save the node even if the user is
    // not in the group.
    // See https://www.drupal.org/node/2399997
    if (!empty($entity->og_group_ref) && !empty($entity->og_group_ref[LANGUAGE_NONE])) {
      foreach ($entity->og_group_ref[LANGUAGE_NONE] as $delta => $value) {
        $entity->og_group_ref[LANGUAGE_NONE][$delta]['field_mode'] = 'admin';
      }
    }
  }
}
