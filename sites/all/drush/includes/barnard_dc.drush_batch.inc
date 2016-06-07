<?php

/**
 * @file
 * Modified versions of standard islandora batch methods.
 *
 * We reimplemented these functions in order to give the batch-generated
 * datastreams a non-generic label taken from MODS data (specifically the
 * "identifier" field).
 */

module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
module_load_include('inc', 'islandora_paged_content', 'includes/batch');
module_load_include('inc', 'islandora_large_image', 'includes/derivatives');

/**
 * Gets the batch definition for creating TN, JPG, JP2 datastreams.
 *
 * @param AbstractObject $paged_content
 *   The object the pages belong to.
 * @param array $pages
 *   All the pages to process.
 *
 * @return array
 *   The batch definition.
 */
function bc_islandora_create_images_batch(AbstractObject $paged_content, array $pages) {
  return array(
    'operations' => array(
      array('bc_islandora_create_images_batch_operation', array(
          $paged_content->id, $pages)),
    ),
    'finished' => 'islandora_paged_content_batch_finished',
    'title' => t('Creating Images for @label ...', array('@label' => $paged_content->label)),
    'init_message' => t('Preparing to create images for @label ...', array('@label' => $paged_content->label)),
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    // 'file' => drupal_get_path('module', 'bc_islandora') . '/includes/bc_islandora.drush_batch.inc',
    'file' => DRUPAL_ROOT . '/sites/all/drush/includes/barnard_dc.drush_batch.inc',
  );
}

/**
 * Batch operation for creating the TN, JPG, JP2 datastreams.
 *
 * @param AbstractObject $paged_content_id
 *   The book the pages belong to.
 * @param array $pages
 *   All the pages to create images for.
 * @param array $context
 *   The context of this batch operation.
 */
function bc_islandora_create_images_batch_operation($paged_content_id, array $pages, &$context) {
  $paged_content = islandora_object_load($paged_content_id);
  $page = islandora_paged_content_start_batch_operation($pages, $context);
  if ($page) {
    $page = islandora_object_load($page);
    $context['results']['successful'][] = bc_islandora_derive_image_datastreams($page);
    if (islandora_paged_content_end_batch_operation($context, 1, array('Created images for @successful of @total pages.'))) {
      islandora_paged_content_update_paged_content_thumbnail($paged_content);
    }
    $context['message'] = t('Creating images for page @page of @total ...', array(
                            '@page' => $context['sandbox']['progress'],
                            '@total' => $context['sandbox']['total']));
  }
}

/**
 * Creates and adds all the image derivatives for the given Page object.
 *
 * @param AbstractObject $object
 *   The page object that the derivative will be generated for.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function bc_islandora_derive_image_datastreams(AbstractObject $object) {
  $ret = bc_islandora_create_all_derivatives($object);
  if ($ret) {
    // Add dimensions for the JP2.
    islandora_paged_content_add_dimensions_relationships($object);
  }
  return $ret;
}

/**
 * Returns an array of names of functions for generating derivatives.
 */
function bc_islandora_create_all_derivatives(AbstractObject $object) {
  $ret = array();
  foreach (array('JPG', 'TN', 'JP2') as $dsid) {
    if ($dsid != 'JPG') {
      $function = 'islandora_large_image_create_' . strtolower($dsid) . '_derivative';
    }
    // We use our own helper function to generate JPGs.
    else {
      $function = 'bc_islandora_create_jpg_derivative';
      print "Got function: $function\n";
    }
    $ret[] = $function($object);
  }
  return $ret;
}

/**
 * Generates a JPG derivative.
 *
 * Modified version of default islandora function; modified to provide DS label
 * from MODS identifier data.
 */
function bc_islandora_create_jpg_derivative(AbstractObject $object) {
  unset($object['JPG']);
  $new_label = _mods_id_label($object, 'JPG');

  $base_name = str_replace(':', '-', $object->id);
  if ($force || !isset($object['JPG'])) {
    $to_return = array(
      'success' => FALSE,
      'messages' => array(),
    );

    $uploaded_file = islandora_large_image_get_uploaded_file($object);
    $args = array();
    $args[] = '-resize ' . escapeshellarg("600 x 800");
    $args[] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
    $derivative_file = islandora_large_image_imagemagick_convert($uploaded_file, "temporary://{$base_name}_JPG.jpg", $args);
    if ($derivative_file === FALSE) {
      $to_return['messages'][] = array(
        'message' => t('Failed to create JPG derivative.'),
        'type' => 'watchdog',
        'severity' => WATCHDOG_WARNING,
      );
    }
    else {
      // I modified this one line.
      $added_successfully = islandora_large_image_add_datastream($object, 'JPG', $derivative_file, 'image/jpeg', $new_label);
      if (TRUE === $added_successfully) {
        $to_return['messages'][] = array(
          'message' => t('Created JPG derivative.'),
          'type' => 'dsm',
          'severity' => 'status',
        );
        $to_return['success'] = TRUE;
      }
      else {
        $to_return['messages'][] = array(
          'message' => t('Failed to add JPG derivative to the object. Error message: @message', array('@message' => $added_successfully)),
          'type' => 'dsm',
          'severity' => 'warning',
        );
      }
    }

    file_unmanaged_delete($uploaded_file);
    file_unmanaged_delete($derivative_file);
    return $to_return;
  }
}

