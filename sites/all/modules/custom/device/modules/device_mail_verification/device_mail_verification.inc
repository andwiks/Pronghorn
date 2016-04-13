<?php
/**
 * @file
 * device_mail_verification.inc
 *
 * @author aizat@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function device_mail_verification_link_confirm().
 *
 * @param string $hash
 *   hash from email.
 */
function device_mail_verification_link_confirm($hash = NULL) {
  // Di sini akan ada merge user.
  $confirm_status = FALSE;
  // Define result variable.
  $result = array();

  $mail_result = db_select('device_mail_verification', 'e')
    ->fields('e')
    ->condition('hash', $hash, '=')
    ->execute();

  // Mail or hash does exist.
  if (isset($mail_result) || $mail_result->rowCount() > 0) {

    $current_mail = $mail_result->fetchAssoc();
    // Hash already processed.
    if ($current_mail['status'] != 0) {

      switch ($current_mail['status']) {
        case 1:
          $confirm_status = 'already_confirmed';
          break;

        case 2:
          $confirm_status = 'already_timeout';
          break;
      }

    }
    // Hash not processed yet.
    else {

      $entities = entity_load('device', array($current_mail['did']));
      // Settings don't exist (this should NOT happen!).
      if (!empty($entities)) {
        // Either no timeout or timeout has no effect.
        if (variable_get('device_mail_timeout', 0) == 0) {

          $confirm_status = 'confirmed';

          // Merge User.
          $result = device_mail_verification_merge_user($current_mail);

        }
        // A timeout might have occurred.
        else {

          // Success! Verification request for existing mail within timeout.
          if ($current_mail['timeout'] > time()) {

            $confirm_status = 'confirmed';
            // Merge User.
            $result = device_mail_verification_merge_user($current_mail);

          }
          // Timeout reached, responding.
          else {
            db_update('device_mail_verification')
            ->fields(array('status' => 2))
            ->condition('dmv_id', $current_mail['dmv_id'], '=')
            ->execute();

            $confirm_status = 'timeout';
          }
        }
      }
    }

  }

  switch ($confirm_status) {
    case 'timeout':
      drupal_set_message(t("It took you too long to confirm this mail address."), 'warning');
      break;

    case 'confirmed':
      if (empty($result)) {
        drupal_set_message(t("Your mail address has been confirmed."), 'status');
      }
      break;

    case 'already_confirmed':
      drupal_set_message(t("Your mail address has already been confirmed."), 'status');
      break;

    default:
      drupal_set_message(t("The mail address could not be confirmed."), 'error');
      break;
  }

  return (!empty($result)) ? $result : drupal_goto(variable_get('device_mail_redirect', ''));
}

/**
 * Function to Merge User.
 */
function device_mail_verification_merge_user($current_mail) {
  // Check device, order, user from device user.
  $device = device_load($current_mail['did']);

  $device_account = db_select('commerce_order', 'co')
    ->fields('co')
    ->condition('uid', $device->uid, '=')
    ->execute();

  // Check device, order, user from web user.
  $web_user = user_load_by_mail($current_mail['mail']);

  $web_account = db_select('commerce_order', 'co')
    ->fields('co')
    ->condition('uid', $web_user->uid, '=')
    ->execute();

  $web_device = db_select('device', 'd')
    ->fields('d')
    ->condition('uid', $web_user->uid, '=')
    ->execute();

  if (isset($web_device) && $web_device->rowCount() > 0) {
    $flag_device = 1;
  }
  else {
    $flag_device = 0;
  }

  $device_uid = $device->uid;
  $web_uid = $web_user->uid;

  $strategi = 0;
  $merge_to = '';

  // Strategi 1. jika hanya salah satu user yang memiliki order,
  // Merge ke user yang memiliki order.
  if (!$device_account->rowCount() || !$web_account->rowCount()) {
    if ($device_account->rowCount() == 0) {
      // Merge to web account and do not send email reset password.
      $strategi = 1;
      $merge_to = 'web';
    }
    elseif ($web_account->rowCount() == 0) {
      // Merge to device account and send email reset password.
      $strategi = 1;
      $merge_to = 'device';
    }
  }
  // Strategi 3. Jika kedua user tidak memiliki order,
  // Merge ke user yang memiliki device.
  elseif (!$device_account->rowCount() && !$web_account->rowCount()) {
    // Merge to device account and send email reset password.
    $strategi = 3;
    $merge_to = 'device';
  }
  // Strategi 2. & 4.
  else {
    // Strategi 4. Jika kedua user punya order,
    // Dan jika kedua user punya device.
    if ($flag_device) {
      // Merge to oldest uid and send email reset password.
      $strategi = 4;
      $merge_to = 'web';
    }
    // Strategi 2. jika kedua user memiliki order,
    // Merge ke user yang memiliki device.
    else {
      // Merge to user who has device.
      $strategi = 2;
      $merge_to = 'device';
    }
  }

  if ($flag_device) {
    $data = array(
      'strategi' => $strategi,
      'mail' => $current_mail['mail'],
      'merge_to' => $merge_to,
    );

    return drupal_get_form('device_mail_merge_confirm_form', $device, $web_device->fetchAssoc(), $data);
  }
  else {
    // New User.
    if ($current_mail['new_flag']) {
      // Update Email User Device.
      $device_user_wrapper = entity_metadata_wrapper('user', $device_uid);
      $device_user_wrapper->mail->set($current_mail['mail']);
      $device_user_wrapper->save();

      watchdog('Device Mail Varification',
            'Merge User New User-> device_account : !device_account_id',
            array('!device_account_id' => $device_uid));
    }
    else {
      device_mail_verication_merge($strategi, $current_mail['mail'], $current_mail['did'], $device_uid, $web_uid, $merge_to);
    }

    // Add email to device_mail.
    $device_wrapper = entity_metadata_wrapper('device', $current_mail['did']);
    $device_wrapper->device_mail->set($current_mail['mail']);
    $device_wrapper->save();

    return array();
  }
}

