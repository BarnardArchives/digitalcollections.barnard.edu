<?php

function theme_bc_islandora_object($vars) {
  $solr_obj = $vars['solr_obj'];
  $title = !$vars['title'] ? $solr_obj['solr_doc']['dc.title'][0] : $vars['title'];
  $start_page = $vars['paged'] && !$vars['start_page'] ? 1 : $vars['start_page'];

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $output = '';

  if (isset($solr_obj['content_models'])) {
    // TODO make this configurable?
    $cmodel_namespace = 'islandora';
    $cmodel_id = str_replace('CModel', '', str_replace("info:fedora/{$cmodel_namespace}:", '', array_shift($solr_obj['content_models'])));
  }
  else {
    $cmodel_id = '';
  }
  switch ($cmodel_id) {
    case 'newspaperIssue':
      if ($title) {
        $output .= "<h2>{$title}</h2>";
      }
      if ($issue = islandora_object_load($solr_obj['PID'])) {
        $identifiers = array();
        $pages = islandora_paged_content_get_pages($issue);
        foreach ($pages as $pid => $page) {
          $page_metadata = islandora_paged_content_get_page_metadata_from_djatoka($pid);
          if ($start_page > 1 && $page['page'] < $start_page) {
            $identifiers[count($pages)+$page['page']-2] = $page_metadata['identifier'];
          }
          elseif ($start_page == 1) {
            $identifiers[] = $page_metadata['identifier'];
          }
          else {
            $identifiers[$page['page']-2] = $page_metadata['identifier'];
          }
        }
        ksort($identifiers);
        dpm($identifiers);
        $output .= theme('islandora_openseadragon_viewer', array('uri' => $identifiers, 'fedora_object' => $issue));
      }
    break;
    case 'newspaperPage':
      // If we got a page, show the issue.
      // TODO but viewer should start with the appropriate page. (or just
      // redirect?)
      $parent_pid = str_replace('info:fedora/', '', $solr_obj['solr_doc']['RELS_EXT_isPageOf_uri_ms'][0]);
      if ($solr_result = _bc_islandora_solr_query(array('PID:"' . $parent_pid . '"'))) {
        $output .= theme('bc_islandora_object', array(
          'solr_obj' => $solr_result['response']['objects'][0],
          'title' => $title,
          'start_page' => intval($solr_obj['solr_doc']['RELS_EXT_isPageNumber_literal_ms'][0]),
        ));
      }
      else {
        watchdog('bc_islandora', "Didn't get solr result for page :(", 'warning');
      }
    break;
  }

  return $output;
}
