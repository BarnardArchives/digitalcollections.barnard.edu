<?php

/**
 * @file
 * Drush command definition for bc_islandora_ingest_objects.
 */

require_once 'includes/bc_islandora.drush_ingest_methods.inc';

/**
 * Implements hook_drush_command().
 */
function bc_islandora_ingest_drush_command() {
  return array(
    'bc_islandora_ingest_objects' => array(
      'description' => 'Ingests objects.',
      'arguments' => array(
        'query_file' => 'A csv file containing information about the objects to be ingested.',
      ),
      'options' => array(
        'type' => 'The type of objects to ingest.',
        'year' => 'Specific year from which to ingest objects.',
        'month' => 'Month from which to ingest objects.',
        'generate-csv' => 'Whether to generate a csv file for use in newspaper ingest.',
        'identifier' => 'The identifier of the collection to ingest.',
      ),
      'examples' => array(
        'drush -u 1 --uri=http://localhost bciio /path/to/pages.csv',
        'drush -u 1 bciio --type=newspaper --year=1991 /home/vagrant/bulletin-subset',
        'drush -u 1 bciio --type=newspaper /home/vagrant/bulletin-subset/1991pages.csv',
        'drush -u 1 bciio --type=book --year=1985 /home/vagrant/yearbook/yearbook.csv',
        'drush -u 1 bciio --type=student_publication /home/vagrant/student-pubs',
        'drush -u 1 bciio --type=student_publication --identifier=BC12-04 /home/vagrant/student-pubs',
        'drush -u 1 bciio --type=manuscript /home/vagrant/manuscripts/metadata.csv',
        'drush -u 1 bciio --type=manuscript --identifier=BC13-58_71Q-Daly1972 /home/vagrant/manuscripts/metadata.csv',
        'drush -u 1 bciio --type=rgBC15 /mnt/barnard.collections/RG15-Alumnae_Scrapbooks/metadata.csv',
        'drush -u 1 bciio --type=rgBC1108 /mnt/barnard.collections/RG11-08-Alumnae_Magazine/metadata.csv',
      ),
      'aliases' => array('bciio'),
      // We can pass in users id on the command line using drush -u.
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
    ),
  );
}

/**
 * Ingests objects using the appropriate ingest method and options.
 */
function drush_bc_islandora_ingest_objects($query_file) {
  if ($type = drush_get_option('type')) {
    $function = "_bc_islandora_drush_ingest_{$type}";
  }
  else {
    drush_log(dt("Please specify type of object to ingest."), 'error');
    return;
  }

  $options = array(
    'year' => drush_get_option('year'),
    'month' => drush_get_option('month'),
    'generate-csv' => drush_get_option('generate-csv'),
    'identifier' => drush_get_option('identifier'),
  );

  if (function_exists($function)) {
    $function($query_file, $options);
  }
  else {
    drush_log(dt("Didn't get ingest method at type = @type", array('@type' => $type)), 'error');
  }
}
