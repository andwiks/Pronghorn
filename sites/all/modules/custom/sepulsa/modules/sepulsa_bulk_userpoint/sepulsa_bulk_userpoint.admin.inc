<?php
/**
 * @file
 * sepulsa_bulk_userpoint.admin.inc
 *
 * @desc
 *   File sepulsa_bulk_userpoint.admin.inc
 *   Created By : Antonius Ananto
 *   File Usage : Form Init
 */

/**
 * Callback Functions.
 */

/**
 * Function sepulsa_bulk_userpoint_bulk_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function sepulsa_bulk_userpoint_bulk_form($form = array(), &$form_state = array()) {
  $form['fid'] = array(
    '#type' => 'managed_file',
    '#title' => t('File to Bulk Transaction'),
    '#description' => '"username" , "points(+/- without . or ,)", "[optional:reason]", "[optional:reference order_id]"',
    '#upload_location' => 'private://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('csv'),
    ),
    '#required' => TRUE,
  );

  if (module_exists('taxonomy')) {
    $options = userpoints_get_categories();
    $form['tid'] = array(
      '#type' => 'select',
      '#title' => t('Category'),
      '#default_value' => isset($txn->tid) ? $txn->tid : userpoints_get_default_tid(),
      '#options' => $options,
      '#description' => t('The !points category that should apply to this bulk transaction.', userpoints_translation()),
      '#weight' => 0,
      '#access' => count($options) > 1,
      '#required' => TRUE,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Function sepulsa_bulk_userpoint_bulk_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_bulk_userpoint_bulk_form_submit($form, &$form_state) {
  global $user;
  // Check if there is file id.
  if (!empty($form_state['values']['fid'])) {
    // Load file object.
    $file = file_load($form_state['values']['fid']);
    $file_path = drupal_realpath($file->uri);
    if (file_exists($file_path)) {
      $operations = $invalid = array();
      $handle = fopen($file_path, "r");
      if ($handle !== FALSE) {
        while (($data = fgetcsv($handle, 0, ',', '"')) !== FALSE) {
          $uid = get_uid_from_username($data[0]);
          // Check if can get valid uid.
          if (!empty($uid)) {
            $item = array(
              'uname' => (isset($data[0]) && !empty($data[0])) ? $data[0] : 'unknown',
              'uid' => $uid,
              'approver' => $user->uid,
              'points' => $data[1],
              'tid' => $form_state['values']['tid'],
              'description' => t('[BULK] @reason', array(
                '@reason' => (isset($data[2]) && !empty($data[2]) ? $data[2] : 'No Reason'),
              )),
            );
            // Check for entity id.
            if (isset($data[3]) && !empty($data[3])) {
              $item['entity_type'] = 'commerce_order';
              $item['entity_id'] = intval($data[3]);
            }
            $operations[] = array(
              '_sepulsa_bulk_userpoint_process',
              array($item),
            );
          }
          else {
            $invalid[] = t('Invalid username: @user - @point (@desc).', array(
              '@user' => $data[0],
              '@point' => $data[1],
              '@desc' => (isset($data[2]) && !empty($data[2]) ? $data[2] : '-'),
            ));
          }
        }
        $batch = array(
          'title' => t('Bulk UserPoint'),
          'operations' => $operations,
          'finished' => '_sepulsa_bulk_userpoint_finish',
          'error_message' => t('Something Went Wrong When Appending User Point Data.'),
          'progress_message' => t('Adding @current of @total User Point Data.'),
        );
        batch_set($batch);
        fclose($handle);
      }
    }
    else {
      drupal_set_message(t('File Not Exists'), 'error');
    }
    // Delete uploaded file.
    $result = file_delete($file);
    if ($result !== TRUE) {
      drupal_set_message(t('Failed deleting managed file %uri. Result was %result', array(
        '%uri' => $uri,
        '%result' => print_r($result, TRUE),
      )), 'error');
    }
  }
  else {
    drupal_set_message(t('File Not Exists'), 'error');
  }
  // Check invalid process.
  if (isset($invalid) && !empty($invalid)) {
    drupal_set_message(t('Unable to process: <pre>@invalid</pre>', array(
      '@invalid' => print_r($invalid, TRUE),
    )), 'error');
  }
}

/**
 * Private Functions.
 */

/**
 * Function get_uid_from_username().
 *
 * @param string $username
 *   Drupal username.
 */
function get_uid_from_username($username) {
  $query = new EntityFieldQuery();
  $result = $query
    ->entityCondition('entity_type', 'user')
    ->propertyCondition('name', trim($username))
    ->execute();
  if (!empty($result)) {
    $ufindser = array_keys($result['user']);
    return $ufindser[0];
  }
  return FALSE;
}
