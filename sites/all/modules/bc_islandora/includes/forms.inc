<?php

/**
 * Uncorrected transcript edit form
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object.
 *   An AbstractObject representing a Fedora object.
 *
 * @return array
 *   The Drupal form definition.
 */
function bc_islandora_oralhistories_uncorrected_transcript_edit_form(array $form, array &$form_state, AbstractObject $object) {
  if (isset($object['UNCORRECTED_TRANSCRIPT'])) {

    $transcript_extensions = array('txt text');
    $transcript = $object['UNCORRECTED_TRANSCRIPT'];

    $form_state['object'] = $object;
    $form_state['uc_transcript'] = $transcript;

    $form = array();
    $form['new_transcript'] = array(
      '#title' => t('Transcript File'),
      '#type' => 'managed_file',
      '#description' => 'A plain text file for the uncorrected transcript. <br/> Valid file types: .text or .txt',
      '#required' => TRUE,
      '#upload_location' => 'temporary://',
      '#upload_validators' => array(
        'file_validate_extensions' => $transcript_extensions,
      ),
    );

//    $current_text = $transcript->content;
//    $form['updatedtranscript'] = array(
//      '#title' => 'Transcript Text',
//      '#description' => 'Plain text value for the UNCORRECTED TRANSCRIPT. <br/> Note line formatting is not kept on submit.',
//      '#type' => 'textarea',
//      '#value' => $current_text,
//      '#rows' => 15,
//    );

    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );

    return $form;
  }
}

/**
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function bc_islandora_oralhistories_uncorrected_transcript_edit_form_submit(array $form, array &$form_state) {
  $object = $form_state['object'];
  $transcript = $form_state['uc_transcript'];
//  $new_tc = $form_state['value']['updatedtranscript'];
//  $new_transcript = check_plain($form_state['input']['updatedtranscript']);

  $new_transcript = file_load($form_state['values']['new_transcript']);
  $new_transcript_path = drupal_realpath($new_transcript->uri);

//  drupal_set_message('<pre>' . print_r($form_state, TRUE) . '</pre>');
//  drupal_set_message(t('Value: @transcript', array('@transcript' => $new_tc)));
//  drupal_set_message(t('Input: @transcript', array('@transcript' => $new_transcript)));

  try {
//    $transcript->setContentFromString($new_transcript);
    $transcript->setContentFromFile($new_transcript_path, FALSE);
    drupal_set_message(t('The @dsid datastream has been updated', array('@dsid' => $transcript->id)), 'status');
    $form_state['redirect'] = "islandora/object/$object->id";
  }
  catch (Exception $e) {
    drupal_set_message(t('Error updating datastream @dsid: @msg', array('@dsid' => $transcript->id, '@msg' => $e->getMessage())), 'error');
  }

}
