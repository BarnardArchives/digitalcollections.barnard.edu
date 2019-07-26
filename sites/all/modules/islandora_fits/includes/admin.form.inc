<?php

/**
 * @file
 * The admin form for the islandora fits module.
 */

/**
 * Admin form for Islandora FITS configuration.
 *
 * @see drupal_get_form
 */
function islandora_fits_admin_form($form, $form_state) {
  if (isset($form_state['triggering_element'])) {
    // Textfield AJAX callback.
    if ($form_state['triggering_element']['#name'] == 'islandora_fits_path_textfield') {
      $fits_path = $form_state['input']['islandora_fits_path_textfield'];
    }
  }
  else {
    $fits_path = variable_get('islandora_fits_executable_path', 'fits.sh');
  }
  $conf_message = islandora_fits_get_path_message($fits_path);
  $form['islandora_fits_do_derivatives'] = array(
    '#type' => 'checkbox',
    '#title' => t('Run Islandora FITS derivatives locally'),
    '#name' => 'islandora_fits_do_derivatives',
    '#description' => t('If checked (default), this server will use FITS to create a technical metadata datastream (the datastream id of which is defined below). Requires a local installation of the FITS tool.'),
    '#return_value' => 1,
    '#default_value' => variable_get('islandora_fits_do_derivatives', 1),
  );
  $form['islandora_fits_wrapper'] = array(
    '#prefix' => '<div id="islandora_fits_wrapper">',
    '#suffix' => '</div>',
  );
  $form['islandora_fits_wrapper']['islandora_fits_executable_path'] = array(
    '#type' => 'textfield',
    '#title' => t('System path to FITS processor'),
    '#name' => 'islandora_fits_path_textfield',
    '#description' => t('The complete path and filename of the File Information Tool Set (FITS) binary, or the command to run if in the path.'),
    '#default_value' => $fits_path,
    '#ajax' => array(
      'callback' => 'islandora_fits_path_ajax',
      'wrapper' => 'islandora_fits_wrapper',
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_fits_do_derivatives"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['islandora_fits_wrapper']['islandora_fits_path_check'] = array(
    '#type' => 'item',
    '#markup' => $conf_message,
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_fits_do_derivatives"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Add form options for what the datastream is called.
  $form['islandora_fits_techmd_dsid'] = array(
    '#type' => 'textfield',
    '#title' => t('Technical metadata datastream ID'),
    '#description' => t("The DSID to use when creating or displaying an object's technical metadata."),
    '#default_value' => variable_get('islandora_fits_techmd_dsid', 'TECHMD'),
  );
  // Add options to include/exclude certain mimetypes/extensions/solution packs.
  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save Configuration'));
  $form['#submit'][] = 'islandora_fits_admin_submit_form';

  return $form;
}

/**
 * Validation handler for Fits admin form.
 */
function islandora_fits_admin_form_validate($form, &$form_state) {
  // Only validate the FITS path if doing derivatives.
  if ($form_state['values']['islandora_fits_do_derivatives']) {
    $errors = islandora_fits_path_check($form_state['values']['islandora_fits_executable_path']);
    if (!empty($errors)) {
      form_set_error('islandora_fits_executable_path', "FITS path is not valid. Provide a valid path or disable FITS derivatives.");
    }
  }
}

/**
 * Form submit handler.
 */
function islandora_fits_admin_submit_form($form, &$form_state) {
  $variables = array(
    'islandora_fits_do_derivatives',
    'islandora_fits_techmd_dsid',
  );
  foreach ($variables as $variable) {
    variable_set($variable, $form_state['values'][$variable]);
  }
  // Conditionally set the FITS executable path.
  if ($form_state['values']['islandora_fits_do_derivatives']) {
    variable_set('islandora_fits_executable_path', $form_state['values']['islandora_fits_executable_path']);
  }
  drupal_set_message(t('The settings have been updated!'));
}

/**
 * Ajax handler for admin form.
 */
function islandora_fits_path_ajax($form, $form_state) {
  return $form['islandora_fits_wrapper'];
}

/**
 * Gets a user-friendly message to show whether the path is valid.
 */
function islandora_fits_get_path_message($fits_path) {
  $errors = islandora_fits_path_check($fits_path);
  if (empty($errors)) {
    $confirmation_message = '<img src="' . url('misc/watchdog-ok.png') . '"/> ' . t('Executable found at @fitspath', array('@fitspath' => $fits_path));
  }
  else {
    $confirmation_message = '<img src="' . url('misc/watchdog-error.png') . '"/> ' . implode('<br/>', $errors);
  }
  return $confirmation_message;
}

/**
 * Checks fits path for an executable.
 *
 * @param string $path
 *   Path to a desired executable, or executable string.
 *
 * @return array
 *   Errors encountered executing the file. If it returns an empty array,
 *   then the file was executable.
 */
function islandora_fits_path_check($path) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $allowed_paths = array(
    'fits.sh',
    'fits',
  );
  $errors = array();
  if (!in_array($path, $allowed_paths)) {
    // Check whether the given file exists.
    if (!is_file($path)) {
      $errors[] = t('The specified file path %file does not exist.', array('%file' => $path));
    }
    // If it exists, check if it's executable.
    elseif (!is_executable($path)) {
      $errors[] = t('The specified file path %file is not executable.', array('%file' => $path));
    }
  }
  // Ensure that if this is a Windows server, $file isn't pointing to a
  // shell script (that combo may cause the page to hang).
  if ((islandora_deployed_on_windows()) && (strpos($path, ".sh") == FALSE)) {
    $errors[] = t('Islandora appears to be running on Windows, but the path given ends in .sh.');
  }
  // If we haven't had errors so far, try executing it with the -v option.
  if (empty($errors)) {
    $command = $path . ' -v';
    exec($command, $output, $return_value);
    if ($return_value !== 0) {
      $errors[] = t('The command %file is not a valid FITS executable.', array('%file' => $path));
    }
  }
  return $errors;
}
