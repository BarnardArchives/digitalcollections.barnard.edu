<?php
/**
 * @file
 * bc_islandora_exhibit_feature.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function bc_islandora_exhibit_feature_taxonomy_default_vocabularies() {
  return array(
    'exhibition_themes' => array(
      'name' => 'Exhibition themes',
      'machine_name' => 'exhibition_themes',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
