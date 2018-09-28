<?php

/**
 * @file
 * Helper functions invoked in custom drush methods.
 *
 * @see bc_islandora.drush.inc
 * @see https://github.com/roblib/scripts/tree/master/drush/drupal7
 */

require_once 'bc_islandora.drush_mods_data.inc';

/**
 * Creates a simple Islandora object of specified type W/O an OBJ datastream.
 *
 * MODS, DC, and RELS are created for these objects. MODS is provided for the
 * script to transform it to DC. If it is a collection a basic POLICY model is
 * added.
 *
 * @param string $pid
 *   The exact pid or namespace (to use the next int in NS) of the object.
 * @param array $data
 *   Describes the object to be created.
 *   Required array variables:
 *    'title' is the LABEL of the object in Fedora (i.e. the title.).
 *    'content_model' is the content model type to create.
 *    'xml' is the FULL MODS record for the object, even if it is basic.
 * @param string $collection_pid
 *   Parent object this object is member of (this sets RELS-EXT relationships)
 *
 * @return string|null
 *   PID of the newly created object.
 *
 * @throws \Exception
 *   Because this documentation is accurate.
 */
function create_simple_object($pid, array $data, $collection_pid, $membership = TRUE) {
  $required_variables = ['title', 'content_model', 'xml'];
  foreach ($required_variables as $v) {
    if (!isset($data[$v])) {
      drush_log(dt('Could not create @pid due to missing required variable: @missing.', [
        '@pid' => $pid,
        '@missing' => $v,
      ]), 'error');
      return NULL;
    }
  }
  module_load_include('inc', 'islandora', 'RestConnection');

  $connection = islandora_get_tuque_connection();
  $repository = $connection->repository;

  $object = $repository->constructObject($pid);

  $object->label = $data['title'];
  $content_model_type = $data['content_model'];

  $xml = $data['xml'];
  $mods_xml = new DOMDocument();
  $mods_xml->loadXML($xml);

  // Begin FEDORA and ISLANDORA relationships.
  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_type);
//  if ($membership) {
//    $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection_pid);
//  }
//  else {
  $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $collection_pid);
//  }

  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasLanguage', 'eng');

  if ($content_model_type === 'islandora:collectionCModel') {
    // Begin basic COLLECTION_POLICY:
    $c_policy = $object->constructDatastream('COLLECTION_POLICY');
    $c_policy->label = 'Collection Policy';
    $c_policy->mimetype = 'text/xml';
    $c_policy->setContentFromFile(DRUPAL_ROOT . '/sites/all/drush/xml/cpolicy.xml', FALSE);

    $object->ingestDatastream($c_policy);
  }

  // DC Transform:
  $transform_xsl = new DOMDocument();
  $xsl_file = DRUPAL_ROOT . '/' . drupal_get_path('module', 'islandora_importer') . '/xsl/mods_to_dc.xsl';
  $transform_xsl->load($xsl_file);
  $proc = new XSLTProcessor();
  $proc->importStylesheet($transform_xsl);
  $dc = $proc->transformToXML($mods_xml);
  $dc_datastream = $object->constructDatastream('DC');
  $dc_datastream->label = 'Dublin Core Record for this object';
  $dc_datastream->mimetype = 'text/xml';
  $dc_datastream->content = $dc;

  $object->ingestDatastream($dc_datastream);

  // Begin Mods:
  $mods_datastream = $object->constructDatastream('MODS');
  $mods_datastream->label = 'MODS record';
  $mods_datastream->mimetype = 'text/xml';
  $mods_datastream->setContentFromString($xml);
  $mods_datastream->checksum = TRUE;
  $mods_datastream->checksumType = 'MD5';
  $mods_datastream->logMessage = 'MODS datastream set from string. || SUCCESS';

  $object->ingestDatastream($mods_datastream);

  try {
    $object->state = 'A';
    $ingest = $repository->ingestObject($object);
  } catch (Exception $e) {
    unset($object);
    throw($e);
  }

  $ret = '';
  if ($ingest == TRUE) {
    $ret = $object->id;
    drush_log(dt("create simple object: @type destination: @dest created successfully. PID: @pid",
      [
        '@type' => $content_model_type,
        '@dest' => $pid,
        '@pid' => $ret,
      ]), 'success');
  }
  else {
    drush_log(dt("create simple object: @type destination: @dest failed to be created.", [
      '@type' => $content_model_type,
      '@dest' => $pid,
    ]), 'warning');
  }

  // Make sure destructors are called as this script can run for a long time.
  unset($transform_xsl);
  unset($proc);
  unset($object);
  unset($mods_datastream);
  unset($dc_datastream);

  return $ret;
}

