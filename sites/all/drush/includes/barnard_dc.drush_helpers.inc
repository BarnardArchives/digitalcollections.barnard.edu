<?php

/**
 * @file
 * Helper functions invoked in custom drush methods.
 *
 * @see bc_islandora.drush.inc
 * @see https://github.com/roblib/scripts/tree/master/drush/drupal7
 */

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

  $content_model_pid = 'islandora:newspaperIssueCModel';

  // TODO should be able to supply this as an argument. (or via CSV?)
  $collection_pid = variable_get('bc_islandora_newspaper_pid', 'islandora:74');

  $xml = <<<ISSUE_MODS
    <mods xmlns="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.loc.gov/mods/v3 http://www.loc.gov/mods/v3/mods-3-5.xsd">
      <titleInfo>
        <title>$title</title>
      </titleInfo>
      <genre authority="marcgt">newspaper</genre>
      <physicalDescription>
        <form authority="marcform">print</form>
        <form authority="marcform">electronic</form>
        <extent>$extent pages</extent>
        <internetMediaType>image/tiff</internetMediaType>
        <digitalOrigin>reformatted digital</digitalOrigin>
      </physicalDescription>
      <relatedItem type="host"> 
        <titleInfo> 
          <title>Barnard Bulletin</title> 
        </titleInfo> 
        <typeOfResource>text</typeOfResource> 
        <identifier type="local">BC12-03</identifier> 
      </relatedItem> 
      <originInfo>
        <dateIssued encoding="w3cdtf">$date</dateIssued>
        <dateCreated encoding="w3cdtf">$date</dateCreated>
      </originInfo>
      <identifier type="local">BC12-03_$date_short</identifier>
    </mods> 
