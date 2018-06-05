<?php
/**
 * @file
 * bc_islandora_exhibit_feature.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function bc_islandora_exhibit_feature_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'exhibition';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'exhibition';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Digital Exhibits';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'exhibition-object';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Object (field_object) */
  $handler->display->display_options['relationships']['field_object_nid']['id'] = 'field_object_nid';
  $handler->display->display_options['relationships']['field_object_nid']['table'] = 'field_data_field_object';
  $handler->display->display_options['relationships']['field_object_nid']['field'] = 'field_object_nid';
  $handler->display->display_options['relationships']['field_object_nid']['delta'] = '-1';
  /* Field: Field: PID */
  $handler->display->display_options['fields']['field_fedora_pid']['id'] = 'field_fedora_pid';
  $handler->display->display_options['fields']['field_fedora_pid']['table'] = 'field_data_field_fedora_pid';
  $handler->display->display_options['fields']['field_fedora_pid']['field'] = 'field_fedora_pid';
  $handler->display->display_options['fields']['field_fedora_pid']['relationship'] = 'field_object_nid';
  $handler->display->display_options['fields']['field_fedora_pid']['label'] = '';
  $handler->display->display_options['fields']['field_fedora_pid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_fedora_pid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_fedora_pid']['alter']['text'] = 'view in digital collections';
  $handler->display->display_options['fields']['field_fedora_pid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_fedora_pid']['alter']['path'] = 'islandora/object/[field_fedora_pid]';
  $handler->display->display_options['fields']['field_fedora_pid']['alter']['prefix'] = '(';
  $handler->display->display_options['fields']['field_fedora_pid']['alter']['suffix'] = ')';
  $handler->display->display_options['fields']['field_fedora_pid']['element_label_colon'] = FALSE;
  /* Field: Field: Caption */
  $handler->display->display_options['fields']['field_caption']['id'] = 'field_caption';
  $handler->display->display_options['fields']['field_caption']['table'] = 'field_data_field_caption';
  $handler->display->display_options['fields']['field_caption']['field'] = 'field_caption';
  $handler->display->display_options['fields']['field_caption']['relationship'] = 'field_object_nid';
  $handler->display->display_options['fields']['field_caption']['label'] = '';
  $handler->display->display_options['fields']['field_caption']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_caption']['element_label_colon'] = FALSE;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_object_image']['id'] = 'field_object_image';
  $handler->display->display_options['fields']['field_object_image']['table'] = 'field_data_field_object_image';
  $handler->display->display_options['fields']['field_object_image']['field'] = 'field_object_image';
  $handler->display->display_options['fields']['field_object_image']['relationship'] = 'field_object_nid';
  $handler->display->display_options['fields']['field_object_image']['label'] = '';
  $handler->display->display_options['fields']['field_object_image']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_object_image']['alter']['text'] = '[field_object_image]
<div class="caption">[field_caption]&nbsp;[field_fedora_pid]</div>';
  $handler->display->display_options['fields']['field_object_image']['element_class'] = 'object-image';
  $handler->display->display_options['fields']['field_object_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_object_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_object_image']['settings'] = array(
    'image_style' => '',
    'image_link' => '',
  );
  /* Field: Content: Description */
  $handler->display->display_options['fields']['field_description']['id'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['table'] = 'field_data_field_description';
  $handler->display->display_options['fields']['field_description']['field'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['relationship'] = 'field_object_nid';
  $handler->display->display_options['fields']['field_description']['label'] = '';
  $handler->display->display_options['fields']['field_description']['element_class'] = 'object-description';
  $handler->display->display_options['fields']['field_description']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'exhibition' => 'exhibition',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Thumbnail */
  $handler->display->display_options['fields']['field_exhibition_tn']['id'] = 'field_exhibition_tn';
  $handler->display->display_options['fields']['field_exhibition_tn']['table'] = 'field_data_field_exhibition_tn';
  $handler->display->display_options['fields']['field_exhibition_tn']['field'] = 'field_exhibition_tn';
  $handler->display->display_options['fields']['field_exhibition_tn']['label'] = '';
  $handler->display->display_options['fields']['field_exhibition_tn']['element_class'] = 'exhibition-tn';
  $handler->display->display_options['fields']['field_exhibition_tn']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_exhibition_tn']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_exhibition_tn']['settings'] = array(
    'image_style' => '',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_class'] = 'exhibition-title';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_class'] = 'exhibition-body';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'exhibits';
  $export['exhibition'] = $view;

  return $export;
}
