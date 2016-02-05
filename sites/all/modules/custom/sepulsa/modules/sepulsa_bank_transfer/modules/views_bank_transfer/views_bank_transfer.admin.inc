<?php
/**
 * @file
 * views_bank_transfer.admin.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function views_bank_transfer_admin_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array
 */
function views_bank_transfer_admin_form($form = array(), $form_state = array()) {
  // Get bank transfer payment options.
  $options = variable_get('views_bank_transfer_options', sepulsa_bank_transfer_get_options());
  // Get bank transfer payment info.
  $infos = variable_get('views_bank_transfer_options_info', $options);

  // Looping for each option.
  foreach ($options as $key => $option) {
    drupal_set_message();
    // Define friendly title.
    $title = str_replace('_', ' ', $key);
    // Create fieldset for this option.
    $form[$key] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => $title,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    // Activate payment instance checkbox.
    $form[$key]['active'] = array(
      '#type' => 'checkbox',
      '#title' => $title,
      '#default_value' => boolval($option),
      '#description' => t('Check to enable payment instance for views.'),
    );
    // Custom payment instance name.
    $form[$key]['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom Name'),
      '#default_value' => $infos[$key],
      '#size' => 60,
      '#maxlength' => 128,
      '#attributes' => array(
        'placeholder' => $title,
      ),
      '#description' => t('Custom payment instance name to be shown at views.'),
    );
  }

  // Submit button.
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Function views_bank_transfer_admin_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function views_bank_transfer_admin_form_validate($form, &$form_state) {
  // Get bank transfer payment options.
  $options = variable_get('views_bank_transfer_options', sepulsa_bank_transfer_get_options());
  // Looping for each payment options.
  foreach ($options as $key => $option) {
    if (isset($form_state['values'][$key]['name']) && !empty($form_state['values'][$key]['name'])) {
      // Validate the name: alphanumeric and space only minimum 3 chars.
      if (!preg_match('#^[a-z0-9 ]{3,}$#i', $form_state['values'][$key]['name'])) {
        form_set_error($key . '][name', t('Invalid custom name for @key. Use only alphanumeric and space character with minimum 3 characters.', array(
          '@key' => $key,
        )));
      }
    }
  }
}

/**
 * Function views_bank_transfer_admin_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function views_bank_transfer_admin_form_submit($form, &$form_state) {
  // Get bank transfer payment options.
  $options = variable_get('views_bank_transfer_options', sepulsa_bank_transfer_get_options());
  // Get bank transfer payment info.
  $infos = variable_get('views_bank_transfer_options_info', $options);
  // Looping for each payment options.
  foreach ($options as $key => $option) {
    $options[$key] = boolval($form_state['values'][$key]['active']);
    if (isset($form_state['values'][$key]['name']) && !empty($form_state['values'][$key]['name'])) {
      $infos[$key] = $form_state['values'][$key]['name'];
    }
  }
  // Update variables.
  variable_set('views_bank_transfer_options', $options);
  variable_set('views_bank_transfer_options_info', $infos);
  // Show update message.
  drupal_set_message(t('Configuration has been updated.'));
}
