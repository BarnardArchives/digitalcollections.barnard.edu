<?php

/**
 * @file
 * Admin form.
 */

/**
 * Islandora Plupload admin form callback.
 */
function islandora_plupload_admin_form($form, $form_state) {
  $form = array();
  $form['islandora_plupload_max_filesize'] = array(
    '#type' => 'textfield',
    '#title' => t('Max file upload size (megabytes)'),
    '#description' => t('Islandora Plupload will bypass PHP settings such as upload_max_filesize'),
    '#default_value' => variable_get('islandora_plupload_max_filesize', '3000'),
  );
  $form['islandora_plupload_chunk_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Plupload chunk size (megabytes)'),
    '#description' => t('This should be lower than post_max_size'),
    '#default_value' => variable_get('islandora_plupload_chunk_size', 15),
  );
  return system_settings_form($form);
}