/**
 * Function device_mail_verication_merge().
 *
 * @param string $strategi
 *   Strategi Merge.
 * @param object $mail
 *   Email Input From Device.
 * @param object $device_did
 *   Device id in device table.
 * @param object $device_uid
 *   Device UID.
 * @param string $web_uid
 *   Web UID.
 * @param string $merge_to
 *   device / web.
 */
function device_mail_verication_merge($strategi = '', $mail = '', $device_did = '', $device_uid = '', $web_uid = '', $merge_to = '') {
  $changed_uid_order = array();
  if ($merge_to == 'device') {
    // Block Web Account.
    $web_user_wrapper = entity_metadata_wrapper('user', $web_uid);
    $web_user_wrapper->status->set(0);
    $web_user_wrapper->save();

    // Change email in user , where uid = $device_uid;
    $device_user_wrapper = entity_metadata_wrapper('user', $device_uid);
    $device_user_wrapper->mail->set($mail);
    $device_user_wrapper->save();

    $account = user_load($device_uid);
    $mail = _user_mail_notify('password_reset', $account);

    if ($strategi = 2) {
      // Log changed uid in commerce order.
      $mail_result = db_select('commerce_order', 'co')
        ->fields('co', array('order_id'))
        ->condition('uid', $web_uid, '=')
        ->execute();
      foreach ($mail_result as $result) {
        $changed_uid_order[] = $result->order_id;
      }

      // Update UID in commerce_order.
      db_update('commerce_order')
            ->fields(array('uid' => $device_uid))
            ->condition('uid', $web_uid, '=')
            ->execute();
    }
  }
  else {
    // Block User Device.
    $device_user_wrapper = entity_metadata_wrapper('user', $device_uid);
    $device_user_wrapper->status->set(0);
    $device_user_wrapper->save();

    // Change UID in device.
    $device_user_wrapper = entity_metadata_wrapper('device', $device_did);
    $device_user_wrapper->uid->set($web_uid);
    $device_user_wrapper->save();

    if ($strategi == 4) {
      // Log changed uid in commerce order.
      $mail_result = db_select('commerce_order', 'co')
        ->fields('co', array('order_id'))
        ->condition('uid', $device_uid, '=')
        ->execute();
      foreach ($mail_result as $result) {
        $changed_uid_order[] = $result->order_id;
      }

      // Update UID in commerce_order.
      db_update('commerce_order')
            ->fields(array('uid' => $web_uid))
            ->condition('uid', $device_uid, '=')
            ->execute();

      // Send Email to reset Password.
      $account = user_load($web_uid);
      $mail = _user_mail_notify('password_reset', $account);
    }
  }

  db_update('device_mail_verification')
    ->fields(array('status' => 1))
    ->condition('mail', $mail, '=')
    ->execute();

  watchdog('Device Mail Varification',
          'Merge User Strategi !strategi ->  UID: !web_account_id merge to !merge_to UID: !device_account_id , changed UID commerce_order : !changed_uid_order',
          array(
            '!merge_to' => $merge_to,
            '!strategi' => $strategi,
            '!device_account_id' => $device_uid,
            '!web_account_id' => $web_uid,
            '!changed_uid_order' => print_r($changed_uid_order, TRUE),));
}

