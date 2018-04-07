<?php

namespace Drupal\migration_tools\EventSubscriber;

use Drupal\migrate\MigrateException;
use Drupal\migrate\MigrateSkipRowException;
use Drupal\migrate_plus\Event\MigrateEvents;
use Drupal\migrate_plus\Event\MigratePrepareRowEvent;
use Drupal\migration_tools\Message;
use Drupal\migration_tools\Modifier\DomModifier;
use Drupal\migration_tools\Modifier\SourceModifierHtml;
use Drupal\migration_tools\Obtainer\Job;
use Drupal\migration_tools\SourceParser\HtmlBase;
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
   * @throws \Drupal\migrate\MigrateSkipRowException|\Drupal\migrate\MigrateException
   */
  public function onMigratePrepareRow(MigratePrepareRowEvent $event) {
    $row = $event->getRow();

    $migration_tools_settings = $row->getSourceProperty('migration_tools');

    // Begin processing migration tools settings.
    if (!empty($migration_tools_settings)) {
      $path = '';

      foreach ($migration_tools_settings as $migration_tools_setting) {
        if ($row->getIdMap() && !$row->needsUpdate()) {
          // Row is already imported, don't run any more logic.
          return;
        }

        $source = $migration_tools_setting['source'];
        $source_type = $migration_tools_setting['source_type'];

        switch ($source_type) {
          case 'url':
            $url = $row->getSourceProperty($source);

            // @todo Improve URL fetching.
            $handle = curl_init($url);
            curl_setopt($handle, CURLOPT_RETURNTRANSFER, TRUE);

            $html = curl_exec($handle);
            $http_response_code = curl_getinfo($handle, CURLINFO_HTTP_CODE);
            curl_close($handle);

            if (!in_array($http_response_code, [200, 301])) {
              $message = sprintf('Was unable to load %s, response code: %d', $url, $http_response_code);
              Message::make($message, [], Message::ERROR);

              throw new MigrateSkipRowException($message);
            }
            $url_pieces = parse_url($url);
            $path = ltrim($url_pieces['path'], '/');

            break;

          case 'html':
            $html = $row->getSourceProperty($source);
            break;

          default:
            throw new MigrateException('Invalid source_type specified');
        }

        // Perform Source Operations.
        $source_operations = $migration_tools_setting['source_operations'];
        if ($source_operations) {
          $source_modifier_html = new SourceModifierHtml($html);
          foreach ($source_operations as $source_operation) {
            $arguments = $source_operation['arguments'] ? $source_operation['arguments'] : [];
            HtmlBase::parseDynamicArguments($arguments, $row->getSource());
            $source_modifier_html->runModifier($source_operation['modifier'], $arguments);
          }
          $html = $source_modifier_html->getContent();
        }

        // Construct Jobs.
        $config_fields = $migration_tools_setting['fields'];

        // Perform DOM Operations.
        $dom_operations = $migration_tools_setting['dom_operations'];

        if (empty($dom_operations)) {
          throw new MigrateException('No dom_operations specified');
        }

        $source_parser = new HtmlBase($path, $html, $row);

        foreach ($dom_operations as $dom_operation) {
          switch ($dom_operation['operation']) {
            case 'get_field':
              // Run Obtainer Jobs on field.
              if ($config_fields) {
                $field_found = FALSE;
                foreach ($config_fields as $field_name => $config_field) {
                  if ($field_name == $dom_operation['field']) {
                    $config_jobs = $config_field['jobs'];
                    if ($config_jobs) {
                      $job = new Job($field_name, $config_field['obtainer']);
                      foreach ($config_jobs as $config_job) {
                        HtmlBase::parseDynamicArguments($config_job['arguments'], $row->getSource());
                        $job->{$config_job['job']}($config_job['method'], $config_job['arguments']);
                        $source_parser->addObtainerJob($job);
                      }
                    }
                    else {
                      throw new MigrateException(t('No jobs specified for field @field', ['@field' => $field_name]));
                    }
                    $source_parser->parse();
                    $field_found = TRUE;
                    break;
                  }
                }
                if (!$field_found) {
                  throw new MigrateException(t('Field @field not configured referenced in dom_operations', ['@field' => $dom_operation['field']]));
                }
              }
              break;

            case 'modifier':
              // Run DOM Modifier on queryPath.
              $dom_modifier = new DomModifier($source_parser->queryPath);
              $arguments = $dom_operation['arguments'] ? $dom_operation['arguments'] : [];
              HtmlBase::parseDynamicArguments($arguments, $row->getSource());

              $dom_modifier->runModifier($dom_operation['modifier'], $arguments);
              break;

            default:
              throw new MigrateException(t('Invalid or empty operation @operation', ['@operation' => $dom_operation['operation']]));
          }
        }
      }
    }
  }

}
