<?php

/**
 * @file
 * Generate responses for the datastream_token rest end point.
 */

/**
 * This will get a token for a datastream.
 *
 * @param array $parameters
 *   - resource: The datastream to generate the response for.
 */
function islandora_rest_datastream_token_get_response(array $parameters) {
  module_load_include('inc', 'islandora', 'includes/authtokens');

  $datastream = $parameters['resource'];
  $token = islandora_get_object_token($datastream->parent->id, $datastream->id, $_GET['uses']);

  drupal_add_http_header('Content-Type', 'application/json; utf-8');
  return drupal_json_encode($token);
}
