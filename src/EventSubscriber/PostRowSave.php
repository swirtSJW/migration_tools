<?php

namespace Drupal\migration_tools\EventSubscriber;

use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Language\Language;
use Drupal\migrate\Event\MigratePostRowSaveEvent;
use Drupal\migrate\Event\MigrateEvents;
use Drupal\redirect\Entity\Redirect;
use Drupal\redirect\RedirectRepository;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

/**
 * Modify raw data on import.
 */
class PostRowSave implements EventSubscriberInterface {

  /**
   * Drupal\Core\Entity\EntityTypeManagerInterface definition.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * Drupal\redirect\RedirectRepository definition.
   *
   * @var \Drupal\redirect\RedirectRepository
   */
  protected $redirectRepository;

  /**
   * Drupal\migrate_plus\Plugin\MigrationConfigEntityPluginManager definition.
   *
   * @var \Drupal\migrate_plus\Plugin\MigrationConfigEntityPluginManager
   */
  protected $migrationConfigEntityPluginManager;

  /**
   * The URL of the document to retrieve.
   *
   * @var string
   */
  protected $url;

  /**
   * {@inheritdoc}
   */
  public function __construct(EntityTypeManagerInterface $entity_type_manager, RedirectRepository $redirect_repository) {
    $this->entityTypeManager = $entity_type_manager;
    $this->redirectRepository = $redirect_repository;
  }

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    $events[MigrateEvents::POST_ROW_SAVE] = 'onMigratePostRowSave';
    return $events;
  }

  /**
   * Callback function for prepare row migration event.
   *
   * @param \Drupal\migrate\Event\MigratePostRowSaveEvent $event
   *   The prepare row event.
   *
   * @throws \Drupal\migrate\MigrateSkipRowException
   */
  public function onMigratePostRowSave(MigratePostRowSaveEvent $event) {
    $row = $event->getRow();
    $migration_tools_settings = $row->getSourceProperty('migration_tools');

    if (!empty($migration_tools_settings)) {
      // @todo Currently only supports 1st migration_tools array entry.
      $migration_tools_setting = $migration_tools_settings[0];
      $source_type = $migration_tools_setting['source_type'];
      $source = $migration_tools_setting['source'];

      if (!isset($migration_tools_setting['redirect'])) {
        // Uses older format. Convert them to avoid breaking older version.
        $redirect = [
          'create' => isset($migration_tools_setting['create_redirects']) ? $migration_tools_setting['create_redirects'] : FALSE,
          'preserve_query_params' => isset($migration_tools_setting['redirect_preserve_query_params']) ? $migration_tools_setting['redirect_preserve_query_params'] : FALSE,
          'source_namespace' => isset($migration_tools_setting['redirect_source_namespace']) ? trim($migration_tools_setting['redirect_source_namespace'], '/') :'',
        ];
        $migration_tools_setting['redirect'] = $redirect;
      }
      // Backfill defaults.
      $redirect_defaults = [
        'create' => FALSE,
        'preserve_query_params' => FALSE,
        'source_namespace' => '',
      ];
      $migration_tools_setting['redirect'] = array_merge($redirect_defaults, $migration_tools_setting['redirect']);
      // Clean-up entries.
      $migration_tools_setting['redirect']['source_namespace'] = (!empty($migration_tools_setting['redirect']['source_namespace'])) ? trim($migration_tools_setting['redirect']['source_namespace'], '/') . '/' : '';
      // Save for use elsewhere;
      $row->setSourceProperty('migration_tools', $migration_tools_setting);


      // Create redirects if enabled.
      if ($source_type == 'url' && !empty($source) && $create_redirects) {
        $source_url = $row->getSourceProperty($source);
        $nids = $event->getDestinationIdValues();
        $source_url_pieces = parse_url($source_url);
        $source_path = ltrim($source_url_pieces['path'], '/');
        $source_path = "{$migration_tools_setting['redirect']['source_namespace']}$source_path";
        $source_query = [];

        if ($preserve_query_params) {
          $source_query = isset($source_url_pieces['query']) ? $source_url_pieces['query'] : [];
        }
        $nid = $nids[0];

        // Check if redirect already exists first before creating.
        $matched_redirect = $this->redirectRepository->findMatchingRedirect($source_path, $source_query);
        if (is_null($matched_redirect)) {
          $redirect_storage = $this->entityTypeManager->getStorage('redirect');
          /** @var Redirect $redirect */
          $redirect = $redirect_storage->create();
          $redirect->setSource($source_path, $source_query);
          //@todo Rework assumption that this is a node.
          $redirect->setRedirect('node/' . $nid, $source_query);
          $redirect->setStatusCode(301);
          $redirect->save();
        }
      }
    }
  }

}
