<?php
/**
 * @file
 * The primary PHP file for this theme.
 */

/**
 * Implements hook_preprocess_page().
 */
function barnard_bootstrap_preprocess_page(&$vars) {
  if (isset($vars['node'])) {
    $node = $vars['node'];
    $vars['theme_hook_suggestions'][] = 'page__node__' . $node->type;
  }
  // If we have bc_islandora and this is the front page, invoke
  // _bc_islandora_featured() and set  $vars['page']['footer']['front_caption'].
  if (module_exists('bc_islandora') && $vars['is_front']) {
    module_load_include('inc', 'bc_islandora', 'includes/theme');
    $vars['page']['content']['front_caption'] = array(
      '#markup' => _bc_islandora_featured(),
      '#prefix' => '<div id="featured_image_caption" class="block block-block frontpage-panel well pull-right clearfix">',
      '#suffix' => '</div>',
    );
  }
  // If we have bc_islandora, this is NOT the front page, and this is not a
  // search result page, call bc_islandora's custom breadcrumb theming method
  // and set $vars['bc_breadcrumb'].
  if (isset($node) && $node->type == 'islandora_solr_content_type') {
    $vars['bc_breadcrumb'] = theme('bc_islandora_breadcrumb', array('breadcrumb' => menu_get_active_breadcrumb()));
  }
  elseif (module_exists('bc_islandora') && !$vars['is_front'] && arg(1) != 'search') {
    $vars['bc_breadcrumb'] = theme('bc_islandora_breadcrumb', array('breadcrumb' => array()));
  }

  // If we have service_links, set $vars['socialmedia'].
  if (module_exists('service_links') && _service_links_match_path()) {
    $vars['socialmedia'] = implode('', service_links_render(NULL));
  }

  // If this is an islandora object, add permalink js.
  if (arg(0) == 'islandora' && arg(1) == 'object') {
    drupal_add_js(array('permalink_path' => $_GET['q']), 'setting');
    drupal_add_js(drupal_get_path('theme', 'barnard_theme') . '/js/permalink.js');
  }

  // for debug
  // kpr($vars);
}

/**
 * Implements hook_preprocess_islandora_basic_collection_wrapper().
 */
function barnard_bootstrap_preprocess_islandora_basic_collection_wrapper(&$vars) {
  $object = $vars['islandora_object'];
  if (isset($object['MODS']) && $mods = simplexml_load_string($object['MODS']->getContent(NULL))) {
    $identifier = (string) $mods->identifier;
    $id_prefix = preg_replace('/^BC/', '', array_shift(explode('-', array_shift(explode('_', $identifier)))));

    // If this is a BC12 object, $vars['student_pubs'] is TRUE.
    if ($object->id == variable_get('bc_islandora_student_pubs_pid', 'islandora:1022') || $id_prefix == '12') {
      $vars['student_pubs'] = TRUE;
    }
  }
}

/**
 * Implements hook_preprocess_islandora_book_book().
 */
function barnard_bootstrap_preprocess_islandora_book_book(&$vars) {
  $object = $vars['object'];
  if (module_exists('bc_islandora')) {
    module_load_include('inc', 'bc_islandora', 'includes/theme');
    // Provide a link to this object's PDF datastream via $vars['dl_links'].
    $vars['dl_links'] = _bc_islandora_dl_links($object, array('PDF'));
    drupal_add_js(libraries_get_path('openseadragon') . '/openseadragon.js');
    $vars['viewer'] = theme('bc_islandora_newspaper_issue_navigator', array('object' => $object));
  }
}

/**
 * Implements hook_preprocess_islandora_book_page().
 */
function barnard_bootstrap_preprocess_islandora_book_page(&$vars) {
  $object = $vars['object'];
  if (module_exists('bc_islandora')) {
    module_load_include('inc', 'bc_islandora', 'includes/theme');
    // Provide a link to this object's JPG datastream via $vars['dl_links'].
    $vars['dl_links'] = _bc_islandora_dl_links($object, array('JPG'));
  }
}

/**
 * Implements hook_preprocess_islandora_large_image().
 */
function barnard_bootstrap_preprocess_islandora_large_image(&$vars) {
  if (module_exists('bc_islandora')) {
    module_load_include('inc', 'bc_islandora', 'includes/theme');
    // Provide a link to this object's JPG datastream via $vars['dl_links'].
    $vars['dl_links'] = _bc_islandora_dl_links($vars['islandora_object'], array('JPG'));
  }
}

/**
 * Implements hook_preprocess_islandora_manuscript_manuscript().
 */
