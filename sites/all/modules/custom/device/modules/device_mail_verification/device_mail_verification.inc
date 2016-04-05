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
 * Function device_mail_verification_link_confirm()
 *
 * @param $hash.
 * hash from email.
 */
function device_mail_verification_link_confirm($hash = NULL) {
  // Di sini akan ada merge user.
  $confirm_status = FALSE;

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
      if (empty($entities)) {

        $confirm_status = FALSE;

      }
      // Settings loaded.
      else {
        // Either no timeout or timeout has no effect.
        if (variable_get('device_mail_timeout', 0) == 0) {

          db_update('device_mail_verification')
            ->fields(array('status' => 1))
            ->condition('dmv_id', $current_mail['dmv_id'], '=')
            ->execute();

          $confirm_status = 'confirmed';

          // Merge User.
          device_mail_verification_merge_user($current_mail);

        }
        // A timeout might have occurred.
        else {

          // Success! Verification request for existing mail within timeout.
          if ($current_mail['timeout'] > time()) {

            db_update('device_mail_verification')
            ->fields(array('status' => 1))
            ->condition('dmv_id', $current_mail['dmv_id'], '=')
            ->execute();

            $confirm_status = 'confirmed';
            
            // Merge User.
            device_mail_verification_merge_user($current_mail);

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
    case FALSE:
      drupal_set_message(t("The mail address could not be confirmed."), 'error');
      break;

    case 'timeout':
      drupal_set_message(t("It took you too long to confirm this mail address."), 'warning');
      break;

    case 'confirmed':
      drupal_set_message(t("Your mail address has been confirmed."), 'status');
      break;

    case 'already_confirmed':
      drupal_set_message(t("Your mail address has already been confirmed."), 'status');
      break;

    default:
      drupal_set_message(t("The mail address could not be confirmed."), 'error');
      break;
  }

  return array();
}

/**
 * Function to Merge User.
 */
function device_mail_verification_merge_user($current_mail) {

  $device = device_load($current_mail['did']);

  $device_account = db_select('commerce_order', 'co')
    ->fields('co')
    ->condition('uid', $device->uid, '=')
    ->execute();

  $web_user = user_load_by_mail($current_mail['mail']);

  $web_account = db_select('commerce_order', 'co')
    ->fields('co')
    ->condition('uid', $web_user->uid, '=')
    ->execute();

  $device_uid = $device->uid;
  $web_uid = $web_user->uid;

  $strategi = 0;
  $merge_to = '';

  // New User
  if ($current_mail['new_flag']) {
    // Update Email User Device. 
    $device_user_wrapper = entity_metadata_wrapper('user', $device_uid);
    $device_user_wrapper->mail->set($current_mail['mail']);
    $device_user_wrapper->save();

    // Send Reset Password
    watchdog('Device Mail Varification', 
          'Merge User New User-> device_account : !device_account_id', 
          array('!device_account_id' => $device_uid,));
  }
  else {
    // Strategi 1. jika hanya salah satu user yang memiliki order, merge ke user yang memiliki order.
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
      else
      {
        watchdog('Device Mail Varification', 
          'Merge User Strategi 1 -> Something Wrong web_account : !web_account_id, device_account : !device_account_id', 
          array('!device_account_id' => $device_uid, '!web_account_id' => $web_uid));
      }
    }
    // Strategi 3. Jika kedua user tidak memiliki order, merge ke user yang memiliki device.
    elseif (!$device_account->rowCount() && !$web_account->rowCount()) {
      // Merge to device account and send email reset password.
      $strategi = 3;
      $merge_to = 'device';
    }
    // Strategi 2. & 4.
    else {
      // Check wheither web account has device already.
      $web_user_has_device = db_select('device', 'd')
        ->fields('d')
        ->condition('uid', $web_uid, '=')
        ->execute();
      // Strategi 4. Jika kedua user punya order, dan jika kedua user punya device.
      if ( isset($web_user_has_device) || $web_user_has_device->rowCount() > 0 ) {
        // Merge to oldest uid and send email reset password.
        $strategi = 4;
        $merge_to = 'web';
      }
      // Strategi 2. jika kedua user memiliki order, merge ke user yang memiliki device.
      else {
        // Merge to user who has device.
        $strategi = 2;
        $merge_to = 'device';
      }
    }
  }

  device_mail_verication_merge($strategi, $current_mail['mail'], $current_mail['did'], $device_uid, $web_uid, $merge_to);

  // add email to device_mail.
  $device_wrapper = entity_metadata_wrapper('device', $current_mail['did']);
  $device_wrapper->device_mail->set($current_mail['mail']);
  $device_wrapper->save();
}

/**
 * Function device_mail_verication_merge().
 *
 * @param string $strategi
 *   Strategi Merge.
 * @param object $mail
 *   Email Input From Device.
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
  watchdog('Device Mail Varification', 
          'Merge User Strategi !strategi ->  UID: !web_account_id merge to !merge_to UID: !device_account_id , changed UID commerce_order : !changed_uid_order', 
          array('!merge_to' => $merge_to,
            '!strategi' => $strategi,
            '!device_account_id' => $device_uid,
            '!web_account_id' => $web_uid,
            '!changed_uid_order' => print_r($changed_uid_order, TRUE),));
}