<?php

/**
 * @file
 * Generate responses for the object rest end point.
 */

/**
 * Responds to the GET request for this resource.
 *
 * This function assumes the requested resource is the subject of all returned
 * triples.
 *
 * @param array $parameters
 *   An associative array containing relevent data for this request.
 *   - resource: The object to generate the response for.
 *   - request: The request parameters
 *     - predicate: The predicate to limit the results to. (optional)
 *     - uri: The uri of the predicate, required if predicate is present.
 *     - object: The object to limit the results to. (optional)
 *     - literal: TRUE if the object is literal, FALSE otherwise. Defaults to
 *       FALSE. (optional)
 *
 * @return array
 *   The un-encoded response containing an array of the following fields.
 *   - predicate: A relationship predicate
 *     - value: The name of the predicate.
 *     - alias: The alias for this predicate's namespace.
 *     - namespace: The namespace of this predicate.
 *   - object: The object of the triple.
 *     - literal: TRUE if the value is a literal FALSE if it is a URI.
 *     - value: The object's value.
 */
function islandora_rest_relationship_get_response(array $parameters) {
  $request = $parameters['request'];
  $predicate = isset($request['predicate']) ? $request['predicate'] : NULL;
  $uri = isset($request['uri']) ? $request['uri'] : NULL;
  $object = isset($request['object']) ? $request['object'] : NULL;
  $literal = isset($request['literal']) ? $request['literal'] : FALSE;
  return $parameters['resource']->relationships->get($uri, $predicate, $object, $literal);
}

/**
 * Responds to the POST request for this resource.
 *
 * Creates a new relationship, with the given $params.
 *
 * @param array $parameters
 *   An associative array containing relevent data for this request.
 *   - resource: The object that will own the new relationship.
 *   - request: The request parameters
 *     - uri: The predicate URI for the given predicate
 *     - predicate: The predicate of the relationship.
 *     - object: Object of the relationship.
 *     - type: The type of the object. This is expressed as an string it can be
 *       either, 'uri', 'string', 'int', 'date', 'none'. (optional)
 *       Defaults to 'uri'.
 */
function islandora_rest_relationship_post_response(array $parameters) {
  $request = $parameters['request'];
  $required = array('uri', 'predicate', 'object', 'type');
  islandora_rest_require_parameters($request, $required);
  $type = islandora_rest_relationship_get_datatype($request['type']);
  $parameters['resource']->relationships->add($request['uri'], $request['predicate'], $request['object'], $type);
  drupal_add_http_header('Status', 201);
}

/**
 * Responds to the DELETE request for this resource.
 *
 * Delete's the given relationship.
 *
 * @param array $parameters
 *   An associative array containing relevent data for this request.
 *   - resource: The object to generate the response for.
 *   - request: The request parameters
 *     - predicate: The predicate to limit the remove to (optional).
 *     - uri: The uri of the predicate, required if predicate is present.
 *     - object: The object to limit the remove to (optional).
 *     - literal: TRUE if the object is literal, FALSE otherwise. Defaults to
 *       FALSE (optional).
 */
function islandora_rest_relationship_delete_response(array $parameters) {
  $request = $parameters['request'];
  $predicate = isset($request['predicate']) ? $request['predicate'] : NULL;
  $uri = isset($request['uri']) ? $request['uri'] : NULL;
  $object = isset($request['object']) ? $request['object'] : NULL;
  $literal = isset($request['literal']) ? $request['literal'] : FALSE;
  $parameters['resource']->relationships->remove($uri, $predicate, $object, $literal);
}

/**
 * Converts the string repersentation of relationship datatype into an integer.
 *
 * @param string $type
 *   The data type of a relationships expected to be either 'uri', 'string',
 *   'int', 'date', or 'none'.
 *
 * @return int
 *   The integer constant value for the given relationship type. Defaults to
 *   RELS_TYPE_URI.
 */
function islandora_rest_relationship_get_datatype($type = NULL) {
  switch ($type) {
    case 'string':
      return RELS_TYPE_STRING;

    case 'int':
      return RELS_TYPE_INT;

    case 'date':
      return RELS_TYPE_DATETIME;

    case 'none':
      return RELS_TYPE_PLAIN_LITERAL;

    case 'uri':
    default:
      return RELS_TYPE_URI;
  }
}