function barnard_bootstrap_preprocess_islandora_manuscript_manuscript(&$vars) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/metadata');
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  drupal_add_js(drupal_get_path('theme', 'barnard_theme') . '/js/manuscript.js');

  $object = $vars['object'];
  $vars['metadata'] = islandora_retrieve_metadata_markup($object);
  $vars['description'] = islandora_retrieve_description_markup($object);
  $pages_ocr = array();
  $pages_hocr = array();

  if ($pages = islandora_paged_content_get_pages($object)) {
    $i = 1;
    foreach ($pages as $pid => $page) {
      if ($page_obj = islandora_object_load($pid)) {
        if (isset($page_obj['OCR'])) {
          $page_ocr = $page_obj['OCR']->getContent(NULL);
          $page_grafs = explode("\n\n", $page_ocr);
          $new_grafs = array();
          foreach ($page_grafs as $i => $graf) {
            $new_grafs[$i] = preg_replace("/\n/", ' ', $graf);
          }
          $pages_ocr[] = implode("\n\n", $new_grafs);
        }
      }
    }
  }

  if (!empty($pages_ocr)) {
    $vars['ms_transcript'] = $pages_ocr;
  }

  if (module_exists('bc_islandora')) {
    module_load_include('inc', 'bc_islandora', 'includes/theme');
    $vars['dl_links'] = _bc_islandora_dl_links($object, array('PDF', 'TRANSCRIPT'));
    if (count(islandora_paged_content_get_pages($object)) > 1) {
      $vars['ms_pager'] = _bc_islandora_np_page_pager($object);
    }
  }
}

/**
 * Implements hook_preprocess_islandora_manuscript_page().
 */
function barnard_bootstrap_preprocess_islandora_manuscript_page(&$vars) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $object = $vars['object'];
  if (isset($object['OCR'])) {
    $vars['ms_transcript'] = $object['OCR']->getContent(NULL);
    drupal_add_js(drupal_get_path('theme', 'barnard_theme') . '/js/manuscript.js');
  }
  if (module_exists('bc_islandora')) {
    module_load_include('inc', 'bc_islandora', 'includes/theme');
    $vars['dl_links'] = _bc_islandora_dl_links($object, array('TRANSCRIPT'));
    $vars['ms_pager'] = _bc_islandora_np_page_pager($object);
  }
}

/**
 * Implements hook_CMODEL_PID_islandora_solr_object_result_alter().
 */
function barnard_bootstrap_islandora_manuscriptpagecmodel_islandora_solr_object_result_alter(&$search_results, $query_processor) {
  $search_results['object_url_params']['solr'] = array(
    'query' => $query_processor->solrQuery,
    'params' => $query_processor->solrParams,
  );
}

/**
 * Implements hook_CMODEL_PID_islandora_solr_object_result_alter().
 */
function barnard_bootstrap_islandora_newspaperpagecmodel_islandora_solr_object_result_alter(&$search_results, $query_processor) {
  $query = trim($query_processor->solrQuery);
  if (empty($query)) {
    unset($search_results['object_url_params']['solr']);
    return; // Leave function.
  }

  // Ben likes this code but wants to do it a different way. This is working for now, but will be changed.
  $field_match = array(
    'catch_all_fields_mt',
    'OCR_t',
    'text_nodes_HOCR_hlt',
  );

  $field_term = '';
  $fields = preg_split('/OR|AND|NOT/', $query_processor->solrQuery);
  foreach ($fields as $field) {
    if (preg_match('/^(.*):\((.*)\)/', $field, $matches)) {
      if (isset($matches[1]) && in_array($matches[1], $field_match)) {
        $field_term = ((isset($matches[2]) && $matches[2]) ? $matches[2] : '');
        break;
      }
    }
  }

  // Ben likes this code but wants to do it a different way. This is working for now, but will be changed.
  $field_match = array(
    'catch_all_fields_mt',
    'OCR_t',
    'text_nodes_HOCR_hlt',
  );

  $field_term = '';
  $fields = preg_split('/OR|AND|NOT/', $query_processor->solrQuery);
  foreach ($fields as $field) {
    if (preg_match('/^(.*):\((.*)\)/', $field, $matches)) {
      if (isset($matches[1]) && in_array($matches[1], $field_match)) {
        $field_term = ((isset($matches[2]) && $matches[2]) ? $matches[2] : '');
        break;
      }
    }
  }

  if ($field_term) {
    $search_term = trim($field_term);
    $search_results['object_url_params']['solr']['params'] = array('defType' => 'dismax');
    $search_results['object_url_params']['solr']['query'] = $search_term;
  }
}

