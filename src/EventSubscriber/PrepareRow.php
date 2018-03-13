<?php

namespace Drupal\migration_tools\EventSubscriber;

use Drupal\migrate\MigrateSkipRowException;
use Drupal\migrate_plus\Event\MigrateEvents;
use Drupal\migrate_plus\Event\MigratePrepareRowEvent;
use Drupal\migration_tools\Message;
use Drupal\migration_tools\Obtainer\Job;
use Drupal\migration_tools\SourceParser\Node;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

/**
 * Modify raw data on import.
 */
class PrepareRow implements EventSubscriberInterface {

  /**
   * The URL of the document to retrieve.
   *
   * @var string
   */
  protected $url;

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    $events[MigrateEvents::PREPARE_ROW] = 'onMigratePrepareRow';
    return $events;
  }

  /**
   * Callback function for prepare row migration event.
   *
   * @param \Drupal\migrate_plus\Event\MigratePrepareRowEvent $event
   *   The prepare row event.
   *
   * @throws \Drupal\migrate\MigrateSkipRowException
   */
  public function onMigratePrepareRow(MigratePrepareRowEvent $event) {
    $row = $event->getRow();

    $field_containing_url = $row->getSourceProperty('field_containing_url');
    $field_containing_html = $row->getSourceProperty('field_containing_html');

    // If field_containing_url is set, then we know it should do job processing.
    // @todo Needs better logic to determine when to run the parsing.
    if (!empty($field_containing_url)) {
      $url = $row->getSourceProperty($field_containing_url);

      if (!empty($field_containing_html)) {
        $html = $row->getSourceProperty($field_containing_html);
      }
      else {
        // @todo Improve URL fetching.
        $handle = curl_init($url);
        curl_setopt($handle, CURLOPT_RETURNTRANSFER, TRUE);

        $html = curl_exec($handle);
        $http_response_code = curl_getinfo($handle, CURLINFO_HTTP_CODE);
        curl_close($handle);

        if (!in_array($http_response_code, [200, 301])) {
          $message = 'Was unable to load !url';
          $variables = array('!url' => $url);
          Message::make($message, $variables, Message::ERROR);
          throw new MigrateSkipRowException();
        }

      }

      $url_pieces = parse_url($url);
      $path = ltrim($url_pieces['path'], '/');

      // @todo Using Node parser by default. Should be decided by config.
      $source_parser = new Node($path, $html, $row);

      // Set HTML Element manipulations from config.
      $html_elements_to_remove = $row->getSourceProperty('html_elements_to_remove') ? $row->getSourceProperty('html_elements_to_remove') : [];
      $source_parser->setHtmlElementsToRemove($html_elements_to_remove);

      $html_elements_to_unwrap = $row->getSourceProperty('html_elements_to_unwrap') ? $row->getSourceProperty('html_elements_to_unwrap') : [];
      $source_parser->setHtmlElementsToUnWrap($html_elements_to_unwrap);

      $html_elements_to_rewrap = $row->getSourceProperty('html_elements_to_rewrap') ? $row->getSourceProperty('html_elements_to_rewrap') : [];
      $source_parser->setHtmlElementsToReWrap($html_elements_to_rewrap);

      // Construct Jobs.
      $config_fields = $row->getSourceProperty('fields');
      if ($config_fields) {
        foreach ($config_fields as $config_field) {
          $config_jobs = $config_field['jobs'];
          if ($config_jobs) {
            $job = new Job($config_field['name'], $config_field['obtainer']);
            foreach ($config_jobs as $config_job) {
              $job->{$config_job['job']}($config_job['method'], $config_job['arguments']);
              $source_parser->addObtainerJob($job);
            }
          }
        }
      }

      // Add Modifiers.
      $modifiers = $row->getSourceProperty('modifiers');
      if ($modifiers) {
        foreach ($modifiers as $modifier) {
          $arguments = $modifier['arguments'] ? $modifier['arguments'] : [];
          $source_parser->getModifier()
            ->addModifier($modifier['method'], $arguments);
        }
      }

      $source_parser->parse();
    }
  }
}