/**
 * Creates a complete islandora object of a specified type.
 *
 * Expects an actual file to process, and allows for deriv generation
 * based on your Islandora settings.
 *
 * MODS, DC, and RELS are created for these objects. MODS is provided for the
 * script to transform it to DC.
 *
 * @param string $pid
 *   The exact pid or namespace (to use the next int in NS) of the object.
 * @param array $data
 *   Describes the object to be created.
 *   Required array variables:
 *    'title' is the label of the object in Fedora.
 *    'content_model' is the content model type to create.
 *    'xml' is the FULL MODS record for the object, even if it is basic.
 *    'mime_type' is the mime type of the archival object.
 *    'mime_label' is the human readable LABEL of mimetype (e.g. TIFF Image).
 *   Optional array variables:
 *    'page_number' is the page number of the object.
 * @param string $collection_pid
 *   Parent object this object is member of (this sets RELS-EXT relationships)
 * @param string $file_path
 *   Full path to the archival object on the file system to ingest.
 *
 * @return string|null
 *   PID of the newly created object.
 *
 * @throws \Exception
 *   Because this documentation is accurate.
 *
 * @todo Change $data['page_number'] since we don't really know what type of
 *   OBJ. Make less assumptions.
 */
function create_complete_object($pid, array $data, $collection_pid, $file_path) {
  $required_variables = [
    'title',
    'content_model',
    'xml',
    'mime_type',
    'mime_label',
  ];
  foreach ($required_variables as $v) {
    if (!isset($data[$v])) {
      drush_log(dt('Could not create @pid due to missing required variable: @missing.', [
        '@pid' => $pid,
        '@missing' => $v,
      ]), 'error');
      return NULL;
    }
  }

  module_load_include('inc', 'islandora', 'RestConnection');

  $connection = islandora_get_tuque_connection();
  $repository = $connection->repository;

  $object = $repository->constructObject($pid);

  $object->label = $data['title'];

  $content_model_type = $data['content_model'];
  $mime_type = $data['mime_type'];
  $mime_label = $data['mime_label'];
  isset($data['page_number']) ? $page_number = $data['page_number'] : NULL;
  $file_name = $file_path;

  // Ingest a thumbnail of the first page to the parent object.
  if ($page_number == 1) {
    $rand = rand('100', '999');
    $tmp_file = '/tmp/tmp_file' . $rand . '.jpg';
    $command = 'convert -quiet -thumbnail 200x200 ' . $file_name . ' ' . $tmp_file;
    exec($command, $exec_output, $exec_return);
    if ($exec_return == 0) {
      $book_item = $repository->constructObject($collection_pid);
      $tn_datastream = $book_item->constructDatastream('TN');
      $tn_datastream->setContentFromFile($tmp_file, FALSE);
      $tn_datastream->label = 'Thumbnail';
      $tn_datastream->mimetype = 'image/jpg';
      $tn_datastream->checksum = TRUE;
      $tn_datastream->checksumType = 'MD5';
      $tn_datastream->logMessage = "TN derivative created by ImageMagick with command - $command || SUCCESS";
      $book_item->ingestDatastream($tn_datastream);
      unlink($tmp_file);
      drush_log('Updated parent object TN.', 'ok');
    }
  }

  $xml = $data['xml'];

  $mods_xml = new DOMDocument();
  $mods_xml->loadXML($xml);

  // Begin FEDORA and ISLANDORA relationships.
  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_type);
  $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $collection_pid);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasLanguage', 'eng');
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isPageOf', $collection_pid);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', $page_number);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isPageNumber', $page_number);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSection', '1');


  // DC Transform:
  $transform_xsl = new DOMDocument();
  $xsl_file = DRUPAL_ROOT . '/' . drupal_get_path('module', 'islandora_importer') . '/xsl/mods_to_dc.xsl';
  $transform_xsl->load($xsl_file);
  $proc = new XSLTProcessor();
  $proc->importStylesheet($transform_xsl);
  $dc = $proc->transformToXML($mods_xml);
  $dc_datastream = $object->constructDatastream('DC');
  $dc_datastream->label = 'Dublin Core Record for this object';
  $dc_datastream->mimetype = 'text/xml';
  $dc_datastream->content = $dc;

  $object->ingestDatastream($dc_datastream);

  // Begin Mods:
  $mods_datastream = $object->constructDatastream('MODS');
  $mods_datastream->label = 'MODS record';
  $mods_datastream->mimetype = 'text/xml';
  $mods_datastream->setContentFromString($xml);
  $mods_datastream->checksum = TRUE;
  $mods_datastream->checksumType = 'MD5';
  $mods_datastream->logMessage = 'MODS datastream set from string. || SUCCESS';

  $object->ingestDatastream($mods_datastream);

  // Create the OBJ:
  $object_stream = $object->constructDatastream('OBJ');
  $object_stream->setContentFromFile($file_name, FALSE);
  $object_stream->label = $mime_label;
  $object_stream->mimetype = $mime_type;
  $object_stream->checksum = TRUE;
  $object_stream->checksumType = 'MD5';
  $object_stream->logMessage = 'OBJ datastream created using batch ingest script || SUCCESS';

  $object->ingestDatastream($object_stream);

  // Set OCR.
  if (!empty($data['ocr'])) {
    $ocr_datastream = $object->constructDatastream('OCR');
    $ocr_datastream->setContentFromFile($data['ocr'], FALSE);
    $ocr_datastream->label = 'OCR';
    $ocr_datastream->mimetype = 'text/plain';
    $ocr_datastream->checksum = TRUE;
    $ocr_datastream->checksumType = 'MD5';
    $ocr_datastream->logMessage = 'OCR set from text file || SUCCESS';
    drush_log("$pid had OCR set from text file.", 'success');
    $object->ingestDatastream($ocr_datastream);
  }

  try {
    $object->state = 'A';
    $ingest = $repository->ingestObject($object);
  } catch (Exception $e) {
    unset($object);
    throw($e);
  }

  $ret = '';
  if ($ingest == TRUE) {
    $ret = $object->id;
    drush_log(dt("create complete object: @type destination: @dest created successfully. PID: @pid",
      [
        '@type' => $content_model_type,
        '@dest' => $pid,
        '@pid' => $ret,
      ]), 'success');
  }
  else {
    drush_log(dt("create complete object: @type destination: @dest failed to be created.", [
      '@type' => $content_model_type,
      '@dest' => $pid,
    ]), 'warning');
  }

  // Make sure destructors are called as this script can run for a long time.
  unset($transform_xsl);
  unset($proc);
  unset($object);
  unset($image_datastream);
  unset($mods_datastream);
  unset($dc_datastream);
  unset($ocr_datastream);

  return $ret;
}