/**
 * Implements hook_CMODEL_PID_islandora_solr_object_result_alter().
 *
 * Add page viewing fragment and search term to show all search results within
 * book on page load.
 */
function barnard_bootstrap_islandora_bookCModel_islandora_solr_object_result_alter(&$search_results, $query_processor) {
  $view_types = array(
    "1" => "1up",
    "2" => "2up",
    "3" => "thumb",
  );

  $field_match = array(
    'catch_all_fields_mt',
    'OCR_t',
    'text_nodes_HOCR_hlt',
  );

  $field_term = '';
  $fields = preg_split('/OR|AND|NOT/', $query_processor->solrQuery);
  foreach ($fields as $field) {
    if (preg_match('/^(.*):\((.*)\)/', $field, $matches)) {
      if (isset($matches[1]) && in_array($matches[1], $field_match)) {
        $field_term = ((isset($matches[2]) && $matches[2]) ? $matches[2] : '');
        break;
      }
    }
  }

  if ($field_term) {
    $search_term = trim($field_term);
  }
  elseif ($query_processor->solrDefType == 'dismax' || $query_processor->solrDefType == 'edismax') {
    $search_term = trim($query_processor->solrQuery);
  }

  $ia_view = variable_get('islandora_internet_archive_bookreader_default_page_view', "1");
  $search_results['object_url_fragment'] = "page/1/mode/{$view_types[$ia_view]}";
  if (!empty($search_term)) {
    $search_results['object_url_fragment'] .= "/search/" . rawurlencode($search_term);
  }
}

/**
 * Implements hook_CMODEL_PID_islandora_solr_object_result_alter().
 *
 * Replaces the url for the search result to be the book's url, not the page.
 * The page is added as a fragment at the end of the book url.
 */
function barnard_bootstrap_islandora_pageCModel_islandora_solr_object_result_alter(&$search_results, $query_processor) {
  // Grab the names of the appropriate solr fields from the db.
  $parent_book_field_name = variable_get('islandora_book_parent_book_solr_field', 'RELS_EXT_isMemberOf_uri_ms');
  $page_number_field_name = variable_get('islandora_paged_content_page_number_solr_field', 'RELS_EXT_isSequenceNumber_literal_ms');
  // If:
  // there's an object url AND
  // there's a solr doc AND
  // the solr doc contains the parent book AND
  // the solr doc contains the page number...
  if (isset($search_results['object_url']) &&
    isset($search_results['solr_doc']) &&
    isset($search_results['solr_doc'][$parent_book_field_name]) &&
    count($search_results['solr_doc'][$parent_book_field_name]) &&
    isset($search_results['solr_doc'][$page_number_field_name]) &&
    count($search_results['solr_doc'][$page_number_field_name])) {
    // Replace the result url with that of the parent book and add the page
    // number as a fragment.
    $book_pid = preg_replace('/info\:fedora\//', '', $search_results['solr_doc'][$parent_book_field_name][0], 1);
    $page_number = $search_results['solr_doc'][$page_number_field_name][0];

    if (islandora_object_access(ISLANDORA_VIEW_OBJECTS, islandora_object_load($book_pid))) {
      $search_results['object_url'] = "islandora/object/$book_pid";
      $view_types = array(
        "1" => "1up",
        "2" => "2up",
        "3" => "thumb",
      );
      $ia_view = variable_get('islandora_internet_archive_bookreader_default_page_view', "1");
      $search_results['object_url_fragment'] = "page/$page_number/mode/{$view_types[$ia_view]}";

      $field_match = array(
        'catch_all_fields_mt',
        'OCR_t',
        'text_nodes_HOCR_hlt',
      );

      $field_term = '';
      $fields = preg_split('/OR|AND|NOT/', $query_processor->solrQuery);
      foreach ($fields as $field) {
        if (preg_match('/^(.*):\((.*)\)/', $field, $matches)) {
          if (isset($matches[1]) && in_array($matches[1], $field_match)) {
            $field_term = ((isset($matches[2]) && $matches[2]) ? $matches[2] : '');
            break;
          }
        }
      }

      if ($field_term) {
        $search_term = trim($field_term);
      }
      elseif ($query_processor->solrDefType == 'dismax' || $query_processor->solrDefType == 'edismax') {
        $search_term = trim($query_processor->solrQuery);
      }

      if (!empty($search_term)) {
        $search_results['object_url_fragment'] .= "/search/" . rawurlencode($search_term);
      }
    }
  }
}
