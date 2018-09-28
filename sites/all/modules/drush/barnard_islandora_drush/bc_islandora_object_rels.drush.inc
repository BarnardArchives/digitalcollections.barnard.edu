<?php

/**
 * @file
 * Drush command definition for bc_islandora_relationships.
 */

/**
 * Implements hook_drush_command().
 */
function bc_islandora_object_rels_drush_command() {
  return array(
    'bc_islandora_object_rels' => array(
      'description' => 'Modify the relationships of a group of pids.',
      'arguments' => array(
        'pid' => 'A unique identifier or comma-separated list of identifiers.',
      ),
      'options' => array(
        'collection' => 'PID of the collection to add relationship.',
      ),
      'examples' => array(
        'drush -u 1 bcior bc:yearbook-1985 --collection=islandora:root',
      ),
      'aliases' => array('bcior'),
    ),
  );
}

/**
 * Purges one or more islandora objects from the fedora repository.
 *
 * @param string $object_id
 *   The pid of the object to purge.
 */
function drush_bc_islandora_object_rels($object_id) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $connection = islandora_get_tuque_connection();
  $repository = $connection->repository;

  $pids = explode(',', $object_id);

  $collection = drush_get_option('collection');

  foreach ($pids as $pid) {
    drush_log(dt('Updating rels of object at PID: @pid', array('@pid' => $pid)), 'ok');
    try {
      $object = islandora_object_load($pid);
      if ($object) {
        $existing_rels = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection);
        if (empty($existing_rels)) {
          $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection);
          drush_log(dt('Added @pid to @collection', array('@pid' => $pid, '@collection' => $collection)), 'ok');
        }
        else {
          drush_log(dt('@pid already exists in @collection', array('@pid' => $pid, '@collection' => $collection)), 'ok');
        }
      }
      else {
       drush_log(dt('Could not load the object at @pid.', array('@pid' => $pid)), 'warning');
      }
    }
    catch (Exception $e) {
      drush_log('Caught exception: @e', array('@e' => $e->getMessage()), 'warning');
    }
  }
}