/**
 * Working but may require updating.
 */

/**
 * Generates csv input for newspaper ingest.
 */
function _bc_islandora_drush_generate_newspaper_csv($directory, $data) {
  $year = $data['year'];
  $month = $data['month'];
  $filename = $year && $month ? "$year-$month" : ($year ? "$year" : '');
  $filename .= 'pages.csv';
  $csv_filename = "$directory/$filename";
  $csv = fopen($csv_filename, 'w');
  $lines = [
    ["title", "date", "filename", "page number", "type", "extent"],
  ];
  $issues = [];

  foreach (scandir($directory) as $file) {
    if (mime_content_type("$directory/$file") == 'image/tiff') {
      $ts = strtotime(array_shift(explode('-', str_replace('BarnardBulletin-', '', $file))));
      $day = date('d', $ts);
      if ((!$month || ($month && date('m', $ts) == $month)) && (!$year || ($year && date('Y', $ts) == $year))) {
        $extent_cmd = 'ls ' . $directory . '/BarnardBulletin-' . date('Ymd', $ts) . '*.tiff | wc -l';
        $extent = trim(`$extent_cmd`);
        $page = preg_replace('/^0+/', '', str_replace('.tiff', '', end(explode('-', $file))));
        $date_title = date('F j, Y', $ts);
        if ($page == 1) {
          $lines[] = [
            "Barnard Bulletin, " . $date_title,
            $date_title,
            '',
            '',
            'issue',
            $extent,
          ];
        }
        $lines[] = [
          $date_title,
          $date_title,
          $file,
          $page,
          'page',
          $extent,
        ];
      }
    }
  }
  foreach ($lines as $line) {
    fputcsv($csv, $line);
  }
  fclose($csv);
  return $csv_filename;
}

/**
 * Creates a newspaper issue.
 */
