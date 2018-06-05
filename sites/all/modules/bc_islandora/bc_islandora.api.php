<?php

/**
 * @file
 * We have an API?
 *
 * Sometimes we like to play with our metadata using php. Why not?
 *
 * @author Benjamin Rosner https://github.com/br2490
 */

/**
 * Hook to configure multiple metadata transformations for use.
 *
 * @return array
 *   Returns an associative array mapping unique machine names to associative
 *   arrays with all the data required to build a primary display profile, such
 *   as:
 *   - "name": A translated string to display.
 *   - "module": The module where the code for the given modification is
 *     located.
 *   - "file": A file under the given module's path to load before trying to
 *     instantiate the modification.
 *   - "function": The name of a method/function to call on the object.
 *   - "description": Short string describing modification.
 */
function hook_barnard_islandora_metadata_functions() {
  return array(
    'machine_name' => array(
      'name' => t('Human-readable name'),
      'module' => 'module_name',
      'file' => 'file_name.inc',
      'function' => 'function_name',
      'description' => t('A description of the modification.'),
    ),
  );
}
