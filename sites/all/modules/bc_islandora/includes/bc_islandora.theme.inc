<?php

function theme_bc_islandora_object($solr_obj) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  dpm($solr_obj);
  $output = '';
  // TODO make this configurable?
  $cmodel_namespace = 'islandora';
  $cmodel_id = str_replace('CModel', '', str_replace("info:fedora/{$cmodel_namespace}:", '', array_shift($solr_obj['content_models'])));
  dpm($cmodel_id);
  switch ($cmodel_id) {
    case 'newspaperIssue':
      if ($issue = islandora_object_load($solr_obj['PID'])) {
        $identifiers = array();
        $pages = islandora_paged_content_get_pages($issue);
        foreach ($pages as $pid => $page) {
          $page_metadata = islandora_paged_content_get_page_metadata_from_djatoka($pid);
          $identifiers[] = $page_metadata['identifier'];
        }
        $output .= theme('islandora_openseadragon_viewer', array('uri' => $identifiers, 'fedora_object' => $issue));
      }
    break;
    case 'newspaperPage':
      // If we got a page, show the issue.
      // TODO but viewer should start with the appropriate page.
      $parent_pid = str_replace('info:fedora/', '', $solr_obj['solr_doc']['RELS_EXT_isPageOf_uri_ms'][0]);
      if ($solr_result = _bc_islandora_solr_query(array('PID:"' . $parent_pid . '"'))) {
        dpm($solr_result);
        $output .= theme('bc_islandora_object', $solr_result['response']['objects'][0]);
      }
      else {
        dpm(':(');
      }
    break;
  }

  return $output;
}
