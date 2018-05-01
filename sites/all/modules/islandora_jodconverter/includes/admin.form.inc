<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_jodconverter_admin_settings_form() {
  $form = array();

  $form['islandora_jodconverter_openoffice_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Openoffice Port'),
    '#default_value' => variable_get('islandora_jodconverter_openoffice_port'),
    '#size' => 150,
    '#required' => TRUE,
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Validate the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_jodconverter_admin_settings_form_validate($form, &$form_state) {
  $port = $form_state['values']['islandora_jodconverter_openoffice_port'];
  if (!preg_match('/^[0-9]*$/', $port)) {
    form_error($form['islandora_jodconverter_openoffice_port'], t('port should be a ingeter value'));
  }
}

/**
 * Process the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_jodconverter_admin_settings_form_submit($form, &$form_state) {
  $port = isset($form_state['values']['islandora_jodconverter_openoffice_port']) ? $form_state['values']['islandora_jodconverter_openoffice_port'] : 8100;
  $port = intval($port);
  variable_set('islandora_jodconverter_openoffice_port', $port);
}
