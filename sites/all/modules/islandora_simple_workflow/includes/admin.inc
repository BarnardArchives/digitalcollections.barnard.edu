<?php

/**
 * @file
 * Admin form.
 */

/**
 * Admin settings form for Islandora Simple Workflow configuration.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_simple_workflow_admin_form(array $form, array &$form_state) {
  $form['islandora_simple_workflow_toggle_parent_column'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Parent column in Inactive Object table'),
    '#default_value' => variable_get('islandora_simple_workflow_toggle_parent_column', FALSE),
    '#description' => t('Enabling this will provide helpful links back to the parent object, but could incur a performance hit in repositories with many inactive objects.'),
  );

  return system_settings_form($form);
}