/**
 * Function device_mail_merge_confirm_form().
 *
 * @param array $form
 *   Form array.
 * @param object $device_user
 *   Device object user device.
 * @param array $web_device_user
 *   Device array user web.
 * @param array $data
 *   Index Strategi, merge_to, mail.
 *
 * @return array
 *   Form array.
 */
function device_mail_merge_confirm_form($form = array(), &$form_state = array(), $device_user = NULL, $web_device_user = array(), $data = array()) {
  $form['strategi'] = array(
    '#type' => 'value',
    '#value' => $data['strategi'],
  );
  $form['mail'] = array(
    '#type' => 'value',
    '#value' => $data['mail'],
  );
  $form['device_did'] = array(
    '#type' => 'value',
    '#value' => $device_user->did,
  );
  $form['device_uid'] = array(
    '#type' => 'value',
    '#value' => $device_user->uid,
  );
  $form['web_did'] = array(
    '#type' => 'value',
    '#value' => $web_device_user['did'],
  );
  $form['web_uid'] = array(
    '#type' => 'value',
    '#value' => $web_device_user['uid'],
  );
  $form['merge_to'] = array(
    '#type' => 'value',
    '#value' => $data['merge_to'],
  );
  $form['dmv_id'] = array(
    '#type' => 'value',
    '#value' => $data['dmv_id'],
  );
  $msg = '<div style = "font-size:20px;"> ';
  $msg .= '<b>' . t('New Device') . '</b> = <br>
            ' . t('IMEI Number') . ' : ' . $device_user->imei . '<br>
            ' . t('Agent') . ' : ' . $device_user->agent . '<br><br>
            <b>' . t('Old Device') . '</b> = <br>
            ' . t('IMEI Number') . ' : ' . $web_device_user['imei'] . '<br>
            ' . t('Agent') . ' : ' . $web_device_user['agent'] . '<br><br>
          <span style="color:red;">' . t('If you agree to this confirmation, the old device can not be used again.') . '</font>';
  $msg .= ' </div>';
  $form = confirm_form($form,
                      t('Are you sure want to merge this device ?'),
                      '',
                      $msg);
  $form['actions']['submit']['#submit'] = array('device_mail_merge_confirm_form_submit');
  return $form;
}

/**
 * Function device_mail_merge_confirm_form_submit().
 */
function device_mail_merge_confirm_form_submit($form, &$form_state) {
  device_mail_verication_merge($form_state['values']['strategi'],
    $form_state['values']['mail'],
    $form_state['values']['device_did'],
    $form_state['values']['device_uid'],
    $form_state['values']['web_uid'],
    $form_state['values']['merge_to']);

  if ($form_state['values']['merge_to'] == 'device') {
    // Block Device User.
    $device_user_wrapper = entity_metadata_wrapper('device', $form_state['values']['web_did']);
    $device_user_wrapper->status->set(0);
    $device_user_wrapper->save();

    // Add email to device_mail (Device User).
    $device_wrapper = entity_metadata_wrapper('device', $form_state['values']['device_did']);
    $device_wrapper->device_mail->set($form_state['values']['mail']);
    $device_wrapper->save();
  }
  else {
    // Block Device User.
    $device_user_wrapper = entity_metadata_wrapper('device', $form_state['values']['device_did']);
    $device_user_wrapper->status->set(0);
    $device_user_wrapper->save();

    // Add email to device_mail (Device Web).
    $device_wrapper = entity_metadata_wrapper('device', $form_state['values']['web_did']);
    $device_wrapper->device_mail->set($form_state['values']['mail']);
    $device_wrapper->save();
  }

  db_update('device_mail_verification')
    ->fields(array('status' => 1))
    ->condition('mail', $form_state['values']['mail'], '=')
    ->execute();

  drupal_set_message(t("Your device has been merged."), 'status');
  drupal_goto(variable_get('device_mail_redirect', ''));
}