function create_issue_object($pid, $data) {
  module_load_include('inc', 'islandora', 'RestConnection');

  $tuque = islandora_get_tuque_connection();
  $connection = $tuque->connection;
  $object = new NewFedoraObject($pid, $connection->repository);

  $title = $data[0];
  $date = date('Y-m-d', strtotime($data[1]));
  $date_short = date('Ymd', strtotime($date));
  $extent = $data[5];

  $dsid = 'MODS';

  $content_model_type = 'islandora:newspaperIssueCModel';

  // TODO should be able to supply this as an argument. (or via CSV?)
  $collection_pid = variable_get('bc_islandora_newspaper_pid', 'islandora:74');

  $xml = _bc_islandora_drush_np_issue_mods($title, $extent, $date, $date_short);

  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_type);
  $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $collection_pid);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', '1');
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'dateIssued', $date);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasLanguage', 'eng');

  $mods_xml = new DOMDocument();
  $mods_xml->loadXML($xml);

  $transform_xsl = new DOMDocument();
  $xsl_file = DRUPAL_ROOT . '/' . drupal_get_path('module', 'islandora_importer') . '/xsl/mods_to_dc.xsl';
  $transform_xsl->load($xsl_file);

  $proc = new XSLTProcessor();
  $proc->importStylesheet($transform_xsl);
  $dc = $proc->transformToXML($mods_xml);

  $object->label = $title;

  $dc_datastream = new NewFedoraDatastream('DC', 'X', $object, $connection->repository);

  $dc_datastream->content = $dc;
  $dc_datastream->mimetype = 'text/xml';
  $dc_datastream->label = 'Dublin Core Record for this object';

  $object->ingestDatastream($dc_datastream);

  $mods_datastream = new NewFedoraDatastream('MODS', 'X', $object, $connection->repository);

  $mods_datastream->setContentFromString($xml);
  $mods_datastream->mimetype = 'text/xml';
  $mods_datastream->label = 'MODS record';
  $mods_datastream->checksum = TRUE;
  $mods_datastream->checksumType = 'MD5';
  $mods_datastream->logMessage = 'MODS datastream created using Newspapers batch ingest script || SUCCESS';

  $object->ingestDatastream($mods_datastream);

  try {
    $object->state = 'A';
    $ingest = $tuque->repository->ingestObject($object);
  } catch (Exception $e) {
    unset($object);
    throw($e);
  }

  if ($ingest == TRUE) {
    drush_log(dt("Book object @pid created successfully\n", ['@pid' => $pid]), 'success');
  }
  else {
    drush_log(dt("There was an error creating the book object!\n", 'warning'));
  }
  unset($object);
  unset($mods_datastream);
  unset($dc_datastream);

  return $pid;
}

/**
 * Creates a page object.
 *
 * Being deprecated in favor of eventual object and classing, eventually.
 */
