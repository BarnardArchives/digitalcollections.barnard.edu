<?php
/**
 * @file
 * @TODO Description
 */

// Loads our content structure: fields, instances, variables.
module_load_include('inc', 'islandora_solr_content_type', 'includes/content
.config');

/**
 * Implements hook_form().
 */
function islandora_solr_content_type_config($form, &$form_state) {

  $form['#attached'] = array(
    'js' => array(
      drupal_get_path('module', 'islandora_solr_content_type') .
      '/js/admin.form.js',
    ),
  );

  $form['comment_islandora_solr_content_type'] = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#options' => array(
      0 => t('Disable and disallow comments by default. (default)'),
      1 => t('Enable and allow comments by default.'),
    ),
    '#title' => t('Default to use for <em>Comments</em> when creating
a new ISCT.'),
    '#description' => t('<strong>Note:</strong> This can be overriden on each
individual page.'),
    '#default_value' => variable_get('comment_islandora_solr_content_type',
      '0'),
  );

  $form['node_submitted_islandora_solr_content_type'] = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#options' => array(
      0 => t('Do <strong>not</strong> diplay authorship and timestamp on pages. (default)'),
      1 => t('Display authorship and timestamp on pages.'),
    ),
    '#title' => t('Hide authorship and timestamp on all ISCT pages.'),
    '#default_value' => variable_get('node_submitted_islandora_solr_content_type',
      '0'),
  );

  $form['pathauto_node_islandora_solr_content_type_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Automatic Path Setting'),
    '#default_value' => variable_get('pathauto_node_islandora_solr_content_type_pattern',
      'collections/[node:title]'),
  );

  $form['isct_menu_by_default'] = array(
    '#type' => 'radios',
    '#options' => array(
      1 => t('Enabled'),
      0 => t('Disabled'),
    ),
    '#title' => t('By default add new Islandora Solr Content to a
    menu?'),
    '#description' => t('This can be override by anyone with access
    administer  menu.'),
    '#default_value' => variable_get('isct_menu_by_default', '0'),
  );

  $menu_options = menu_get_menus();
  $type = $form['#node_type'];

  $form['menu_options_islandora_solr_content_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available menus'),
    '#default_value' => variable_get('menu_options_islandora_solr_content_type',
      array('main-menu')),
    '#options' => $menu_options,
    '#description' => t('The menus available to place links in for this
    content type.'),
    '#states' => array(
      'invisible' => array(
        'input[name="isct_menu_by_default"]' => array(
          'value' => 0,
        ),
      ),
    ),
  );

  $options = menu_parent_options(menu_get_menus(), array('mlid' => 0));
  $form['menu_parent_islandora_solr_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Default parent item'),
    '#default_value' => variable_get('menu_parent_islandora_solr_content_type',
      'main-menu:0'),
    '#options' => $options,
    '#description' => t('Choose the menu item to be the default parent for
    a new link in the content authoring form.'),
    '#attributes' => array('class' => array('menu-title-select')),
    '#states' => array(
      'invisible' => array(
        'input[name="isct_menu_by_default"]' => array(
          'value' => 0,
        ),
      ),
    ),
  );

  // Remove this later.
  $form['isct_development_mode'] = array(
    '#type' => 'radios',
    '#options' => array(
      1 => t('Enabled'),
      0 => t('Disabled'),
    ),
    '#title' => t('Development mode.'),
    '#default_value' => variable_get('isct_development_mode', '0'),
  );
  // Remove this later.
  $form['update_isct_module'] = array(
    '#type' => 'submit',
    '#value' => t('Update Module (development)'),
    '#submit' => array('islandora_solr_content_type_update_custom_type'),
    '#states' => array('invisible' => array('input[name="isct_development_mode"]' => array('value' => 0))),
  );

  // Call Drupal.isct_menu_update_parent_list() to filter the list of
  // available default parent menu items based on the selected menus.
  drupal_add_js('(function ($) { Drupal.isct_menu_update_parent_list(); })
  (jQuery);', array('scope' => 'footer', 'type' => 'inline'));

  return system_settings_form($form);
}

// vim: set filetype=php:
