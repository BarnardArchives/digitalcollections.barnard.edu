<?php

/**
 * @file
 * Drush command definition for bc_islandora_purge_object.
 */

/**
 * Implements hook_drush_command().
 */
function bc_islandora_purge_drush_command() {
  return array(
    'bc_islandora_purge_object' => array(
      'description' => 'Purges an object from the repository.',
      'arguments' => array(
        'pid' => 'A unique identifier.',
      ),
      'examples' => array(
        'drush -u 1 bcipo bc:yearbook-1985',
      ),
      'aliases' => array('bcipo'),
    ),
  );
}

/**
 * Purges one or more islandora objects from the fedora repository.
 *
 * @param string $object_id
 *   The pid of the object to purge.
 */
function drush_bc_islandora_purge_object($object_id) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $connection = islandora_get_tuque_connection();
  $repository = $connection->repository;

  $pids = array($object_id);
  $object = islandora_object_load($object_id);

  if ($pages = islandora_paged_content_get_pages($object)) {
    $continue = drush_prompt(dt('Got paged object at pid = @pid. Purge pages?', array('@pid' => $object_id)));
    if (preg_match('/^y/', strtolower($continue)) === 1) {
      foreach ($pages as $page_id => $page) {
        $pids[] = $page_id;
      }
    }
  }

  foreach ($pids as $pid) {
    drush_log(dt('Purging object at pid = @pid...', array('@pid' => $pid)), 'ok');
    try {
      $repository->purgeObject($pid);
    }
    catch (Exception $e) {
      drush_log('Caught exception: @e', array('@e' => $e->getMessage()), 'warning');
    }
  }
}