function create_page_object($file_path, $data, $collection_pid, $page_pid, $type) {
  module_load_include('inc', 'islandora', 'RestConnection');

  $connection = islandora_get_tuque_connection();
  $repository = $connection->repository;

  $pid = $page_pid;

  $object = $repository->constructObject($pid);

  // Set variables for MODS generation below. (Values differ depending on
  // $type.)
  switch ($type) {
    case 'newspaper':
      $date = $data[1];
      $date_short = date('Ymd', strtotime($date));
      $page_number = $data[3];
      $title = 'Barnard Bulletin';
      $label = $title . ', ' . $date . ', page ' . $page_number;
      $date_issued = '<dateIssued encoding="w3cdtf">' . date('Y-m-d', strtotime($date)) . '</dateIssued>';
      $date_created = '<dateCreated encoding="w3cdtf">' . date('Y-m-d', strtotime($date)) . '</dateCreated>';
      $id_short = 'BC12-03';
      $id_long = 'BC12-03_' . $date_short . '-' . $page_number;
      $file_name = $file_path . '/' . $data[2];
      $content_model_type = 'islandora:newspaperPageCModel';
      $genre = 'newspaper';
      $img_mime = 'image/tiff';
      break;

    case 'book':
      $date = $data[1];
      $date_short = date('Ymd', strtotime($date));
      $title = $data[0];
      $year = array_pop(explode(' ', $data[1]));
      $page_number = array_pop($data);
      $label = $title . ' ' . $year . ', page ' . $page_number;
      $date_issued = '<dateIssued encoding="iso8601">' . $data[3] . '</dateIssued>';
      $date_created = '<dateCreated encoding="iso8601">' . $data[3] . '</dateCreated>';
      $id_short = 'BC12-01';
      $id_long = 'BC12-01_' . $data[3] . '_' . str_pad($page_number, 3, '0', STR_PAD_LEFT);
      $file_name = $file_path;
      $content_model_type = 'islandora:pageCModel';
      $genre = 'yearbook';
      $img_mime = 'image/jp2';
      break;

    case 'pub':
      $content_model_type = 'islandora:pageCModel';
      $page_number = $data['page_number'];
      $img_mime = 'image/tiff';
      $file_name = $file_path;
      $label = $data['title'];

      $title = $label;
      $genre = $data['genre'];
      $host_title = $data['title_host'];
      $host_id = $data['host_id'];
      $date = $data['date'];
      $page_id = $data['page_id'];

      // Student publications MODS doesn't follow the book/newspaper template,
      // so we generate it here instead.
      $xml = _bc_islandora_drush_page_mods($type, $title, $genre, $host_title, $host_id, $page_id, $date, '');
      break;

    case 'ms':
      $content_model_type = 'islandora:manuscriptPageCModel';
      $page_number = $data['page_number'];
      $label = $data['title'];
      $file_name = $file_path;
      $xml = $data['xml'];
      $img_mime = 'image/tiff';
      break;
  }

  // Ingest a thumbnail of the first page to the parent object collection.
  if ($page_number == 1) {
    $rand = rand('100', '999');
    $tmp_file = '/tmp/tmp_file' . $rand . '.jpg';
    $command = 'convert -quiet -thumbnail 200x200 ' . $file_name . ' ' . $tmp_file;
    exec($command, $exec_output, $exec_return);
    if ($exec_return == 0) {
      $book_item = $repository->constructObject($collection_pid);
      $tn_datastream = $book_item->constructDatastream('TN');
      $tn_datastream->setContentFromFile($tmp_file, FALSE);
      $tn_datastream->label = 'Thumbnail';
      $tn_datastream->mimetype = 'image/jpg';
      $tn_datastream->checksum = TRUE;
      $tn_datastream->checksumType = 'MD5';
      $tn_datastream->logMessage = "TN derivative created by ImageMagick with command - $command || SUCCESS";
      $book_item->ingestDatastream($tn_datastream);
      unlink($tmp_file);
      drush_log('Updated parent object TN.', 'ok');
    }
  }

  $dsid = 'MODS';

  // Generate MODS here for book and newspaper pages.
  if (!isset($xml)) {
    $xml = _bc_islandora_drush_page_mods($type, $title, $genre, $label, $id_short, $id_long, $date_created, $date_issued);
  }

  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_type);
  $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $collection_pid);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isPageOf', $collection_pid);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', $page_number);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isPageNumber', $page_number);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSection', '1');
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasLanguage', 'eng');

  $mods_xml = new DOMDocument();
  $mods_xml->loadXML($xml);

  $transform_xsl = new DOMDocument();
  $xsl_file = DRUPAL_ROOT . '/' . drupal_get_path('module', 'islandora_importer') . '/xsl/mods_to_dc.xsl';
  $transform_xsl->load($xsl_file);

  $proc = new XSLTProcessor();
  $proc->importStylesheet($transform_xsl);
  $dc = $proc->transformToXML($mods_xml);

  $object->label = $label;

  $dc_datastream = $object->constructDatastream('DC');

  $dc_datastream->content = $dc;
  $dc_datastream->mimetype = 'text/xml';
  $dc_datastream->label = 'Dublin Core Record for this object';

  $object->ingestDatastream($dc_datastream);

  $mods_datastream = $object->constructDatastream('MODS');

  $mods_datastream->content = $xml;
  $mods_datastream->mimetype = 'text/xml';
  $mods_datastream->label = 'MODS record';
  $mods_datastream->checksum = TRUE;
  $mods_datastream->checksumType = 'MD5';
  $mods_datastream->logMessage = 'MODS datastream created using batch ingest script || SUCCESS';

  $object->ingestDatastream($mods_datastream);

  $image_datastream = $object->constructDatastream('OBJ');
  $image_datastream->setContentFromFile($file_name, FALSE);
  $image_datastream->label = $label;
  $image_datastream->mimetype = $img_mime;
  $image_datastream->checksum = TRUE;
  $image_datastream->checksumType = 'MD5';
  $image_datastream->logMessage = 'OBJ datastream created using batch ingest script || SUCCESS';

  $object->ingestDatastream($image_datastream);

  if (!empty($data['ocr'])) {
    $ocr_datastream = $object->constructDatastream('OCR');
    $ocr_datastream->setContentFromFile($data['ocr'], FALSE);
    $ocr_datastream->label = 'OCR';
    $ocr_datastream->mimetype = 'text/plain';
    $ocr_datastream->checksum = TRUE;
    $ocr_datastream->checksumType = 'MD5';
    $ocr_datastream->logMessage = 'OCR set from text file || SUCCESS';
    drush_log("OCR set from text file.", 'ok');
    $object->ingestDatastream($ocr_datastream);
  }
  drush_log("Trying to create page object.\n", 'ok');
  try {
    $object->state = 'A';
    $ingest = $connection->repository->ingestObject($object);
  } catch (Exception $e) {
    unset($object);
    throw($e);
  }

  if ($ingest == TRUE) {
    drush_log(dt("Page object @pid created successfully\n", ['@pid' => $object->id]), 'success');
  }
  else {
    drush_log(dt("There was an error creating the page object at @pid.\n", ['@pid' => $object->id]), 'error');
  }

  // Make sure destructors are called as this script can run for a long time.
  unset($object);
  unset($image_datastream);
  unset($mods_datastream);
  unset($dc_datastream);
  unset($ocr_datastream);

  return $pid;
}

