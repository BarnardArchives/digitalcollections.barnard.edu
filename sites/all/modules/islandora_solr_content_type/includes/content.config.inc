<?php
/**
 * @file
 * @TODO Description
 */

/**
 * Function to add our content fields and instances. "C"RUD.
 */
function islandora_solr_content_type_install_custom_type() {
  foreach (_islandora_solr_content_type_installed_fields() as $field) {
    field_create_field($field);
  }
  foreach (_islandora_solr_content_type_instances() as $fieldinstance) {
    field_create_instance($fieldinstance);
  }
  foreach (_islandora_solr_content_type_variables() as $var => $option) {
    variable_set($var, $option);
  }
}

/**
 * Function to UPDATE our content fields and instances. CR"U"D.
 */
function islandora_solr_content_type_update_custom_type($form = NULL,
  &$form_state = NULL) {
  try {
    foreach (_islandora_solr_content_type_installed_fields() as $field) {
      if (!field_info_field($field['field_name'])) {
        field_create_field($field);
      }
      else {
        field_update_field($field);
      }
    }
    foreach (_islandora_solr_content_type_instances() as $fieldinstance) {
      field_update_instance($fieldinstance);
    }
    // Do something about variables.
    drupal_set_message(t('Module update completed.'), 'status');
  }
  catch (Exception $e) {
    drupal_set_message(t('Module update failed.'), 'error');
  }
}

/**
 * Function to remove our conntent fields and instances. CRU"D".
 */
function islandora_solr_content_type_uninstall_custom_type() {
  $fielnd_names = array_keys(_islandora_solr_content_type_installed_fields());
  foreach ($fielnd_names as $field) {
    field_delete_field($field);
  }
  $instances = field_info_instances('node', 'islandora_solr_content_type');
  foreach ($instances as $instance_name => $fieldinstance) {
    field_delete_instance($fieldinstance);
  }
  foreach (array_keys(_islandora_solr_content_type_variables()) as $var) {
    variable_del($var);
  }
}

/**
 * Fields, instances and variables for islandora_solr_content_type.
 *
 * Note that some fields, instances and variable names use the name initialism
 * (ISCT) to avoid the strlen limit on fields and instances.
 */
function _islandora_solr_content_type_variables() {
  $variables = array();

  // No comments please.
  $variables['comment_islandora_solr_content_type'] = '0';

  // No submitted info (i.e. author info and date).
  $variables['node_submitted_islandora_solr_content_type'] = '0';

  // Default state to published, without other options.
  $variables['node_options_islandora_solr_content_type']
    = array('0' => 'status');

  // Auto path generation default.
  $variables['pathauto_node_islandora_solr_content_type_pattern']
    = 'collections/[node:title]';

  // Auto-add to menu.
  $variables['isct_menu_by_default'] = '1';

  // Menu Default.
  $variables['menu_options_islandora_solr_content_type'] = array('main-menu');

  // Parent menu for all if above == 1.
  $variables['menu_parent_islandora_solr_content_type'] = 'main-menu:0';

  return $variables;
}

/**
 * Returns an array of our fields.
 *
 * @return array
 *   An array of fields and their properties.
 */
