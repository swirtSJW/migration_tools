<?php
/**
 * @file
 * Tools for handling nodes and fields.
 */

class NodeTools {

  /**
   * Converts an array of field values/labels to an array of field keys.
   *
   * @param array $values
   *   An array of values to be converted to keys.
   * @param string $field_name
   *   The machine name of the field to lookup the allowed values.
   *
   * @return array
   *   The array of keys converted from matching $values.
   */
  public static function convertToFieldKeys($values, $field_name) {
    $converted = array();
    if (!empty($field_name) && is_array($values)) {
      $field = field_info_field($field_name);
      $allowed_values = list_allowed_values($field);
      if (!empty($allowed_values)) {
        $allowed_keys = array_flip($allowed_values);
        foreach ($values as $key => $value) {
          if (!empty($allowed_keys[$value])) {
            $converted[] = $allowed_keys[$value];
          }
          else {
            $message = "The value '!value' was not an allowed value of the field '!field' so it was discarded.";
            $vars = array(
              '!value' => $value,
              '!field' => $field_name,
            );
            MigrationMessage::makeMessage($message, $vars, WATCHDOG_NOTICE, 1);
          }
        }
      }
      else {
        $message = "No list of allowed values could be found for the field '!field'.";
        $vars = array('!field' => $field_name);
        MigrationMessage::makeMessage($message, $vars, WATCHDOG_ERROR, 1);
      }

    }
    else {
      $message = "convertToKeys called with either an empty field_name:'!field' or values were not an array \n !values.";
      $vars = array(
        '!field' => $field_name,
        '!values' => print_r($values, TRUE),
      );
      MigrationMessage::makeMessage($message, $vars, WATCHDOG_ERROR, 1);
    }

    return $converted;
  }

  /**
   * Sets the $entity->body filter setting for all body fields of all languages.
   *
   * @param object $entity
   *   Node entity object. Altered by reference
   * @param string $filter_machine_name
   *   The filter machine name to assign. (example: 'full_html')
   */
  public static function reassignBodyFilter($entity, $filter_machine_name) {
    if (!empty($entity->body) && is_array($entity->body)) {
      foreach ($entity->body as &$language) {
        foreach ($language as $lang => &$body) {
          // It is possible that $body is not an array.
          if (is_array($body)) {
            $body['value_format'] = $filter_machine_name;
            $body['format'] = $filter_machine_name;
          }
        }
        // Break the reference.
        unset($body);
      }
      // Break the reference.
      unset($language);
    }
  }
}