/**
 * Sets and processes derivative-creation batches.
 *
 * @see bc_islandora.drush_batch.inc
 */
function _bc_islandora_create_derivatives($objects) {
  require_once 'bc_islandora.drush_batch.inc';
  foreach ($objects as $pid) {
    $book = islandora_object_load($pid);
    $pages = array_keys(islandora_paged_content_get_pages($book));
    $img_batch = bc_islandora_create_images_batch($book, $pages);
    $pdf_batch = bc_islandora_create_pdf_batch($book, $pages);
    $ocr_batch = islandora_paged_content_create_ocr_batch($book, $pages, [
      'language' => 'eng',
      'preprocess' => FALSE,
    ]);
    batch_set($img_batch);
    batch_set($pdf_batch);
    batch_set($ocr_batch);
  }
  drush_backend_batch_process();
}

/**
 * Creates a book object.
 */
function create_book_object($pid, $data) {
  module_load_include('inc', 'islandora', 'RestConnection');

  $tuque = islandora_get_tuque_connection();
  $connection = $tuque->connection;

  // Create the OBJ here, for FEDORA/ISLANDORA RELS-EXT/INT relationship->add
  // method.
  $object = new NewFedoraObject($pid, $connection->repository);

  $dsid = 'MODS';

  $content_model_type = 'islandora:bookCModel';

  // If we didn't get an explicit parent collection pid in $data, assume the
  // default book collection pid.
  $collection_pid = isset($data['collection_pid']) ?
    $data['collection_pid'] :
    variable_get('bc_islandora_yearbook_pid', 'islandora:bookCollection');

  // Generate the MODS XML datastream, unless MODS xml was passed in via $data.
  // (Also generate the RELS-EXT or RDF datastream.)
  if (!isset($data['xml'])) {
    $date = $data[3];
    $vol = explode(' ', $data[1]);
    $id_date = $vol[1];
    $title = $data[0] . ' ' . $id_date;
    $extent = $data[12] . ' pages';
    $description = $data[7];

    $xml = _bc_islandora_drush_book_mods($title, $date, $extent, $description, $id_date);

    $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_type);
    $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $collection_pid);
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', '1');
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'dateIssued', $date);
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasLanguage', 'eng');
  }
  // Just generate RDF if we got MODS in $data.
  else {
    $xml = $data['xml'];
    $date = $data['date'];
    $title = $data['title'];
    $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_type);
    $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $collection_pid);
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasLanguage', 'eng');
  }

  $mods_xml = new DOMDocument();
  $mods_xml->loadXML($xml);

  $transform_xsl = new DOMDocument();
  $xsl_file = DRUPAL_ROOT . '/' . drupal_get_path('module', 'islandora_importer') . '/xsl/mods_to_dc.xsl';
  $transform_xsl->load($xsl_file);

  $proc = new XSLTProcessor();
  $proc->importStylesheet($transform_xsl);
  $dc = $proc->transformToXML($mods_xml);

  $object->label = $title;

  $dc_datastream = new NewFedoraDatastream('DC', 'X', $object, $connection->repository);

  $dc_datastream->content = $dc;
  $dc_datastream->mimetype = 'text/xml';
  $dc_datastream->label = 'Dublin Core Record for this object';

  $object->ingestDatastream($dc_datastream);

  $mods_datastream = new NewFedoraDatastream('MODS', 'X', $object, $connection->repository);

  $mods_datastream->setContentFromString($xml);
  $mods_datastream->mimetype = 'text/xml';
  $mods_datastream->label = 'MODS record';
  $mods_datastream->checksum = TRUE;
  $mods_datastream->checksumType = 'MD5';
  $mods_datastream->logMessage = 'MODS datastream created using Newspapers batch ingest script || SUCCESS';

  $object->ingestDatastream($mods_datastream);

  try {
    $object->state = 'A';
    $ingest = $tuque->repository->ingestObject($object);
  } catch (Exception $e) {
    unset($object);
    throw($e);
  }

  if ($ingest == TRUE) {
    drush_log(dt("Book object @pid created successfully\n", ['@pid' => $pid]), 'success');
  }
  else {
    drush_log(dt("There was an error creating the book object!\n", 'warning'));
  }
  unset($object);
  unset($mods_datastream);
  unset($dc_datastream);

  return $pid;
}

