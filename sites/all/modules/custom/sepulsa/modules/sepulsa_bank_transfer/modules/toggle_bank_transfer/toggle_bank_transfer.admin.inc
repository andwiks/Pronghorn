<?php
/**
 * @file
 * toggle_bank_transfer.admin.inc
 *
 * @author andreas
 * @since January 19, 2016
 */

/**
 * Callbank Functions.
 */

/**
 * Function toggle_bank_transfer_admin_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function toggle_bank_transfer_admin_form($form = array(), $form_state = array()) {
  // Get bank transfer payment options.
  $options = variable_get('toggle_bank_transfer_options', sepulsa_bank_transfer_get_options());
  // Get bank transfer payment info.
  $infos = variable_get('toggle_bank_transfer_options_info', $options);

  // Looping for each option.
  foreach ($options as $key => $option) {
    $title = str_replace('_', ' ', $key);
    // Define update info.
    $info = t('Status: <strong>@status</strong>.<br />', array(
      '@status' => $options[$key] ? t('disable [OFF]') : t('enable [ON]'),
    ));
    if (isset($infos[$key]) && !empty($infos[$key])) {
      foreach ($infos[$key] as $status => $data) {
        $info .= t('Last <strong>@action</strong> update %time by %user.<br />', array(
          '@action' => $status ? t('disable') : t('enable'),
          '%time' => date('r', $infos[$key][$status]['time']),
          '%user' => $infos[$key][$status]['user'],
        ));
      }
    }
    // Check the permission.
    if (user_access('toggle ' . $title)) {
      $form[$key] = array(
        '#type' => 'checkbox',
        '#title' => $title,
        '#default_value' => boolval($option),
        '#suffix' => $info . '<br />',
        '#description' => t('Check to disable and uncheck to enable @title.', array(
          '@title' => $title,
        )),
      );
    }
  }

  // Submit button.
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Function toggle_bank_transfer_admin_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function toggle_bank_transfer_admin_form_submit($form, &$form_state) {
  // Check form operation.
  if (isset($form_state['values']['op']) && $form_state['values']['op'] == t('Save')) {
    // Get user object.
    global $user;
    // Get bank transfer payment options.
    $options = variable_get('toggle_bank_transfer_options', sepulsa_bank_transfer_get_options());
    // Get bank transfer payment info.
    $infos = variable_get('toggle_bank_transfer_options_info', $options);
    // Looping for each option.
    foreach ($options as $key => $option) {
      if (isset($form_state['values'][$key]) && boolval($form_state['values'][$key]) !== $options[$key]) {
        $options[$key] = boolval($form_state['values'][$key]);
        $infos[$key][$options[$key]] = array(
          'time' => time(),
          'user' => isset($user->mail) && !empty($user->mail) ? $user->mail : $user->name,
        );
      }
    }
    // Save the variable.
    variable_set('toggle_bank_transfer_options', $options);
    variable_set('toggle_bank_transfer_options_info', $infos);
    // Show message to admin.
    drupal_set_message('Configuration has been saved.');
  }
}
