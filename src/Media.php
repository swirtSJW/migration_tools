<?php

namespace Drupal\migration_tools;

use Drupal\Core\Database\Connection;
use Drupal\Core\Entity\EntityTypeManager;
use Drupal\Media\MediaInterface;


/**
 * Support functions related to Media.
 */
class Media {

  /**
   * Create the text used for a media embeds.
   *
   * @param array $mediaData
   *   The data needed to build the media element.  entity_uuid is the only
   *   required element.
   *
   * @return string
   *  Either returns a media embed html or en empty string.
   */
  public static function buildMediaEmbed(array $mediaData) {
    $embedHtml = '';
    $defaultSettings = [
      'alt' => '',
      'title' => '',
      'align' => '',
      'caption' => '',
      'embed_button' => 'media_browser',
      'embed_display' => 'media_image',
      'embed_display_settings' => '',
      'entity_type' => 'media',
      'entity_uuid' => '',
    ];

    $mediaData = array_merge($defaultSettings, $mediaData);

    if (!empty($mediaData['entity_uuid'])) {
      // There is an entity so build the replacement.
      // I don't like hard coding this, but not sure I can depend on a twig
      // template that may not be there.
      // @codingStandardsIgnoreStart
      // <drupal-entity
      $embedHtml .= '<drupal-entity ';
      // Not checked for empty because sometimes 508 needs an empty alt.
      //  alt="I am alternate text"
      $embedHtml .= "alt = \"{$mediaData['alt']}\" ";
      //  title="I am the title"
      $embedHtml .= (!empty($mediaData['title'])) ? "title = \"{$mediaData['title']}\" " : '';
      //  data-align="left"
      $embedHtml .= (!empty($mediaData['align'])) ? "data-align = \"{$mediaData['align']}\" " : '';
      //  data-caption="Some caption"
      $embedHtml .= (!empty($mediaData['caption'])) ? "caption = \"{$mediaData['caption']}\" " : '';
      //  data-embed-button="media_browser"
      $embedHtml .= (!empty($mediaData['embed_button'])) ? "data-embed-button = \"{$mediaData['embed_button']}\" " : '';
      //  data-entity-embed-display="media_image"
      $embedHtml .= (!empty($mediaData['embed_display'])) ? "data-entity-embed-display = \"{$mediaData['embed_display']}\" " : '';
      // Will probably need to do special handling there to encode all the settings.
      //  data-entity-embed-display-settings="{&quot;image_style&quot;:&quot;card&quot;,&quot;image_link&quot;:&quot;file&quot;}"
      $embedHtml .= (!empty($mediaData['embed_display_settings'])) ? "data-entity-embed-display-settings = \"{$mediaData['embed_display_settings']}\" " : '';
      //  data-entity-type="media"
      $embedHtml .= (!empty($mediaData['entity_type'])) ? "data-entity-type = \"{$mediaData['entity_type']}\" " : '';
      //  data-entity-uuid="5822ceac-75bf-4ad7-b030-e6700a8a2398">
      $embedHtml .= "data-entity-uuid = \"{$mediaData['entity_uuid']}\" >";
      $embedHtml .= "</drupal-entity>";
      // @codingStandardsIgnoreEnd
    }

    return $embedHtml;
  }

  /**
   * Get the uuid for a media item if the media item exists.
   *
   * @param string $media_id
   *   A media ide to look up.
   *
   * @return string
   *   The UUID for for the media entity or empty string if none found.
   */
  public static function getMediaUuidfromMid($media_id) {
    $uuid = '';
    if (!empty($media_id)) {
      // Get a media storage object.
      $media_storage = \Drupal::EntityTypeManager()->getStorage('media');
      // Load a single media.
      $media = $media_storage->load($media_id);
      if ($media instanceof MediaInterface) {
        $uuid = $media->get('uuid')->value;
      }
    }
    return $uuid;
  }



  public static function lookupMediaByRedirect($href) {
    $mid = '';
    // Lookup the path in the redirect to see if one exists.
    // if it exists, determine the mid of the destination.

    return $mid;
  }

  }
