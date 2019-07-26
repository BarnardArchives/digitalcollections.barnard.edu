<?php

/**
 * @file
 * Custom blocks for Barnard Islandora.
 */

/**
 * Provides a list to change the sorting on a current search query.
 *
 * @global IslandoraSolrQueryProcessor $_islandora_solr_queryclass
 *   The IslandoraSolrQueryProcessor object which includes the current query
 *   settings and the raw Solr results.
 *
 * @return string
 *   A rendered list containing Solr sort links.
 */
function bc_islandora_islandora_solr_sort_select() {
  module_load_include('inc', 'islandora', 'includes/utilities');
  global $_islandora_solr_queryclass;
  $output = '';
  // Check if the page is dealing with search results.
  if (islandora_solr_results_page($_islandora_solr_queryclass)) {
    // Parameters set in URL.
    $params = isset($_islandora_solr_queryclass->internalSolrParams) ? $_islandora_solr_queryclass->internalSolrParams : array();
    $path = current_path();
    $sort_terms = islandora_solr_get_fields('sort_fields');
    // Get current sort (check in the actual parameters used for the query,
    // because modules might have altered it).
    if (isset($_islandora_solr_queryclass->solrParams['sort'])) {
      $sort_explode = preg_split(
        ISLANDORA_SOLR_QUERY_SPLIT_REGEX,
        $_islandora_solr_queryclass->solrParams['sort']
      );
      $current_sort_term = stripslashes($sort_explode[0]);
      $current_sort_order = isset($sort_explode[1]) ? $sort_explode[1] : NULL;
    }
    else {
      // If no parameters are given, internally solr defaults to 'score desc'.
      // See Common Query Paremeters section:
      // http://archive.apache.org/dist/lucene/solr/ref-guide/
      $current_sort_term = 'score';
      $current_sort_order = 'desc';
    }
    $list_items = array();
    foreach ($sort_terms as $term => $label) {
      if ($term == $current_sort_term) {
        if (isset($current_sort_order) && $current_sort_order == 'asc') {
          $order = 'desc';
          $order_rev = 'asc';
        }
        else {
          $order = 'asc';
          $order_rev = 'desc';
        }
      }
      // The term is score or a date, we want descending order by default.
      elseif ($term == 'score' || stripos($term, 'date')) {
          $order = 'desc';
          $order_rev = 'asc';
        }
      else {
          $order = 'desc';
          $order_rev = 'asc';
      }
      if ($term == $current_sort_term) {
        // Dual buttons. Should possibly be done in render array, no time like the present. 
        $primary = '<div class="btn-group"><button type="button" ' .
        drupal_attributes(array(
          'data-toggle' => 'dropdown',
          'class' => array('dropdown', 'dropdown-toggle', 'btn', 'btn-default'))) . '>' . 
        $label . ' ' . _bc_islandora_solr_sort_word_expand($current_sort_order) . ' </button>';
        $primary .= '<button type="button" ' .
        drupal_attributes(array(
          'data-toggle' => 'dropdown',
          'class' => array('dropdown', 'dropdown-toggle', 'btn', 'btn-default'))) . '><span class="caret"></span></button>';
        $item = _bc_islandora_solr_sort_link_gen($term, $label, $order, $params, $path, TRUE);
      } else {
        $item = _bc_islandora_solr_sort_link_gen($term, $label, $order_rev, $params, $path);
        $list_items[] = $item;
        $item = _bc_islandora_solr_sort_link_gen($term, $label, $order, $params, $path);
      }
      // Add link to list.
      $list_items[] = $item;
    }
    // Theme item list.
    if (!empty($list_items)) {
      $output = '<div class="dropdown islandora-solr-sort-select">';
      $output .= $primary;
      $output .= theme('item_list', array(
        'items' => $list_items,
        'title' => NULL,
        'type' => 'ul',
        'attributes' => array('class' => array('dropdown-menu', 'islandora-solr-sort-selects')),
      ));
      $output .= '</div></div>';
    }
  }
  return $output;
}

/**
 * Provides word expansion for sorting ascending or descending.
 * 
 * @param  string Accepts either 'asc' or 'desc'
 * @return string Full word.
 */
function _bc_islandora_solr_sort_word_expand($word) {
  return $word === 'desc' ? '(newest-oldest)' : '(oldest-newest)';
}

/**
 * Helper function to generate links for Solr Sort.
 * 
 * @param  string
 * @param  string
 * @param  string
 * @param  string
 * @param  string
 * @param  boolean
 * @return Rendered HTML snippet.
 */
function _bc_islandora_solr_sort_link_gen($term, $label, $order, $params, $path, $active=FALSE) {
  $sort_param = $term . ' ' . $order;
  $query = array_merge($params, array('sort' => $sort_param));
  $attributes = array(
    'sort' => array(
      'attr' => array(
        'class' => array(),
      ),
      'query' => $query,
      'path' => $path,
    ),
  );
  $attr =& $attributes['sort']['attr'];
  if ($active) {
    // Dual buttons. Should possibly be done in render array, no time like the present. 
    $attr['class'] = array('active');

  }
  $attr['rel'] = 'nofollow';
  $attr['title'] = $label;
  $attr['href'] = url($path, array('query' => $query));
  $hooks = islandora_build_hook_list(ISLANDORA_SOLR_FACET_BUCKET_CLASSES_HOOK_BASE);
  drupal_alter($hooks, $attributes, $_islandora_solr_queryclass);
  // XXX: We're not using l() because of
  // @link http://drupal.org/node/41595 active classes. @endlink
  $item = '<a' . drupal_attributes($attr) . '>' . $label . ' ' . _bc_islandora_solr_sort_word_expand($order) . '</a>';
  return $item;
}
