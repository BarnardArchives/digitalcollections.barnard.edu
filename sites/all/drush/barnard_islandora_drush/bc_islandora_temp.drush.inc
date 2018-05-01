<?php

/**
 * @file
 * Custom drush commands for Barnard Digital Collections.
 *
 * Much of this was adapted from Robertson Library's drush code.
 *
 * @see https://github.com/roblib/scripts/tree/master/drush/drupal7.
 */

require_once 'includes/bc_islandora.drush_ingest_helpers.inc';

/**
 * Implements hook_drush_command().
 */
function bc_islandora_temp_drush_command() {
  return array(
    'bc_islandora_generate_url_aliases' => array(
      'examples' => array(
        'drush -u 1 bcigua',
      ),
      'aliases' => array('bcigua'),
      // We can pass in users id on the command line using drush -u.
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
    ),
    'bc_islandora_check_ocr' => array(
      'examples' => array(
        'drush -u 1 bcico',
      ),
      'aliases' => array('bcico'),
    ),
    'bc_islandora_fix_pub_metadata' => array(
      'examples' => array(
        'drush bcifpm',
      ),
      'arguments' => array(
        'pid' => 'The pid of the object to process.',
      ),
      'aliases' => array('bcifpm'),
    ),
  );
}

/**
 * Generates URL aliases for objects in the repo that don't have them.
 *
 * Requires islandora_pathauto module.
 */
function drush_bc_islandora_temp_generate_url_aliases() {
  $everything_query = <<<SPARQL
    SELECT DISTINCT ?object
    FROM <#ri>
    WHERE {
      ?object <fedora-model:hasModel> ?model
    }
SPARQL;
  $tuque = islandora_get_tuque_connection();
  $results = $tuque->repository->ri->query($everything_query, 'sparql');
  foreach ($results as $result) {
    $pid = $result['object']['value'];
    if ($object = islandora_object_load($pid)) {
      // Make the alias (if it doesn't already exist).
      $default_path = "islandora/object/{$pid}";
      if (drupal_get_path_alias($default_path) == $default_path) {
        $rels = $object->relationships->get();
        foreach ($rels as $rdf) {
          if (preg_match('/isMemberOf/', $rdf['predicate']['value']) && !in_array($rdf['object']['value'], array('islandora:root', 'islandora:collectionCModel'))) {
            drush_log(dt('Creating alias for @pid', array('@pid' => $object->id)), 'ok');
            islandora_pathauto_create_alias($object, 'bulkupdate');
          }
        }
      }
      else {
        drush_log(dt('Alias already exists at pid = @pid', array('@pid' => $object->id)), 'warning');
      }
    }
    else {
      drush_log(dt('Couldn\'t load object at pid = @pid', array('@pid' => $pid)), 'warning');
    }
  }
}

/**
 * Checks if book page objects have (H)OCR datastreams.
 */
function drush_bc_islandora_temp_check_ocr($collection_pid = 'islandora:bookCollection') {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  // SPARQL query for isMemberOf book collection and hasModel bookCModel.
  $query = <<<EOQ
PREFIX islandora-rels-ext: <http://islandora.ca/ontology/relsext#>
PREFIX fedora-rels-ext: <info:fedora/fedora-system:def/relations-external#>
SELECT ?object
FROM <#ri>
WHERE {
  ?object fedora-rels-ext:isMemberOf <info:fedora/{$collection_pid}> ;
       <fedora-model:hasModel> <info:fedora/islandora:bookCModel> ;
}
EOQ;
  $tuque = islandora_get_tuque_connection();
  $results = $tuque->repository->ri->query($query, 'sparql');
  foreach ($results as $result) {
    $pid = isset($result['object']['value']) && $result['object']['type'] == 'pid' ?
      $result['object']['value'] : NULL;
    if ($pid && $book = islandora_object_load($pid)) {
      $pages = islandora_paged_content_get_pages($book);
      foreach ($pages as $p) {
        if ($page = islandora_object_load($p['pid'])) {
          if (!isset($page['OCR'])) {
            print "Didn't get OCR at book = {$book->id}\n";
            break;
          }
          elseif (!isset($page['HOCR'])) {
            print "Didn't get HOCR at book = {$book->id}\n";
            break;
          }
        }
      }
    }
  }
}

