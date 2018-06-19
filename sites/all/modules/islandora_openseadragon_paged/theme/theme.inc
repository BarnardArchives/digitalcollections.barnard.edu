<?php

/**
 * @file
 * Theme functions for islandora_openseadragon_paged.
 * 
 * @TODO: Consider using normal OSD and hooking, passing in desired settings.
 * Last update 2018-06-19, br2490 - updated OSD paged for Islandora versions >1.09. Still uses deprecated calls.
 */

/**
 * Implements hook_preprocess_theme().
 */
function template_preprocess_islandora_paged_seadragon_viewer(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/authtokens');
  module_load_include('inc', 'islandora_openseadragon', 'includes/utilities');

  $pages = $variables['pages'];
  $library_path = libraries_get_path('openseadragon');
  $module_path = drupal_get_path('module', 'islandora_openseadragon');

  $primary_pid = $variables['object']->id;

  // Populate tileSources with pages.
  $identifiers = array();
  foreach ($pages as $pid => $page) {
    // @NOTE: islandora_openseadragon_identifier_tile_source() is deprecated, see Islandora OSD v1.11, inc/utilies.inc.
    $tile_source = islandora_openseadragon_identifier_tile_source(
      url("islandora/object/{$pid}/datastream/JP2/view", array(
        'absolute' => TRUE,
        'query' => array(
          'token' => islandora_get_object_token($pid, 'JP2', 2),
        ),
      )
    ));
    $tile_source['overlays'] = islandora_openseadragon_viewer_query_solr_for_overlays($pid);
    $variables['tile_sources'][] = $tile_source;
  }
  ksort($identifiers);

  $variables['pid'] = $primary_pid;
  $variables['id'] = $variables['viewer_id'] = 'islandora-openseadragon';
  $variables['settings'] = array(
    'pid' => $variables['pid'],
    'imageServer' => variable_get('islandora_openseadragon_tilesource', 'djatoka'),
    'djatokaServerBaseURL' => url(variable_get('islandora_openseadragon_djatoka_url', 'adore-djatoka/resolver'),
      array('absolute' => TRUE)
    ),
    'iiifServerBaseURL' => url(variable_get('islandora_openseadragon_iiif_url', 'iiif')),
    // Some additional settings to pass to the OpenSeadragon viewer... not sure if this is necessary anymore.
    'options' => array(
      'id' => $variables['id'],
      'prefixUrl' => file_create_url("{$library_path}/images/"),
      'tileSources' => $variables['tile_sources'],
      'sequenceMode' => true,
      // 'preserveViewport' => true,
      'showReferenceStrip' => true,
      'referenceStripSizeRatio' => '0.15',
    ) + islandora_openseadragon_get_settings()
  );

  $variables['viewer'] = _openseadragon_paged_viewer($variables);
}

/**
 * Generates OpenSeadragon viewer output and accompanying JavaScript and CSS.
 * 
 * We reimplement this in order to pass in custom settings via
 * $variables['settings'].
 *
 * @see islandora_openseadragon_preprocess_islandora_openseadragon_viewer()
 */
function _openseadragon_paged_viewer($variables) {
  module_load_include('inc', 'islandora_openseadragon', 'includes/utilities');
  $library_path = libraries_get_path('openseadragon');
  $module_path = drupal_get_path('module', 'islandora_openseadragon');
  
  drupal_add_js(array(
    'islandoraOpenSeadragon' => $variables['settings'],
  ), 'setting');

  drupal_add_js("$library_path/openseadragon.js", array('weight' => -4));
  if (islandora_openseadragon_use_djatoka_server()) {
    drupal_add_js("$module_path/js/djtilesource.js", array('weight' => -3));
  }
  drupal_add_js("$module_path/js/islandora_openseadragon.js", array('weight' => -2));
  drupal_add_css("$module_path/css/islandora_openseadragon.theme.css");

  return "<div id=\"{$variables['id']}\" class=\"islandora-openseadragon\"></div>";
}
