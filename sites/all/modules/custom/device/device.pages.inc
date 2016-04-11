<?php
/**
 * @file
 * device.pages.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function device_user_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $user
 *   User Object.
 */
function device_user_form($form = array(), $form_state = array(), $user = NULL) {
  // Define device table header.
  $header = array(
    'label' => array('data' => t('Label'), 'field' => 'dev.label'),
    'imei' => array('data' => t('IMEI'), 'field' => 'dev.imei'),
    'phone' => array('data' => t('Phone'), 'field' => 'dev.phone'),
    'created' => array('data' => t('Created'), 'field' => 'dev.created'),
    'access' => array('data' => t('Access'), 'field' => 'dev.access'),
  );
  // Define device table rows.
  $rows = array();

  // Query the device for this user: only active device.
  $result = array();
  if (isset($user->uid) && !empty($user->uid)) {
    $query = db_select('device', 'dev')->extend('TableSort');
    $query->fields('dev');
    $query->condition('uid', $user->uid);
    $query->condition('status', 0, '>');
    $result = $query->orderByHeader($header)->execute()->fetchAll();
  }

  // Looping for each result.
  if (!empty($result)) {
    foreach ($result as $row) {
      $rows[$row->did] = array(
        'label' => $row->label,
        'imei' => $row->imei,
        'phone' => $row->phone,
        'created' => format_date($row->created, 'short'),
        'access' => (!empty($row->access)) ? format_date($row->access, 'short') : '-',
      );
    }
  }

  // Define form table.
  $form['device'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#js_select' => FALSE,
    '#multiple' => FALSE,
    '#default_value' => key($rows),
    '#empty' => t('There are no device registered.'),
  );

  // Check if there is result.
  if (!empty($result)) {
    $form['user'] = array(
      '#type' => 'value',
      '#value' => $user->uid,
    );
    // Edit label button.
    $form['edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit'),
    );
    // Delete button.
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }

  return $form;
}

/**
 * Function device_user_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function device_user_form_validate($form, &$form_state) {
  // Validate device ID and user ID.
  if (isset($form_state['values']['device']) && !empty($form_state['values']['device'])
    && isset($form_state['values']['user']) && !empty($form_state['values']['user'])
  ) {
    if (!device_user_validate($form_state['values']['user'], $form_state['values']['device'])) {
      form_set_error($form_state['values']['device'], t('Access Denied.'));
    }
  }
}

/**
 * Function device_user_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function device_user_form_submit($form, &$form_state) {
  // Get form operation.
  $op = array_search($form_state['values']['op'], $form_state['values']);
  if (!empty($op) && $op != 'op') {
    $form_state['redirect'] = 'user/' . $form_state['values']['user'] . '/device/' . $form_state['values']['device'] . '/' . $op;
  }
  else {
    drupal_set_message(t('Unknown operation.'), 'error');
  }
}

/**
 * Function device_user_edit_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $user
 *   User object.
 * @param object $device
 *   Device object.
 *
 * @return array
 *   Form array.
 */
function device_user_edit_form($form = array(), $form_state = array(), $user = NULL, $device = NULL) {
  if (empty($user) || empty($device)) {
    drupal_access_denied();
  }
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );
  $form['did'] = array(
    '#type' => 'value',
    '#value' => $device->did,
  );
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $device->label,
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );
  return confirm_form($form, filter_xss_admin(t('Edit device label')), 'user/' . $user->uid . '/device', NULL, t('Edit'));
}

/**
 * Function device_user_edit_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function device_user_edit_form_validate($form, &$form_state) {
  // Validate device ID and user ID.
  if (isset($form_state['values']['uid']) && !empty($form_state['values']['uid'])
    && isset($form_state['values']['did']) && !empty($form_state['values']['did'])
  ) {
    if (!device_user_validate($form_state['values']['uid'], $form_state['values']['did'])) {
      drupal_access_denied();
      exit;
    }
  }
  // Check for invalid character.
  if (isset($form_state['values']['label']) && !preg_match('#^[a-z0-9\.\-]+$#i', $form_state['values']['label'])) {
    form_set_error('label', t('No special character allowed for device label, use only alphanumeric, dot (.) and dash(-).'));
  }
}

/**
 * Function device_user_edit_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function device_user_edit_form_submit($form, &$form_state) {
  // Delete device entity.
  $device_wrapper = entity_metadata_wrapper('device', $form_state['values']['did']);
  $device_wrapper->label->set($form_state['values']['label']);
  $device_wrapper->save();
  // Notify the user.
  $t_args = array('%label' => $form_state['values']['label']);
  drupal_set_message(t('The device %label has been updated.', $t_args));
  watchdog('device', 'Update device label %label by user.', $t_args, WATCHDOG_NOTICE);
  // Redirect to list page.
  $form_state['redirect'] = 'user/' . $form_state['values']['uid'] . '/device';
}

/**
 * Function device_user_delete_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $user
 *   User object.
 * @param object $device
 *   Device object.
 *
 * @return array
 *   Form array.
 */
function device_user_delete_form($form = array(), $form_state = array(), $user = NULL, $device = NULL) {
  if (empty($user) || empty($device)) {
    drupal_access_denied();
  }
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );
  $form['did'] = array(
    '#type' => 'value',
    '#value' => $device->did,
  );
  $form['label'] = array(
    '#type' => 'value',
    '#value' => $device->label,
  );

  $message = t('Are you sure you want to delete device %label?', array('%label' => $device->label));
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, filter_xss_admin($message), 'user/' . $user->uid . '/device', filter_xss_admin($caption), t('Delete'));
}

/**
 * Function device_user_delete_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function device_user_delete_form_validate($form, &$form_state) {
  // Validate device ID and user ID.
  if (isset($form_state['values']['uid']) && !empty($form_state['values']['uid'])
    && isset($form_state['values']['did']) && !empty($form_state['values']['did'])
  ) {
    if (!device_user_validate($form_state['values']['uid'], $form_state['values']['did'])) {
      drupal_access_denied();
      exit;
    }
  }
}

/**
 * Function device_user_delete_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function device_user_delete_form_submit($form, &$form_state) {
  // Delete device entity.
  entity_delete('device', $form_state['values']['did']);
  // Notify the user.
  $t_args = array('%label' => $form_state['values']['label']);
  drupal_set_message(t('The device %label has been deleted.', $t_args));
  watchdog('device', 'Deleted device %label by user.', $t_args, WATCHDOG_NOTICE);
  // Redirect to list page.
  $form_state['redirect'] = 'user/' . $form_state['values']['uid'] . '/device';
}

/**
 * Private Functions.
 */

/**
 * Function device_user_validate().
 *
 * @param int $uid
 *   User ID.
 * @param int $did
 *   Device ID.
 *
 * @return bool
 *   True if user id has valid device ID. False otherwise.
 */
function device_user_validate($uid, $did) {
  $query = db_select('device', 'dev')
    ->fields('dev')
    ->condition('dev.uid', $uid)
    ->condition('dev.did', $did)
    ->condition('dev.status', 0, '>')
    ->countQuery()
    ->execute()
    ->fetchField();
  return !empty($query) ? TRUE : FALSE;
}