ISSUE_MODS;

  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_pid);
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
  }
  catch(Exception $e) {
    unset($object);
    throw($e);
  }

  if ($ingest == TRUE) {
    drush_log(dt("Book object @pid created successfully\n", array('@pid' => $pid)), 'success');
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
 * Creates a book object.
 */
function create_book_object($pid, $data) {
  module_load_include('inc', 'islandora', 'RestConnection');

  $tuque = islandora_get_tuque_connection();
  $connection = $tuque->connection;
  
  // Create the OBJ here, for FEDORA/ISLANDORA RELS-EXT/INT relationship->add method.
  $object = new NewFedoraObject($pid, $connection->repository);

  $dsid = 'MODS';

  $content_model_pid = 'islandora:bookCModel';

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

    $xml = <<<BOOK_MODS
      <mods xmlns="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.loc.gov/mods/v3 http://www.loc.gov/mods/v3/mods-3-5.xsd">
        <titleInfo>
          <title>$title</title>
        </titleInfo>

        <name type="corporate" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n79065105">
          <namePart>Barnard College</namePart>
          <role>
            <roleTerm type="text" authority="marcrelator">Publisher</roleTerm>
          </role>
        </name>

        <typeOfResource>text</typeOfResource>

        <genre authority="marcgt">yearbook</genre>
  
        <originInfo eventType="publication">
          <place>
            <placeTerm type="code" authority="marccountry">nyu</placeTerm>
            <placeTerm type="text">New York (N.Y.)</placeTerm>
          </place>
          <publisher>Barnard College</publisher>
          <dateIssued encoding="iso8601" keyDate="yes">$date</dateIssued>
          <dateCreated encoding="iso8601" keyDate="yes">$date</dateCreated>
          <issuance>continuing</issuance>
        </originInfo>

        <language>
          <languageTerm type="code" authority="iso639-2b">eng</languageTerm>
        </language>

        <physicalDescription>
          <form authority="marcform">print</form>
          <form authority="marcform">electronic</form>
          <extent>$extent</extent>
          <internetMediaType>image/jpeg2000</internetMediaType>
          <digitalOrigin>reformatted digital</digitalOrigin>
        </physicalDescription>

        <abstract>$description</abstract>

        <subject authority="lcsh" authorityURI="http://id.loc.gov/authorities/subjects" valueURI="http://id.loc.gov/authorities/subjects/sh85147738">
          <topic>Women's colleges</topic>
        </subject>
        <subject authority="lcsh" authorityURI="http://id.loc.gov/authorities/subjects" valueURI="http://id.loc.gov/authorities/subjects/sh85147501">
          <topic>Women college students</topic>
        </subject>

        <subject authority="lcsh" authorityURI="http://id.loc.gov/authorities/subjects" valueURI="http://id.loc.gov/authorities/subjects/sh85129244">
          <topic>Student activities</topic>
        </subject>

        <subject>
          <name type="corporate" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n79065105">
          </name>
        </subject>

        <relatedItem type="host"> 
          <titleInfo> 
            <title>Mortarboard</title> 
          </titleInfo> 
          <typeOfResource>text</typeOfResource> 
          <identifier type="local">BC12-01</identifier> 
        </relatedItem> 

        <identifier type="local">BC12-01_$id_date</identifier>

        <location> 
          <physicalLocation authority="marcorg">NNC</physicalLocation> 
          <physicalLocation authority="marcorg">NyNyBAW</physicalLocation> 
          <physicalLocation>Barnard College Library (New York, N.Y.)</physicalLocation> 
        </location> 

        <accessCondition type="useAndReproduction">This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. Contact the Archives at archives@barnard.edu for more information on access and use, including commercial uses.</accessCondition>

        <recordInfo>
          <recordContentSource>Barnard Archives and Special Collections</recordContentSource>
          <recordContentSource authority="marcorg">NNC</recordContentSource>
          <recordContentSource authority="marcorg">NyNyBAW</recordContentSource>
          <languageOfCataloging>
            <languageTerm authority="iso639-2b">eng</languageTerm>
          </languageOfCataloging>
        </recordInfo>
      </mods>
BOOK_MODS;
    $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_pid);
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
    $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_pid);
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
  }
  catch(Exception $e) {
    unset($object);
    throw($e);
  }

  if ($ingest == TRUE) {
    drush_log(dt("Book object @pid created successfully\n", array('@pid' => $pid)), 'success');
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
 * Creates a student publication (or manuscript) object.
 */
function create_pub_object($pid, $data) {
  module_load_include('inc', 'islandora', 'RestConnection');

  $tuque = islandora_get_tuque_connection();
  $connection = $tuque->connection;

  $dsid = 'MODS';

  // $content_model_pid = 'islandora:collectionCModel';
  $content_model_pid = $data['cm_pid'];
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

  $object = new NewFedoraObject($pid, $connection->repository);

  $object->label = $data['title'];

  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_pid);
  $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $collection_pid);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasLanguage', 'eng');

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
  }
  catch(Exception $e) {
    unset($object);
    throw($e);
  }

  if ($ingest == TRUE) {
    drush_log(dt("Publication collection object @pid created successfully\n", array('@pid' => $pid)), 'success');
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
 */
function create_page_object($file_path, $data, $collection_pid, $page_pid, $type) {
  $connection = islandora_get_tuque_connection();

  $pid = $page_pid;

  $object = new NewFedoraObject($pid, $connection->repository);

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
      $content_model_pid = 'islandora:newspaperPageCModel';
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
      $content_model_pid = 'islandora:pageCModel';
      $genre = 'yearbook';
      $img_mime = 'image/jp2';
      break;

    case 'pub':
      $content_model_pid = 'islandora:pageCModel';
      $page_number = $data['page_number'];
      $img_mime = 'image/tiff';
      $file_name = $file_path;
      $label = $data['title'];

      $title = $label;
      $genre = $data['genre'];
      $title_host = $data['title_host'];
      $host_id = $data['host_id'];
      $date = $data['date'];
      $page_id = $data['page_id'];

      // Student publications MODS doesn't follow the book/newspaper template,
      // so we generate it here instead.
      $xml = <<<PAGE_MODS
        <mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
          <titleInfo>
            <title>$title</title>
          </titleInfo>
          <genre authority="lcgft" authorityURI="http://id.loc.gov/authorities/genreForms" valueURI="http://id.loc.gov/authorities/genreForms/gf2014026139">$genre</genre>
          <relatedItem type="host" displayLabel="Record Group">
            <titleInfo>
              <title>Student Publications</title>
            </titleInfo>
          </relatedItem>
          <relatedItem type="host">
            <titleInfo>
              <title>$title_host</title>
            </titleInfo>
            <typeOfResource>text</typeOfResource>
            <identifier type="local">$host_id</identifier>
          </relatedItem>
          <identifier type="local">$page_id</identifier>
          <originInfo>
            <dateCreated encoding="w3cdtf">$date</dateCreated>
          </originInfo>
        </mods>
PAGE_MODS;
      break;
    case 'ms':
      $page_number = $data['page_number'];
      $label = $data['title'];
      $file_name = $file_path;
      $content_model_pid = 'islandora:manuscriptPageCModel';
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
      $book_item = new FedoraObject($collection_pid, $connection->repository);
      $tn_datastream = new NewFedoraDatastream('TN', 'M', $book_item, $connection->repository);
      $tn_datastream->setContentFromFile($tmp_file, FALSE);
      $tn_datastream->label = 'Thumbnail';
      $tn_datastream->mimetype = 'image/jpg';
      $tn_datastream->checksum = TRUE;
      $tn_datastream->checksumType = 'MD5';
      $tn_datastream->logMessage = "TN derivative created by ImageMagick with command - $command || SUCCESS";
      $book_item->ingestDatastream($tn_datastream);
      unlink($tmp_file);
    }
  }

  $dsid = 'MODS';

  // Generate MODS here for book and newspaper pages.
  if (!isset($xml)) {
    $xml = <<<PAGE_MODS
      <mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <titleInfo>
          <title>$label</title>
        </titleInfo>
        <genre authority="marcgt">$genre</genre>
        <originInfo>
          $date_issued
          $date_created
        </originInfo>
        <relatedItem type="host"> 
          <titleInfo> 
            <title>$title</title> 
          </titleInfo> 
          <typeOfResource>text</typeOfResource> 
          <identifier type="local">$id_short</identifier> 
        </relatedItem> 
        <identifier type="local">$id_long</identifier>
      </mods>
PAGE_MODS;
  }
  
  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model_pid);
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

  $dc_datastream = new NewFedoraDatastream('DC', 'X', $object, $connection->repository);

  $dc_datastream->content = $dc;
  $dc_datastream->mimetype = 'text/xml';
  $dc_datastream->label = 'Dublin Core Record for this object';

  $object->ingestDatastream($dc_datastream);

  $mods_datastream = new NewFedoraDatastream('MODS', 'X', $object, $connection->repository);

  $mods_datastream->content = $xml;
  $mods_datastream->mimetype = 'text/xml';
  $mods_datastream->label = 'MODS record';
  $mods_datastream->checksum = TRUE;
  $mods_datastream->checksumType = 'MD5';
  $mods_datastream->logMessage = 'MODS datastream created using Newspapers batch ingest script || SUCCESS';

  $object->ingestDatastream($mods_datastream);

  $image_datastream = new NewFedoraDatastream('OBJ', 'M', $object, $connection->repository);
  $image_datastream->setContentFromFile($file_name, FALSE);
  // $image_datastream->label = 'TIFF image';
  $image_datastream->label = $label;
  // $image_datastream->mimetype = 'image/tiff';
  $image_datastream->mimetype = $img_mime;
  $image_datastream->checksum = TRUE;
  $image_datastream->checksumType = 'MD5';
  $image_datastream->logMessage = 'OBJ datastream created using Newspapers batch ingest script || SUCCESS';

  $object->ingestDatastream($image_datastream);

  try {
    $object->state = 'A';
    $ingest = $connection->repository->ingestObject($object);
  }
  catch (Exception $e) {
    unset($object);
    throw($e);
  }

  if ($ingest == TRUE) {
    drush_log(dt("Page object @pid created successfully\n", array('@pid' => $pid)), 'success');
  }
  else {
    drush_log(dt("There was an error creating the page object!\n"), 'error');
  }

  // Make sure destructors are called as this script can run for a long time.
  unset($object);
  unset($image_datastream);
  unset($mods_datastream);
  unset($dc_datastream);

  return $pid;
}

