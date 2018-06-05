<?php

/**
 * @file
 * Utility functions for Barnard Digital Collections exhibits.
 */

/**
 * Prepares exhibit object data.
 */
function _bc_islandora_prepare_exhibit_objects($node) {
  $object_data = array('objects' => array());
  $obj_themes = array();
  // Create a structured list of exhibit objects.
  foreach ($node->field_object[$node->language] as $i => $object) {
    $object_data['objects'][$i]['description'] = isset($object['node']->field_description[$node->language]) ?
      check_markup($object['node']->field_description[$node->language][0]['value'], $object['node']->field_description[$node->language][0]['format']) :
      '';
    $object_data['objects'][$i]['pid'] = isset($object['node']->field_fedora_pid[$node->language]) ?
      $object['node']->field_fedora_pid[$node->language][0]['value'] :
      '';
    $object_data['objects'][$i]['layout'] = isset($object['node']->field_layout[$node->language]) ?
      _bc_islandora_get_ex_layout($object['node']->field_layout[$node->language][0]['value']) :
      '';
    $obj_theme = $object['node']->field_theme[$node->language][0]['taxonomy_term']->name;
    $object_data['objects'][$i]['theme'] = $obj_theme;
    // If we didn't see this theme yet, add it to the list.
    if (!in_array($obj_theme, $obj_themes)) {
      $obj_themes[] = $obj_theme;
      $object_data['theme_output'] .= l($obj_theme, request_path(), array(
        'attributes' => array(
          'class' => 'theme',
          'data-theme' => $obj_theme,
        ),
        'fragment' => 'page' . strval($i + 1),
      ));
    }
    $images = isset($object['node']->field_images[$node->language]) ?
      $object['node']->field_images[$node->language] :
      array();
    $object_data['objects'][$i]['images'] = array();
    foreach ($images as $j => $fc_item) {
      $fc_id = $fc_item['value'];
      $fc_entity = entity_load('field_collection_item', array($fc_id));
      $object_data['objects'][$i]['images'][$j] = array(
        'uri' => $fc_entity[$fc_id]->field_image[$node->language][0]['uri'],
        'caption' => $fc_entity[$fc_id]->field_caption[$node->language][0]['value'],
        'pid' => $fc_entity[$fc_id]->field_fedora_pid[$node->language][0]['value'],
      );
      if (count($images) > 1) {
        $object_data['objects'][$i]['thumbnail_output'] .= l(theme('image_style', array(
          'style_name' => 'thumbnail',
          'path' => $object_data['objects'][$i]['images'][$j]['uri'],
          'attributes' => array('id' => 'tn' . strval($j + 1)),
        )),
        request_path(),
        array(
          'fragment' => 'page' . strval($i + 1) . '#tn' . strval($j + 1),
          'html' => TRUE,
        ));
      }
    }
  }
  return $object_data;
}

/**
 * Helper function to make exhibit layout field values human-readable.
 */
function _bc_islandora_get_ex_layout($lid) {
  $layout = '';
  switch ($lid) {
    case 1:
      $layout = 'left';
      break;

    case 2:
      $layout = 'right';
      break;

    case 4:
      $layout = 'center';
      break;
  }
  return $layout;
}