function _islandora_solr_content_type_installed_fields() {
  $fields = array();

  // "Custom" body field (forced to full view, no summary).
  $fields['isct_body'] = array(
    'translatable' => 1, 'field_name' => 'isct_body', 'module' => 'text',
    'type' => 'text_long', 'settings' => array(), 'active' => 1, 'locked' => 1,
    'cardinality' => 1, 'deleted' => 0,
  );

  // Solr query field.
  $fields['isct_solr_query'] = array(
    'translatable' => 0, 'field_name' => 'isct_solr_query', 'module' => 'text',
    'type' => 'text_long', 'settings' => array(), 'active' => 1, 'locked' => 1,
    'cardinality' => 1, 'deleted' => 0,
  );

  // Solr query field.
  $fields['isct_solr_search_query'] = array(
    'translatable' => 0, 'field_name' => 'isct_solr_search_query',
    'module' => 'text', 'type' => 'text_long', 'settings' => array(),
    'active' => 1, 'locked' => 1, 'cardinality' => 1, 'deleted' => 0,
  );

  // Field: number of results to display per page (rows).
  $fields['isct_query_rows'] = array(
    'translatable' => 1, 'field_name' => 'isct_query_rows',
    'module' => 'number', 'type' => 'number_integer', 'settings' => array(),
    'active' => 1, 'locked' => 1, 'cardinality' => 1, 'deleted' => 0,
  );

  // Field: Islandora Solr search block title.
  $fields['isct_display_search_title'] = array(
    'translatable' => 0, 'field_name' => 'isct_display_search_title',
    'module' => 'text', 'type' => 'text', 'settings' => array(), 'active' => 1,
    'locked' => 1, 'cardinality' => 1, 'deleted' => 0,
  );

  // Field: Islandora Solr facet block title.
  $fields['isct_display_facet_title'] = array(
    'translatable' => 0, 'field_name' => 'isct_display_facet_title',
    'module' => 'text', 'type' => 'text', 'settings' => array(), 'active' => 1,
    'locked' => 1, 'cardinality' => 1, 'deleted' => 0,
  );

  // Field: modify #access of facet block's date_slider.
  $fields['isct_display_dateslider'] = array(
    'translatable' => 0, 'field_name' => 'isct_display_dateslider',
    'module' => 'list', 'type' => 'list_boolean', 'settings' => array(
      'allowed_values' => array(
        0 => '', 1 => '',
      ),
    ), 'active' => 1, 'locked' => 1, 'cardinality' => 1, 'deleted' => 0,
  );

  // Field: modify #access of facet block's date_facet.
  $fields['isct_display_datefacets'] = array(
    'translatable' => 0, 'field_name' => 'isct_display_datefacets',
    'module' => 'list', 'type' => 'list_boolean', 'settings' => array(
      'allowed_values' => array(
        0 => '', 1 => '',
      ),
    ), 'active' => 1, 'locked' => 1, 'cardinality' => 1, 'deleted' => 0,
  );

  // Field: modify #markup of facet block's date_range.
  $fields['isct_display_auto_expand_date'] = array(
    'translatable' => 0, 'field_name' => 'isct_display_auto_expand_date',
    'module' => 'list', 'type' => 'list_boolean', 'settings' => array(
      'allowed_values' => array(
        0 => '', 1 => '',
      ),
    ), 'active' => 1, 'locked' => 1, 'cardinality' => 1, 'deleted' => 0,
  );

  // Field: facets displayed on a collection page.
  $fields['isct_display_facets'] = array(
    'translatable' => 0, 'field_name' => 'isct_display_facets',
    'module' => 'list', 'type' => 'list_text', 'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => 'islandora_solr_content_type_solr_get_facets',
    ), 'active' => 1, 'locked' => 1, 'cardinality' => -1, 'deleted' => 0,
  );

  // Field: featured area for use with an image, caption, that will autohide.
  $fields['isct_featured_content_area'] = array(
    'translatable' => 1, 'field_name' => 'isct_featured_content_area',
    'module' => 'text', 'type' => 'text_long', 'settings' => array(),
    'active' => 1, 'locked' => 1, 'cardinality' => 1, 'deleted' => 0,
  );

  // Field: (bool) hide featured area when user preforms a collection query?
  $fields['isct_featured_topic_hide'] = array(
    'translatable' => 0, 'field_name' => 'isct_featured_topic_hide',
    'module' => 'list', 'type' => 'list_boolean', 'settings' => array(
      'allowed_values' => array(
        0 => '', 1 => '',
      ),
    ), 'active' => 1, 'locked' => 1, 'cardinality' => 1, 'deleted' => 0,
  );

  return $fields;
}

/**
 * Spits out the instances of our fields.
 *
 * @return array
 *   An array of field instances based on our types with additional params.
 */