/**
 * Creates a pdf object.
 */
function create_pdf_object($pdf, $xml) {
  $connection = islandora_get_tuque_connection();
  $mods_xml = new DOMDocument();
  $mods_xml->loadXML($xml);
  $simple_mods_xml = simplexml_load_string($xml);
  $label = (string) $simple_mods_xml->titleInfo->title;
  print "$label\n";
  $obj_pid = 'bc:oh-' . str_replace(' ', '-', strtolower(str_replace('Oral history of ', '', $label)));
  $collection_pid = variable_get('bc_islandora_oh_pid', 'islandora:990');

  if ($object = islandora_object_load($obj_pid)) {
    drush_log(dt('Object exists at @pid; skipping...', array('@pid' => $obj_pid)), 'ok');
    return $obj_pid;
  }
  else {
    $object = new NewFedoraObject($obj_pid, $connection->repository);
  }

  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'sp_pdf');
  $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $collection_pid);

  $transform_xsl = new DOMDocument();
  $xsl_file = DRUPAL_ROOT . '/' . drupal_get_path('module', 'islandora_importer') . '/xsl/mods_to_dc.xsl';
  $transform_xsl->load($xsl_file);

  $proc = new XSLTProcessor();
  $proc->importStylesheet($transform_xsl);
  $dc = $proc->transformToXML($mods_xml);

  $object->label = $label;

  $dc_datastream = new NewFedoraDatastream('DC', 'X', $object, $connection->repository);

  $dc_datastream->content = $dc;
  $dc_datastream->mimetype = 'text/xml';
  $dc_datastream->label = 'Dublin Core Record for this object';

  $object->ingestDatastream($dc_datastream);
  $mods_datastream = new NewFedoraDatastream('MODS', 'X', $object, $connection->repository);

  $mods_datastream->content = $xml;
  $mods_datastream->mimetype = 'text/xml';
  $mods_datastream->label = 'MODS record';
  $mods_datastream->checksum = TRUE;
  $mods_datastream->checksumType = 'MD5';
  $mods_datastream->logMessage = 'MODS datastream created using PDF batch ingest script || SUCCESS';

  $object->ingestDatastream($mods_datastream);
  $pdf_datastream = new NewFedoraDatastream('OBJ', 'M', $object, $connection->repository);
  $pdf_datastream->setContentFromFile($pdf, FALSE);
  $pdf_datastream->label = $label;
  $pdf_datastream->mimetype = 'application/pdf';
  $pdf_datastream->checksum = TRUE;
  $pdf_datastream->checksumType = 'MD5';
  $pdf_datastream->logMessage = 'OBJ datastream created using PDF batch ingest script || SUCCESS';

  $object->ingestDatastream($pdf_datastream);

  try {
    $object->state = 'A';
    $ingest = $connection->repository->ingestObject($object);
  }
  catch (Exception $e) {
    unset($object);
    throw($e);
  }

  if ($ingest == TRUE) {
    drush_log(dt("Page object @pid created successfully\n", array('@pid' => $pid)), 'success');
  }
  else {
    drush_log(dt("There was an error creating the page object!\n"), 'error');
  }

  // Make sure destructors are called as this script can run for a long time.
  unset($object);
  unset($pdf_datastream);
  unset($mods_datastream);
  unset($dc_datastream);

  return $obj_pid;
}