/**
 * Gets the batch definition for creating a PDF from the given pages.
 *
 * This also updates each page object with a corresponding pdf.
 *
 * @param AbstractObject $paged_content
 *   The paged_content the pages belong to.
 * @param array $pages
 *   All the pages to include in the PDF.
 * @param array $options
 *   Options to pass into convert.
 *
 * @return array
 *   The batch definition.
 */
function bc_islandora_create_pdf_batch(AbstractObject $paged_content, array $pages, array $options) {
  return array(
    'operations' => array(
      array('bc_islandora_create_pdf_page_batch_operation', array(
          $pages, $options)),
      array('bc_islandora_create_pdf_paged_content_batch_operation', array($paged_content->id)),
    ),
    'finished' => 'islandora_paged_content_batch_finished',
    'title' => t('Creating PDF from @label ...', array('@label' => $paged_content->label)),
    'init_message' => t('Preparing to create PDF from @label ...', array('@label' => $paged_content->label)),
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    // 'file' => drupal_get_path('module', 'bc_islandora') . '/includes/bc_islandora.drush_batch.inc',
    'file' => DRUPAL_ROOT . '/sites/all/drush/includes/barnard_dc.drush_batch.inc',
  );
}

/**
 * Batch operation for combining PDF files.
 *
 * @param string $paged_content_id
 *   The paged content identifier to store the PDF file.
 * @param array $context
 *   The context of this batch operation.
 */
function bc_islandora_create_pdf_paged_content_batch_operation($paged_content_id, &$context) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $paged_content = islandora_object_load($paged_content_id);
  if (empty($context['results']['pages'])) {
    return;
  }
  islandora_paged_content_start_batch_operation($context['results']['pages'], $context);
  $pdf_file = drupal_realpath("temporary://{$paged_content->id}_PDF.pdf");
  // Combined # of pages at a time.
  $files = array_slice($context['results']['pages'], $context['sandbox']['progress'], 10);
  $append = $context['sandbox']['progress'] != 0;
  $context['results']['successful'][] = $append ? islandora_paged_content_pdf_append($pdf_file, $files) : islandora_paged_content_pdf_combine($files, $pdf_file);
  $finished_message = array('Created PDF with @pages pages.', array('@pages' => count($context['results']['pages'])));
  if (islandora_paged_content_end_batch_operation($context, count($files), $finished_message)) {
    // I modified this line to add the MODS id label.
    islandora_paged_content_update_datastream($paged_content, $pdf_file, _mods_id_label($paged_content, 'PDF'));
    $context['results']['files'][] = $pdf_file;
  }
  $context['message'] = t('Adding page @page of @total to the PDF ...', array(
                          '@page' => $context['sandbox']['progress'],
                          '@total' => $context['sandbox']['total']));
}


/**
 * Batch operation for converting a page into a PDF.
 *
 * @param array $pages
 *   All the pages to convert.
 * @param array $options
 *   Options to pass into convert.
 * @param array $context
 *   The context of this batch operation.
 */
function bc_islandora_create_pdf_page_batch_operation(array $pages, array $options, &$context) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $page = islandora_paged_content_start_batch_operation($pages, $context);
  if ($page) {
    $page = islandora_object_load($page);
    $pdf_file = islandora_paged_content_page_derive_pdf($page, $options);
    $context['results']['files'][] = $pdf_file;
    $context['results']['pages'][] = $pdf_file;
    // I modified this line to add the MODS id label.
    $context['results']['successful'][] = islandora_paged_content_update_datastream($page, $pdf_file, _mods_id_label($page, 'PDF'));
    islandora_paged_content_end_batch_operation($context);
    $context['message'] = t('Converting page @page of @total into a PDF ...', array(
                            '@page' => $context['sandbox']['progress'],
                            '@total' => $context['sandbox']['total']));
  }
}

/**
 * Helper function to get the identifier from MODS data.
 */
function _mods_id_label($object, $dsid) {
  $mods = $object['MODS']->content;
  $xml = simplexml_load_string($mods);
  $id = (string) $xml->identifier;
  return $id . '.' . strtolower($dsid);
}
