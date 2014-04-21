<?php

function create_issue_object($data) {
  module_load_include('inc', 'islandora', 'RestConnection');
  
  $tuque = islandora_get_tuque_connection();
  $connection = $tuque->connection;
  
  $title = $data[0];
  $date = $data[1];
 
  $pid = 'bulletin:' . date('Ymd', strtotime($date));

  $dsid = 'MODS';

  $content_model_pid = 'islandora:newspaperIssueCModel';

  // TODO should be able to supply this as an argument. (or via CSV?)
  $collection_pid = 'islandora:200';

  $xml = <<<ISSUE_MODS
    <mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink">
      <titleInfo>
        <title>$title</title>
      </titleInfo>
      <typeOfResource>text</typeOfResource>
      <genre authority="marcgt">newspaper</genre>
      <originInfo>
        <publisher>Barnard College</publisher>
        <place>
          <placeTerm type="text">New York, NY</placeTerm>
        </place>
        <dateIssued>$date</dateIssued>
        <issuance>serial</issuance>
        <frequency authority="marcfrequency">Weekly</frequency></originInfo>
      <subject>
        <geographic>Barnard College</geographic>
      </subject>
    </mods>
ISSUE_MODS;
  
  $rdf_string = <<<ISSUE_RDF
    <rdf:RDF xmlns:fedora="info:fedora/fedora-system:def/relations-external#" xmlns:fedora-model="info:fedora/fedora-system:def/model#" xmlns:islandora="http://islandora.ca/ontology/relsext#" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#">
      <rdf:Description rdf:about="info:fedora/$pid">
        <fedora-model:hasModel rdf:resource="info:fedora/$content_model_pid"></fedora-model:hasModel>
        <fedora:isMemberOf rdf:resource="info:fedora/$collection_pid"></fedora:isMemberOf>
        <islandora:isSequenceNumber>1</islandora:isSequenceNumber>
        <islandora:dateIssued>1901-01-07</islandora:dateIssued>
      </rdf:Description>
    </rdf:RDF>
ISSUE_RDF;
 
  $mods_xml = new DOMDocument();
  $mods_xml->loadXML($xml);

  $transform_xsl = new DOMDocument();
  $xsl_file = DRUPAL_ROOT . '/' . drupal_get_path('module', 'islandora_importer') . '/xsl/mods_to_dc.xsl';
  $transform_xsl->load($xsl_file);

  $proc = new XSLTProcessor();
  $proc->importStylesheet($transform_xsl);
  $dc = $proc->transformToXML($mods_xml);

  $object = new NewFedoraObject($pid, $connection->repository);
  
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
  
  $rels_datastream = new NewFedoraDatastream('RELS-EXT', 'X', $object, $connection->repository);
  $rels_datastream->setContentFromString($rdf_string);
  $rels_datastream->label = 'RELS-EXT';
  $rels_datastream->mimetype = 'text/xml';
  $rels_datastream->logMessage = 'RELS-EXT datastream created using Newspapers batch ingest script || SUCCESS';

  $object->ingestDatastream($rels_datastream);  
  try {
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
  unset($rels_datastream);
  unset($dc_datastream);
  
  return $pid;
}

function create_page_object($file_path, $data, $collection_pid) {
  $connection = islandora_get_tuque_connection();

  $page_number = $data[3];
  $date = $data[1];
  $title = $data[0];
  
  $file_name = $file_path . '/' . $data[2];

  if ($page_number == 1) {
    $rand = rand('100', '999');
    $tmp_file = '/tmp/tmp_file' . $rand . '.jpg';
    $command = 'convert -thumbnail 200x200 ' . $file_name . ' ' . $tmp_file;
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
  $label = $title . ' - page ' . $page_number;

  $pid = 'bulletin:' . date('Ymd', strtotime($date)) . '-' . $page_number;
  var_export($pid);

  $dsid = 'MODS';

  $content_model_pid = 'islandora:newspaperPageCModel';

  $xml = <<<PAGE_MODS
<?xml version="1.0" encoding="UTF-8"?>
  <mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
    <titleInfo>
      <title>$label</title>
    </titleInfo>
    <identifier type="PID">$pid</identifier>
</mods>
PAGE_MODS;
  
  $rdf_string = <<<PAGE_RDF
    <rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:fedora="info:fedora/fedora-system:def/relations-external#" xmlns:fedora-model="info:fedora/fedora-system:def/model#" xmlns:islandora="http://islandora.ca/ontology/relsext#">
      <rdf:Description rdf:about="info:fedora/$pid">
        <fedora-model:hasModel rdf:resource="info:fedora/$content_model_pid"></fedora-model:hasModel>
        <islandora:hasLanguage></islandora:hasLanguage>
        <islandora:isPageOf rdf:resource="info:fedora/$collection_pid"></islandora:isPageOf>
        <islandora:isSequenceNumber>$page_number</islandora:isSequenceNumber>
        <islandora:isPageNumber>$page_number</islandora:isPageNumber>
        <islandora:isSection>1</islandora:isSection>
        <fedora:isMemberOf rdf:resource="info:fedora/$collection_pid"></fedora:isMemberOf>
      </rdf:Description>
    </rdf:RDF>
PAGE_RDF;

  $mods_xml = new DOMDocument();
  $mods_xml->loadXML($xml);

  $transform_xsl = new DOMDocument();
  $xsl_file = DRUPAL_ROOT . '/' . drupal_get_path('module', 'islandora_importer') . '/xsl/mods_to_dc.xsl';
  $transform_xsl->load($xsl_file);

  $proc = new XSLTProcessor();
  $proc->importStylesheet($transform_xsl);
  $dc = $proc->transformToXML($mods_xml);

  $object = new NewFedoraObject($pid, $connection->repository);

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
  $image_datastream->label = 'TIFF image';
  $image_datastream->mimetype = 'image/tiff';
  $image_datastream->checksum = TRUE;
  $image_datastream->checksumType = 'MD5';
  $image_datastream->logMessage = 'OBJ datastream created using Newspapers batch ingest script || SUCCESS';
  
  $object->ingestDatastream($image_datastream);
  
  $rels_datastream = new NewFedoraDatastream('RELS-EXT', 'X', $object, $connection->repository);
  $rels_datastream->setContentFromString($rdf_string);
  $rels_datastream->label = 'Fedora Object to Object Relationship Metadata.';
  $rels_datastream->mimetype = 'text/xml';
  $rels_datastream->logMessage = 'RELS-EXT datastream created using Newspapers batch ingest script || SUCCESS';
  
  $object->ingestDatastream($rels_datastream);
  try {
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
  
  unset($object);//make sure destructors are called as this script can run for a long time.
  unset($image_datastream);
  unset($mods_datastream);
  unset($rels_datastream);
  unset($dc_datastream);

  return $pid;
}