/**
 * Helper function: returns structured data about a publication.
 */
function _bc_islandora_pub_data($xml_file, $pid = '', $type = '', $page_number = '') {
  $xml = file_get_contents($xml_file);
  $mods = simplexml_load_string($xml);
  $data = [
    'cm_pid' => 'islandora:collectionCModel',
    'xml' => $xml,
    'collection_pid' => variable_get('bc_islandora_pubs_pid', 'islandora:1022'),
    'date' => (string) $mods->originInfo->dateCreated,
    'title' => (string) $mods->titleInfo->title,
  ];
  if ($type == 'page') {
    $data['genre'] = (string) $mods->genre;
    $data['title_host'] = (string) $mods->titleInfo->title;
    $data['host_id'] = (string) $mods->relatedItem[1]->identifier;
    $data['page_id'] = (string) $mods->identifier . '_' . str_pad($page_number, 3, '0', STR_PAD_LEFT);
  }
  return $data;
}

/**
 * Helper function: processes (ingests or purges) publication pages.
 */
function _bc_islandora_process_pub_pages($parent_pid, $parent_xml_file, $path) {
  foreach (scandir($path) as $page) {
    if (preg_match('/^\./', $page) === 0 && preg_match('/\.tif$/', $page) !== 0) {
      $page_file = $path . '/' . $page;
      $page_number_padded = str_replace('.tif', '', end(explode('_', $page)));
      $page_number = preg_replace('/^0+/', '', $page_number_padded);
      $page_pid = $parent_pid . '-' . $page_number;
      $page_data = _bc_islandora_pub_data($parent_xml_file, $issue_pid, 'page', $page_number);
      $page_data['page_number'] = $page_number;
      $page_data['title'] .= ', page ' . $page_number;
      if (!islandora_object_load($page_pid)) {
        create_page_object($page_file, $page_data, $parent_pid, $page_pid, 'pub');
      }
    }
  }
}

/**
 * Creates a student publication (or manuscript) object.
 */
function create_pub_object($pid, $data) {
  module_load_include('inc', 'islandora', 'RestConnection');

  $tuque = islandora_get_tuque_connection();
  $connection = $tuque->connection;
  $repository = $tuque->repository;

  $dsid = 'MODS';

  $content_model_type = $data['cm_pid'];
  $collection_pid = $data['collection_pid'];

  $xml = $data['xml'];

  $mods_xml = new DOMDocument();
  $mods_xml->loadXML($xml);

  $transform_xsl = new DOMDocument();
  $xsl_file = DRUPAL_ROOT . '/' . drupal_get_path('module', 'islandora_importer') . '/xsl/mods_to_dc.xsl';
  $transform_xsl->load($xsl_file);

  $proc = new XSLTProcessor();
  $proc->importStylesheet($transform_xsl);
  $dc = $proc->transformToXML($mods_xml);

  $object = $repository->constructObject($pid);

  $object->label = $data['title'];

  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_type);
  $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $collection_pid);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasLanguage', 'eng');

  $dc_datastream = $object->constructDatastream('DC');

  $dc_datastream->content = $dc;
  $dc_datastream->mimetype = 'text/xml';
  $dc_datastream->label = 'Dublin Core Record for this object';

  $object->ingestDatastream($dc_datastream);

  $mods_datastream = $object->constructDatastream('MODS');

  $mods_datastream->setContentFromString($xml);
  $mods_datastream->mimetype = 'text/xml';
  $mods_datastream->label = 'MODS record';
  $mods_datastream->checksum = TRUE;
  $mods_datastream->checksumType = 'MD5';
  $mods_datastream->logMessage = 'MODS datastream created using Newspapers batch ingest script || SUCCESS';

  $object->ingestDatastream($mods_datastream);

  try {
    $object->state = 'A';
    $ingest = $tuque->repository->ingestObject($object);
  } catch (Exception $e) {
    unset($object);
    throw($e);
  }

  if ($ingest == TRUE) {
    drush_log(dt("@type object @pid created successfully\n", [
      '@type' => $content_model_type,
      '@pid' => $pid,
    ]), 'success');
  }
  else {
    drush_log(dt("Failed to create @type object @pid\n", [
      '@type' => $content_model_type,
      '@pid' => $pid,
    ]), 'warning');
  }
  $return = $object->id;
  unset($object);
  unset($mods_datastream);
  unset($dc_datastream);

  return $return;
}

