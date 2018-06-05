<?php

/**
 * @file
 * Admin callbacks for Islandora OpenSeadragon Paged.
 */

/**
 * Admin form. Adapted from the islandora_openseadragon module.
 */
function islandora_openseadragon_paged_admin($form, &$form_state) {
  $settings = variable_get('islandora_openseadragon_paged_settings', array());

  $form = array();
  $form['islandora_openseadragon_paged_settings'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  $form['islandora_openseadragon_paged_settings']['preserveViewport'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preserve viewport'),
    '#default_value' => $settings['preserveViewport'],
    '#description' => t('Whether to preserve zoom and pan when moving between pages.'),
  );
  $form['islandora_openseadragon_paged_settings']['showReferenceStrip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show reference strip'),
    '#default_value' => $settings['showReferenceStrip'],
    '#description' => t('Whether to show the reference strip (page thumbnails).'),
  );
  $form['islandora_openseadragon_paged_settings']['referenceStripSizeRatio'] = array(
    '#type' => 'textfield',
    '#title' => t('Reference strip size ratio'),
    '#default_value' => $settings['referenceStripSizeRatio'],
    '#description' => t('The size of the reference strip as a fraction of the viewport size.'),
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return system_settings_form($form);
}