/**
 * Fixes publication metadata.
 */
function drush_bc_islandora_temp_fix_pub_metadata($pid) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $pub_obj = islandora_object_load($pid);
  $pub_mods_xml = isset($pub_obj['MODS']) ? simplexml_load_string($pub_obj['MODS']->getContent(NULL)) : '';
  $pub_obj_id = $pub_mods_xml->identifier;

  // Publication ids that constitute "exceptions" (different behavior
  // required).
  $exceptions = array('BC12-09_1959', 'BC12-26_1970', 'BC12-30_C19711972');
  $exception = in_array($pub_obj_id, $exceptions);
  $query_term = $exception ? 'islandora-rels-ext:isPageOf' : 'fedora-rels-ext:isMemberOf';

  $pub_pids = array();
  $tuque = islandora_get_tuque_connection();
  $repo = $tuque->repository;
  $query = <<<EOQ
PREFIX islandora-rels-ext: <http://islandora.ca/ontology/relsext#>
PREFIX fedora-rels-ext: <info:fedora/fedora-system:def/relations-external#>
SELECT ?object
FROM <#ri>
WHERE {
  ?object $query_term <info:fedora/{$pid}> ;
}
EOQ;

  $results = $repo->ri->query($query, 'sparql');
  foreach ($results as $r) {
    if (isset($r['object']) && $r['object']['type'] == 'pid') {
      $pub_pids[] = $r['object']['value'];
    }
  }

  // Whether to interrupt the loop.
  $interrupt = FALSE;

  foreach ($pub_pids as $pub_pid) {
    $issue_obj = islandora_object_load($pub_pid);
    $issue_rels = $issue_obj->relationships->get();

    foreach ($issue_rels as $r) {
      if ($r['predicate']['value'] == 'isMemberOf' && $parent_obj = islandora_object_load($r['object']['value'])) {
        foreach (islandora_paged_content_get_pages($issue_obj) as $pub_page) {
          try {
            drush_log(dt("Updating MODS at pid = @pid", array('@pid' => $pub_page['pid'])), 'ok');
            _update_page_mods($parent_obj, $pub_page['pid']);
          }
          catch (Exception $e) {
            drush_log(dt('Caught exception: @e', array('@e' => $e->getMessage())), 'warning');
          }
        }
      }
      elseif ($exception && !$interrupt) {
        $interrupt = TRUE;
        foreach ($pub_pids as $page_pid) {
          try {
            drush_log(dt("Updating MODS at pid = @pid", array('@pid' => $page_pid)), 'ok');
            _update_page_mods($pub_obj, $page_pid);
          }
          catch (Exception $e) {
            drush_log(dt('Caught exception: @e', array('@e' => $e->getMessage())), 'warning');
          }
        }
      }
      else {
        continue;
      }
    }
  }
}

/**
 * Update page-level publication metadata.
 */
function _update_page_mods($parent_obj, $page_pid) {
  $page_obj = islandora_object_load($page_pid);
  $page_mods = simplexml_load_string($page_obj['MODS']->getContent(NULL));
  // Get page number from title.
  preg_match('/[1-9]+$/', $page_mods->titleInfo->title, $page_match);
  unset($page_mods->relatedItem->title);
  $page_related_title = $page_mods->relatedItem[0]->titleInfo->title;
  $page_host_title = $page_mods->relatedItem[1]->titleInfo->title;
  if ($page_host_title != $parent_obj->label) {
    $page_mods->relatedItem[1]->titleInfo->title = $parent_obj->label;
  }
  if ($page_related_title != 'Student Publications') {
    $page_mods->relatedItem[0]->titleInfo->title = 'Student Publications';
  }
  if (!empty($page_match)) {
    $id_parts = explode('_', $page_mods->identifier);
    array_pop($id_parts);
    $id_parts[] = str_pad($page_match[0], 3, '0', STR_PAD_LEFT);
    $page_mods->identifier = implode($id_parts, '_') . "\n";
  }
  $page_obj['MODS']->content = $page_mods->asXML();
  $page_obj->ingestDatastream($page_obj['MODS']);
}