function _islandora_solr_content_type_instances() {
  $instances = array();
  $t = get_t();

  $instances['isct_body'] = array(
    'bundle' => 'islandora_solr_content_type', 'field_name' => 'isct_body',
    'entity_type' => 'node', 'label' => $t('Collection Summary'),
    'deleted' => 0, 'required' => 0, 'settings' => array(
      'text_processing' => 1, 'user_register_form' => 0,
    ), 'widget' => array(
      'active' => 1, 'module' => 'text', 'type' => 'text_textarea',
      'weight' => 1,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'settings' => array(), 'weight' => 1,
      ),
    ),
  );

  $instances['isct_solr_query'] = array(
    'bundle' => 'islandora_solr_content_type',
    'field_name' => 'isct_solr_query', 'entity_type' => 'node',
    'label' => $t('Solr Collection View Query - Landing Page'),
    'description' => $t('The Solr query that will generate desired 
collection view. <br> An easy way to create this question is to visit the
 <a target="_blank" href="@url">search page</a>
  and start with a basic query. The URL bar will display the encoded query 
  string. This will give you an idea of a query string.<br><strong><em>Note:
   You are not restricted to the Solr search fields available to a site visitor.
   </em></strong><br> Barnard College Examples: <ul> <li>Get all yearbooks by 
   mods title AND only display their cover (by collection membership):
    mods_relatedItem_host_titleInfo_title_ms:"Mortarboard" AND 
    RELS_EXT_isMemberOf_uri_ms:"info:fedora/islandora:bookCollection"</li> 
    <li>Get all photographs using mods: mods_genre_ms:"photographs"</li></ul>',
    array('@url' => $GLOBALS['base_root'] . '/islandora/search')),
    'default_value' => array(
      0 => array(
        'value' => $t('RELS_EXT_isMemberOf_uri_ms:"info:fedora/bc:barnard-literary-magazine"'),
        // This is for dev and should be removed.
      ),
    ), 'deleted' => 0, 'required' => 1, 'settings' => array(
      'text_processing' => 0, 'user_register_form' => 0,
    ), 'widget' => array(
      'active' => 1, 'module' => 'text', 'type' => 'text_textarea',
      'settings' => array('rows' => 2), 'weight' => 2,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'type' => 'hidden',
      ),
    ),
  );

  $instances['isct_solr_search_query'] = array(
    'bundle' => 'islandora_solr_content_type',
    'field_name' => 'isct_solr_search_query', 'entity_type' => 'node',
    'label' => $t('Solr Collection View Query - Search Query'),
    'description' => $t('The Solr query that will generate desired \'collection 
    search results.\' <br>If this field is left blank the module will tokenize 
    the landing page query (field above) which will generate a decent search 
    query .<br>Only use this if you want to modify the Solr query used when the 
    user performs a collection search.<br>Note: tokenize your fields! Anything 
    field ending in "ms" should likely become "mt".<br>Be cautious: this query 
    should be as close to the above query as possible. Otherwise the collection
    view and collection search results will look disparate.'),
    'default_value' => array(), 'deleted' => 0, 'required' => 0,
    'settings' => array(
      'text_processing' => 0, 'user_register_form' => 0,
    ), 'widget' => array(
      'active' => 1, 'module' => 'text', 'type' => 'text_textarea',
      'settings' => array('rows' => 2), 'weight' => 3,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'type' => 'hidden',
      ),
    ),
  );

  $instances['isct_query_rows'] = array(
    'bundle' => 'islandora_solr_content_type',
    'field_name' => 'isct_query_rows', 'entity_type' => 'node',
    'label' => $t('Number of results to display per page'),
    'default_value' => array(0 => array('value' => $t('8'))), 'deleted' => 0,
    'description' => $t('The number of records (rows) to display from the 
    Solr query.'),
    'required' => 1, 'settings' => array(
      'text_processing' => 0, 'user_register_form' => 0,
    ), 'widget' => array(
      'active' => 1, 'module' => 'number', 'settings' => array('size' => 10),
      'type' => 'number', 'weight' => 4,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'type' => 'hidden',
      ),
    ),
  );

  $instances['isct_display_search_title'] = array(
    'bundle' => 'islandora_solr_content_type',
    'field_name' => 'isct_display_search_title', 'entity_type' => 'node',
    'label' => $t('Islandor Solr Search Block Title.'),
    'description' => $t('Title of the Islandora Search block on this collection
     page. Use &lt;none&gt; for blank.'),
    'default_value' => array(array('value' => 'Search this collection')),
    'deleted' => 0, 'required' => 1, 'settings' => array(
      'text_processing' => 0, 'user_register_form' => 0,
    ), 'widget' => array(
      'active' => 1, 'module' => 'text', 'type' => 'text',
      'settings' => array(), 'weight' => 21,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'type' => 'hidden',
      ),
    ),
  );

  $instances['isct_display_facet_title'] = array(
    'bundle' => 'islandora_solr_content_type',
    'field_name' => 'isct_display_facet_title', 'entity_type' => 'node',
    'label' => $t('Islandor Solr Facet Block Title.'),
    'description' => $t('Title of the Islandora Facet block on this collection 
    page. Use &lt;none&gt; for blank.'),
    'default_value' => array(array('value' => '<none>')), 'deleted' => 0,
    'required' => 1, 'settings' => array(
      'text_processing' => 0, 'user_register_form' => 0,
    ), 'widget' => array(
      'active' => 1, 'module' => 'text', 'type' => 'text',
      'settings' => array(), 'weight' => 22,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'type' => 'hidden',
      ),
    ),
  );

  $instances['isct_display_datefacets'] = array(
    'bundle' => 'islandora_solr_content_type',
    'field_name' => 'isct_display_datefacets', 'entity_type' => 'node',
    'label' => $t('Remove <strong>all</strong> date faceting from the Facet block?'),
    'default_value' => array(0 => array('value' => 0)), 'deleted' => 0,
    'required' => 0, 'settings' => array('user_register_form' => 0),
    'widget' => array(
      'active' => 1, 'module' => 'options', 'type' => 'options_onoff',
      'settings' => array('display_label' => 1), 'weight' => 23,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'type' => 'hidden',
      ),
    ),
  );

  $instances['isct_display_dateslider'] = array(
    'bundle' => 'islandora_solr_content_type',
    'field_name' => 'isct_display_dateslider', 'entity_type' => 'node',
    'label' => $t('Remove the date slider region (i.e., date picker) in the Facet block?'),
    'default_value' => array(0 => array('value' => 1)), 'deleted' => 0,
    'required' => 0, 'settings' => array('user_register_form' => 0),
    'widget' => array(
      'active' => 1, 'module' => 'options', 'type' => 'options_onoff',
      'settings' => array('display_label' => 1), 'weight' => 24,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'type' => 'hidden',
      ),
    ),
  );

  $instances['isct_display_auto_expand_date'] = array(
    'bundle' => 'islandora_solr_content_type',
    'field_name' => 'isct_display_auto_expand_date', 'entity_type' => 'node',
    'label' => $t('Automatically expand the date search region in the Facets block?'),
    'default_value' => array(0 => array('value' => 1)), 'deleted' => 0,
    'required' => 0, 'settings' => array('user_register_form' => 0),
    'widget' => array(
      'active' => 1, 'module' => 'options', 'type' => 'options_onoff',
      'settings' => array('display_label' => 1), 'weight' => 25,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'type' => 'hidden',
      ),
    ),
  );

  $instances['isct_display_facets'] = array(
    'bundle' => 'islandora_solr_content_type',
    'field_name' => 'isct_display_facets', 'entity_type' => 'node',
    'type' => 'list_text', 'label' => $t('Facets to display on page'),
    'deleted' => 0, 'required' => 0,
    'settings' => array('user_register_form' => 0), 'tree' => 1,
    'collapsible' => 1, 'widget' => array(
      'active' => 1, 'module' => 'options', 'settings' => array(),
      'type' => 'options_buttons', 'weight' => 26,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'type' => 'hidden',
      ),
    ),
  );

  $instances['isct_featured_topic_hide'] = array(
    'bundle' => 'islandora_solr_content_type',
    'field_name' => 'isct_featured_topic_hide', 'entity_type' => 'node',
    'label' => $t('Hide the featured area when the user preforms a search in the collection?'),
    'default_value' => array(0 => array('value' => 1)), 'deleted' => 0,
    'required' => 0, 'settings' => array('user_register_form' => 0),
    'widget' => array(
      'active' => 1, 'module' => 'options', 'type' => 'options_onoff',
      'settings' => array('display_label' => 1), 'weight' => 31,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'type' => 'hidden',
      ),
    ),
  );

  $instances['isct_featured_content_area'] = array(
    'bundle' => 'islandora_solr_content_type',
    'field_name' => 'isct_featured_content_area', 'entity_type' => 'node',
    'label' => $t('Featured Area Region'), 'deleted' => 0, 'required' => 0,
    'settings' => array(
      'text_processing' => 1, 'user_register_form' => 0,
    ), 'widget' => array(
      'active' => 1, 'module' => 'text', 'type' => 'text_textarea',
      'weight' => 32,
    ), 'display' => array(
      'default' => array(
        'label' => 'hidden', 'settings' => array(), 'weight' => -1,
      ),
    ),
  );

  return $instances;
}

// vim: set filetype=php:
