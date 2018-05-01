<?php

/**
 * @file
 * Utility functions.
 */

/**
 * To get all allowed format for JODConverter.
 *
 * @return array
 *   All allowed formats.
 */
function islandora_jodconverter_default_formats() {
  $all_formats = array();
  $all_formats['odt'] = array(
    'name' => 'OpenDocument Text',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['sxw'] = array(
    'name' => 'OpenOffice.org 1.0 Text',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['rtf'] = array(
    'name' => 'OpenDocument',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['doc'] = array(
    'name' => 'Rich Text Format',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['docx'] = array(
    'name' => 'Rich Text Format',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['wpd'] = array(
    'name' => 'WordPerfect',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['txt'] = array(
    'name' => 'Plain Text',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['html'] = array(
    'name' => 'HTML',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['pdf'] = array(
    'name' => 'Portable Document',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['wiki'] = array(
    'name' => 'MediaWiki wikitext',
    'from' => array(),
    'to' => array(),
  );

  $all_formats['ods'] = array(
    'name' => 'OpenDocument Spreadsheet',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['sxc'] = array(
    'name' => 'OpenOffice.org 1.0 Spreadsheet',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['xls'] = array(
    'name' => 'Microsoft Excel',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['xlsx'] = array(
    'name' => 'Microsoft Excel',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['csv'] = array(
    'name' => 'Comma-Separated Values',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['tsv'] = array(
    'name' => 'Tab-Separated Values',
    'from' => array(),
    'to' => array(),
  );

  $all_formats['odp'] = array(
    'name' => 'OpenDocument Presentation',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['sxi'] = array(
    'name' => 'OpenOffice.org 1.0 Presentation',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['ppt'] = array(
    'name' => 'Microsoft PowerPoint',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['pptx'] = array(
    'name' => 'Microsoft PowerPoint',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['swf'] = array(
    'name' => 'Macromedia Flash',
    'from' => array(),
    'to' => array(),
  );

  $all_formats['odg'] = array(
    'name' => 'Microsoft PowerPoint',
    'from' => array(),
    'to' => array(),
  );
  $all_formats['svg'] = array(
    'name' => 'Macromedia Flash',
    'from' => array(),
    'to' => array(),
  );

  $families = islandora_jodconverter_get_families();
  foreach ($families as $family) {
    foreach ($family['from'] as $from) {
      if (isset($all_formats[$from])) {
        $all_formats[$from]['to'] = array_merge($all_formats[$from]['to'], $family['to']);
      }
    }
  }

  foreach ($families as $family) {
    foreach ($family['to'] as $to) {
      if (isset($all_formats[$to])) {
        $all_formats[$to]['from'] = array_merge($all_formats[$to]['from'], $family['from']);
      }
    }
  }
  return $all_formats;
}

/**
 * To get a format information.
 *
 * @param string $ext
 *   The extension of a format type.
 *
 * @return array|bool
 *   The format type information.
 */
function islandora_jodconverter_get_format($ext) {
  $all_formats = islandora_jodconverter_default_formats();
  if (!isset($all_formats[$ext])) {
    watchdog('JODConverter', 'Undefined type %format', array('%format' => $ext), WATCHDOG_ERROR);
    return FALSE;
  }
  return $all_formats[$ext];
}

/**
 * Get the Family information.
 *
 * @param string $family
 *   The family name.
 *
 * @return array
 *   The family information.
 */
function islandora_jodconverter_get_families($family = NULL) {
  $families = array(
    'Text' => array(
      'from' => array(
        'odt',
        'sxw',
        'rtf',
        'doc',
        'docx',
        'wpd',
        'txt',
        'html',
      ),
      'to' => array(
        'pdf',
        'odt',
        'sxw',
        'rtf',
        'doc',
        'docx',
        'txt',
        'html',
        'wiki',
      ),
    ),
    'Spreadsheet' => array(
      'from' => array(
        'ods',
        'sxc',
        'xls',
        'xlsx',
        'csv',
        'tsv',
      ),
      'to' => array(
        'pdf',
        'ods',
        'sxc',
        'xls',
        'xlsx',
        'csv',
        'tsv',
        'html',
      ),
    ),
    'Presentation' => array(
      'from' => array(
        'odp',
        'sxi',
        'ppt',
        'pptx',
      ),
      'to' => array(
        'pdf',
        'swf',
        'odp',
        'sxi',
        'ppt',
        'pptx',
        'html',
      ),
    ),
    'Drawing' => array(
      'from' => array(
        'odg',
      ),
      'to' => array(
        'svg',
        'swf',
      ),
    ),
  );

  $return_array = array();
  if ($family && !is_array($family)) {
    $family = array($family);
  }

  if ($family) {
    foreach ($family as $value) {
      if (isset($value)) {
        $return_array[$value] = $families[$value];
      }
    }
    return $return_array;
  }
  else {
    return $families;
  }
}

/**
 * Execute the convert command.
 *
 * @param string $command
 *   The commend after java -jar *.jar.
 *
 * @return array
 *   the out put of commend line. Return NULL if secusses executed.
 */
function islandora_jodconverter_exec_convert($command) {
  $conv_path = drupal_realpath(libraries_get_path('jodconverter-2.2.2') . '/lib/jodconverter-cli-2.2.2.jar');
  $output = array();
  $exe_jar = "java -jar";
  $port = variable_get('islandora_jodconverter_openoffice_port', 8100);
  if ($port != 8100) {
    $exe_jar .= "-p $port";
  }
  exec("$exe_jar $conv_path $command  2>&1", $output, $return_var);
  return $output;
}

/**
 * To get the extension of a file.
 *
 * @param string $filename
 *   The file name
 *
 * @return bool|string
 *   return the extension or false if not exist
 */
function islandora_jodconverter_get_ext($filename) {
  $pos = strrpos($filename, ".");
  if ($pos === FALSE) {
    return FALSE;
  }
  else {
    return substr($filename, $pos + 1);
  }
}