function _map_rg1108_csv_rows(array $data, array $head = []) {
  $ret['id'] = $data[0];
  $ret['title'] = $data[1];
  $ret['date_exact'] = $data[2];
  $ret['extent'] = $data[3];
  $ret['description'] = $data[4];
  $ret['useAndReproduction'] = $data[5];
  $ret['accesscondition_local'] = $data[6];
  $ret['volume'] = $data[7];

  return $ret;
}

/**
 * Simple CSV row mapping based on the header of each row.
 *
 * @param array $data
 *   A structured CSV row.
 * @param array $head
 *   Column headings (NB UNUSED).
 *
 * @return array
 *   Chocolaty goodness.
 *
 * @todo header mapping.
 */
function _map_rg15_csv_rows(array $data, array $head = []) {
  $object_data_array['id'] = $data[0];
  $object_data_array['title'] = $data[1];
  // This can be automated by including a header row, and figuring out
  // how many 'topics' there are, their location, etc. The mods script
  // that fires after this CAN HANDLE multiples.
  $object_data_array['creators'][0]['name'] = $data[2];
  $object_data_array['creators'][0]['uri'] = $data[3];
  $object_data_array['resource_type'] = $data[4];
  $object_data_array['genre'] = $data[5];
  $object_data_array['genre_uri'] = $data[6];
  $object_data_array['date_exact'] = $data[7];
  $object_data_array['date_start_approx'] = $data[8];
  $object_data_array['date_end_approx'] = $data[9];
  // NB start and end date only present if date is approx range...
  $object_data_array['date_start_exact'] = $data[10];
  $object_data_array['date_end_exact'] = $data[11];
  $object_data_array['extent'] = $data[12];
  $object_data_array['description'] = $data[13];
  $object_data_array['location'] = $data[14];
  $object_data_array['related_item_title'] = $data[15];
  $object_data_array['related_item_pid'] = $data[16];
  // This can be automated by including a header row, and figuring out
  // how many 'topics' there are, their location, etc. The mods script
  // that fires after this CAN HANDLE multiples.
  $object_data_array['topics'][0]['topic'] = $data[17];
  $object_data_array['topics'][0]['uri'] = $data[18];
  $object_data_array['topics'][1]['topic'] = $data[19];
  $object_data_array['topics'][1]['uri'] = $data[20];
  $object_data_array['topics'][2]['topic'] = $data[21];
  $object_data_array['topics'][2]['uri'] = $data[22];
  $object_data_array['topics'][3]['topic'] = $data[23];
  $object_data_array['topics'][3]['uri'] = $data[24];
  $object_data_array['topics'][4]['topic'] = $data[25];
  $object_data_array['topics'][4]['uri'] = $data[26];
  $object_data_array['topics'][5]['topic'] = $data[27];
  $object_data_array['topics'][5]['uri'] = $data[28];
  $object_data_array['topics'][6]['topic'] = $data[29];
  $object_data_array['topics'][6]['uri'] = $data[30];

  return $object_data_array;
}

/**
 * Simple CSV row mapping based on the header of each row.
 *
 * @param array $data
 *   A structured CSV row.
 * @param array $head
 *   Column headings.
 *
 * @return array
 *   Chocolaty goodness.
 *
 * @todo everything!
 */
function _map_csv_row_to_array(array $data, array $head) {
  $permitted_row_titles = [
    'id',
    'title',
    'creator_name',
    'creator_uri',
    'resource_type',
    'genre',
    'genre_uri',
    'date_exact',
    'date_start_approx',
    'date_end_approx',
    'date_start_exact',
    'date_end_exact',
    'extent',
    'description',
    'location',
    'related_item_title',
    'related_item_pid',
    'topics_topic',
    'topics_uri',
  ];
  $mandatory_row_titles = ['id', 'title'];

  if (!in_array($mandatory_row_titles, $head)) {
    return NULL;
  }
  // Function before combine to de-dupe head and append unique id to each (int).
  // Position in data corresponds to head so map head[i] = data[i] and ret arr.
  // @TODO have the appropriate data contained - ala, if it is a date make sure it validates as a date.
  $object_data_array = array_combine($head, $data);
  print_r($object_data_array);

  return $object_data_array;
}
