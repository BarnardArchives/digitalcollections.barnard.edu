<?php

/**
 * @file
 * Theme functions for islandora_openseadragon_paged.
 */

/**
 * Implements hook_preprocess_theme().
 */
function template_preprocess_islandora_paged_seadragon_viewer(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/authtokens');

  $output = '';
  $object = $variables['object'];
  $pages = $variables['pages'];
  $identifiers = array();
  // Populate $identifiers.
  foreach ($pages as $pid => $page) {
    $identifiers[] = url("islandora/object/{$pid}/datastream/JP2/view", array(
      'absolute' => TRUE,
      'query' => array(
        'token' => islandora_get_object_token($pid, 'JP2', 2),
      ),
    ));
  }
  ksort($identifiers);
  // Extra settings to pass to the OpenSeadragon viewer.
  $osd_settings = variable_get('islandora_openseadragon_paged_settings', array(
    'preserveViewport' => 'true',
    'showReferenceStrip' => 'true',
    'referenceStripSizeRatio' => '0.15',
  ));

  $output .= _openseadragon_paged_viewer(array('uri' => $identifiers, 'settings' => $osd_settings));
  $variables['viewer'] = $output;
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
  // Variable fedora_object included in results, but not used.
  $library_path = libraries_get_path('openseadragon');
  $module_path = drupal_get_path('module', 'islandora_openseadragon');
  $variables['viewer_id'] = 'islandora-openseadragon';
  // Here's where we look for additional custom settings.
  $settings = array_merge($variables['settings'] ? $variables['settings'] : array(), array(
    'id' => $variables['viewer_id'],
    'prefixUrl' => file_create_url("$library_path/images/"),
  ));

  module_load_include('inc', 'islandora_openseadragon', 'includes/utilities');
  $installed_version = islandora_openseadragon_get_installed_version();

  if (version_compare($installed_version, '0.9.82') <= 0) {
    // XXX: Version numbers don't quite line-up for the change in the nav image
    // prefix URL, so now, let the hacking begin!... Just for compatibility
    // with older versions.
    $types = array('zoomIn', 'zoomOut', 'home', 'fullpage', 'previous', 'next');
    $actions = array(
      'REST' => 'rest',
      'GROUP' => 'grouphover',
      'HOVER' => 'hover',
      'DOWN' => 'pressed',
    );
    foreach ($types as $type) {
      foreach ($actions as $action => $namepart) {
        $settings['navImages'][$type][$action] = format_string('!type_!action.png', array(
          '!type' => strtolower($type),
          '!action' => $namepart,
        ));
      }
    }
  }

  $settings = array_merge($settings, islandora_openseadragon_get_settings());
  foreach ($settings as $key => $val) {
    if (filter_var($val, FILTER_VALIDATE_FLOAT)) {
      $settings[$key] = (float) $val;
    }
    elseif (filter_var($val, FILTER_VALIDATE_INT)) {
      $settings[$key] = (int) $val;
    }
    elseif (filter_var($val, FILTER_VALIDATE_URL)) {
      $settings[$key] = check_url($val);
    }
  }

  // This is where we jam in some HOCR coordinates.
  if (isset($variables['fedora_object']) && isset($_GET['solr'])) {
    $params = $_GET['solr']['params'];
    $params['fq'][] = format_string('PID:"!pid"', array(
      '!pid' => $variables['fedora_object']->id,
    ));
    module_load_include('inc', 'islandora_paged_content', 'includes/highlighting');
    $highlighting_info = islandora_paged_content_perform_solr_highlighting_query($_GET['solr']['query'], $params);
    $highlights = array();

    foreach ($highlighting_info as $pid => $info) {
      if (array_key_exists('snippets', $info)) {
        foreach ($info['snippets'] as $snippet => $bounding_boxes) {
          foreach ($bounding_boxes as $bounding_box) {
            // The size on the x axis is used to calculate the proportional
            // values under the openseadragon coordinate system.
            $highlights[] = array(
              'x' => $bounding_box['left'] / $info['page']['width'],
              'y' => $bounding_box['top'] / $info['page']['width'],
              'width' => ($bounding_box['right'] - $bounding_box['left']) / $info['page']['width'],
              'height' => ($bounding_box['bottom'] - $bounding_box['top']) / $info['page']['width'],
              'className' => "islandora-openseadragon-highlight",
            );
          }
        }
      }
    }
    $settings['overlays'] = $highlights;
  }

  drupal_add_js(array(
      'islandoraOpenSeadragon' => array(
        'pid' => isset($variables['fedora_object']) ? $variables['fedora_object']->id : NULL,
        'resourceUri' => $variables['uri'],
        'tileSize' => (int) variable_get('islandora_openseadragon_tile_size', '256'),
        'tileOverlap' => (int) variable_get('islandora_openseadragon_tile_overlap', '0'),
        'fitToAspectRatio' => variable_get('islandora_openseadragon_fit_to_aspect_ratio', FALSE),
        'settings' => $settings,
      ),
    ), 'setting');
  drupal_add_js("$library_path/openseadragon.js", array('weight' => -4));
  drupal_add_js("$module_path/js/islandora_openseadragon.js", array('weight' => -3));
  drupal_add_js("$module_path/js/djtilesource.js", array('weight' => -2));
  drupal_add_css("$module_path/css/islandora_openseadragon.theme.css");
  return "<div id=\"{$variables['viewer_id']}\" class=\"islandora-openseadragon\"></div>";
}
