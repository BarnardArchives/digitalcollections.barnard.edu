<?php
/**
 * @file
 * barnard_theme_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function barnard_theme_feature_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function barnard_theme_feature_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_islandora_solr_fields_features_default_fields().
 */
function barnard_theme_feature_islandora_solr_fields_features_default_fields() {
  return array(
  0 => array(
    'solr_field' => 'dc.title',
    'field_type' => 'facet_fields',
    'weight' => -10,
    'solr_field_settings' => array(
      'label' => 'Title',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
    ),
  ),
  1 => array(
    'solr_field' => 'fgs_label_s',
    'field_type' => 'result_fields',
    'weight' => -10,
    'solr_field_settings' => array(
      'label' => 'Label',
      'snippet' => NULL,
      'date_format' => '',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
      'link_to_object' => TRUE,
      'link_to_search' => FALSE,
    ),
  ),
  2 => array(
    'solr_field' => 'catch_all_fields_mt',
    'field_type' => 'search_fields',
    'weight' => -10,
    'solr_field_settings' => array(
      'label' => 'All',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
    ),
  ),
  3 => array(
    'solr_field' => 'fgs_label_s',
    'field_type' => 'facet_fields',
    'weight' => -9,
    'solr_field_settings' => array(
      'label' => 'Label',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
    ),
  ),
  4 => array(
    'solr_field' => 'fgs_label_t',
    'field_type' => 'search_fields',
    'weight' => -9,
    'solr_field_settings' => array(
      'label' => 'Label',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
    ),
  ),
  5 => array(
    'solr_field' => 'RELS_EXT_hasModel_uri_ms',
    'field_type' => 'result_fields',
    'weight' => -9,
    'solr_field_settings' => array(
      'label' => 'Model(s)',
      'snippet' => NULL,
      'date_format' => '',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
      'link_to_object' => FALSE,
      'link_to_search' => FALSE,
    ),
  ),
  6 => array(
    'solr_field' => 'dc.description',
    'field_type' => 'facet_fields',
    'weight' => -8,
    'solr_field_settings' => array(
      'label' => 'Description',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
    ),
  ),
  7 => array(
    'solr_field' => 'dc.description',
    'field_type' => 'search_fields',
    'weight' => -8,
    'solr_field_settings' => array(
      'label' => 'Description',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
    ),
  ),
  8 => array(
    'solr_field' => 'fgs_createdDate_mt',
    'field_type' => 'result_fields',
    'weight' => -8,
    'solr_field_settings' => array(
      'label' => 'Created',
      'snippet' => NULL,
      'date_format' => '',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
      'link_to_object' => FALSE,
      'link_to_search' => FALSE,
    ),
  ),
  9 => array(
    'solr_field' => 'fgs_lastModifiedDate_mt',
    'field_type' => 'result_fields',
    'weight' => -7,
    'solr_field_settings' => array(
      'label' => 'Modified',
      'snippet' => NULL,
      'date_format' => '',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
      'link_to_object' => FALSE,
      'link_to_search' => FALSE,
    ),
  ),
  10 => array(
    'solr_field' => 'dc.description',
    'field_type' => 'result_fields',
    'weight' => -6,
    'solr_field_settings' => array(
      'label' => 'Description',
      'snippet' => 1,
      'date_format' => '',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
      'link_to_object' => FALSE,
      'link_to_search' => FALSE,
    ),
  ),
  11 => array(
    'solr_field' => 'dc.date',
    'field_type' => 'sort_fields',
    'weight' => 3,
    'solr_field_settings' => array(
      'label' => 'Date',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
    ),
  ),
  12 => array(
    'solr_field' => 'mods_subject_topic_ms',
    'field_type' => 'facet_fields',
    'weight' => 3,
    'solr_field_settings' => array(
      'label' => 'Topic',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
    ),
  ),
  13 => array(
    'solr_field' => 'dc.title',
    'field_type' => 'sort_fields',
    'weight' => 4,
    'solr_field_settings' => array(
      'label' => 'Title',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
    ),
  ),
  14 => array(
    'solr_field' => 'fgs_createdDate_dt',
    'field_type' => 'facet_fields',
    'weight' => 4,
    'solr_field_settings' => array(
      'label' => 'Date',
      'range_facet_select' => 1,
      'range_facet_variable_gap' => 1,
      'range_facet_start' => 'NOW/YEAR-20YEARS',
      'range_facet_end' => 'NOW',
      'range_facet_gap' => '+1YEAR',
      'date_facet_format' => 'Y',
      'range_facet_slider_enabled' => 1,
      'range_facet_slider_color' => '#edc240',
      'date_filter_datepicker_range' => '-100:+3',
      'permissions' => array(
        1 => 1,
        2 => 2,
        3 => 3,
      ),
      'date_filter_datepicker_enabled' => NULL,
    ),
  ),
);
}
