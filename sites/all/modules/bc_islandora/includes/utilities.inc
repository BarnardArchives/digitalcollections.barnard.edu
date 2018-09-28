<?php

/**
 * @file
 * Utility functions for bc_islandora.
 */

/**
 * Generates a cache id for $object.
 */
function _barnard_islandora_get_transcript_cid($object) {
  $object_mods = isset($object['MODS']) ? simplexml_load_string($object['MODS']->getContent(NULL)) : NULL;
  $object_cid = $object_mods && $object_mods->identifier ? $object_mods->identifier : str_replace(':', '', $object->id);
  $object_cid .= '_transcript';
  return $object_cid;
}

/**
 * Generates a transcript for $object.
 */
function _barnard_islandora_generate_transcript($object) {
  $object_ocr = '';
  if ($pages = islandora_paged_content_get_pages($object)) {
    foreach ($pages as $page_pid => $page) {
      $page_object = islandora_object_load($page_pid);
      if (isset($page_object['OCR'])) {
        $object_ocr .= $page_object['OCR']->getContent(NULL);
      }
    }
  }
  elseif (isset($object['OCR'])) {
    $object_ocr .= $object['OCR']->getContent(NULL);
  }

  return $object_ocr;
}

/**
 * Generates a date format string for the supplied dates.
 */
function _barnard_islandora_get_date_format($date_1, $date_2) {
  $date_format = max(
    count(explode('/', $date_1)),
    count(explode('/', $date_2))
  );
  switch ($date_format) {
    case 1:
      return 'Y';

    case 2:
      return 'Y/m';

    case 3:
      return 'Y/m/d';
  }
  return NULL;
}

/**
 * Returns the latest date corresponding to $date_to.
 */
function _barnard_islandora_get_latest_possible_date($date_to) {
  $date_count = count(explode('/', $date_to));
  switch ($date_count) {
    // We always end a year in December, which has 31 days.
    case 1:
      return '/12/31';

    // Figure out the last day of the given month.
    case 2:
      $dto = new DateTime($date_to . '/1');
      $dto->add(new DateInterval('P1M'))->sub(new DateInterval('P1D'));
      return '/' . $dto->format('d');

    case 3:
      return NULL;
  }
  return NULL;
}
