<?php
/**
 * @file
 * device_mail_verification.admin.inc
 *
 * @author aizat@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function device_services_settings().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   System settings form array.
 */
function device_mail_verification_settings($form = array(), $form_state = array()) {
  // Fieldset for JWT.
  $form['dmv'] = array(
    '#type' => 'fieldset',
    '#title' => t('Device Mail Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Enable / disable default claims.
  $form['dmv']['device_mail_timeout'] = array(
    '#type' => 'select',
    '#title' => t('Select timeout device mail verification'),
    '#options' => array(
      0 => t('None'),
      (60 * 5) => t('5 Minutes'),
      (60 * 15) => t('15 Minutes'),
      (60 * 30) => t('30 Minutes'),
      (60 * 60 * 1) => t('1 Hour'),
      (60 * 60 * 2) => t('2 Hours'),
      (60 * 60 * 3) => t('3 Hours'),
      (60 * 60 * 6) => t('6 Hours'),
      (60 * 60 * 12) => t('12 Hours'),
      (60 * 60 * 24) => t('1 Day'),
    ),
    '#default_value' => variable_get('device_mail_timeout', 0),
    '#description' => t('Set timeout verification.'),
  );
  // Setting subject mail.
  $form['dmv']['device_mail_subject'] = array(
    '#type' => 'textarea',
    '#title' => t('Set subject verification mail.'),
    '#default_value' => variable_get('device_mail_subject', 'Verification of your mail at Sepulsa.com'),
    '#description' => t('Set subject verification mail.'),
    '#cols' => 30,
    '#rows' => 5,
  );
  // Setting body mail.
  $form['dmv']['device_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Set subject verification mail.'),
    '#default_value' => variable_get('device_mail_body',
      'Please click on the following link to confirm this email address at Sepulsa.com'),
    '#description' => t('Set subject verification body before link verification.'),
    '#cols' => 30,
    '#rows' => 5,
  );
  $form['dmv']['device_mail_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Set Redirect Page.'),
    '#default_value' => variable_get('device_mail_redirect', ''),
    '#description' => t('Set redirect page after confirm.'),
  );
  return system_settings_form($form);
}

function device_mail_verification_update_manual($form = array(), $form_state = array()) {
  $form['update_manual'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update Manual User Device'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['update_manual']['device_did'] = array(
    '#type' => 'textfield',
    '#title' => t('Device ID : '),
    '#default_value' => '',
    '#description' => t('Device ID will be merged.'),
    '#required' => TRUE,
  );
  $form['update_manual']['mail_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Web Email : '),
    '#default_value' => '',
    '#description' => t('The user will be merged.'),
    '#required' => TRUE,
  );
  $form['update_manual']['merge_to'] = array(
    '#type' => 'radios',
    '#title' => t('Merge To :'),
    '#default_value' => 0,
    '#options' => array(0 => t('Web Account'), 1 => t('Device Account')),
    '#description' => t('Merge user to account Web / Device.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proses'),
    '#submit' => array('device_mail_verification_update_manual_submit'),
  );
  return $form;
}

function device_mail_verification_update_manual_submit($form, &$form_state) {
  $result = array();
  $info = FALSE;
  $merge_to = array('web','device');
  $device = device_load($form_state['values']['device_did']);
  if (isset($device) && !empty($device)) {
    if ($device->uid != 0) {
      if (valid_email_address($form_state['values']['mail_user'])) {
        $web_user = db_select('users', 'u')
                ->fields('u')
                ->condition('mail', $form_state['values']['mail_user'],'=')
                ->condition('status', 1, '=')
                ->execute()
                ->fetchAssoc();
        if (isset($web_user) && !empty($web_user)) {
          if ($web_user['uid'] == $device->uid) {
            // Re-syncronize.
            $user = db_select('commerce_order', 'co')
                ->fields('co')
                ->condition('uid', $web_user['uid'], '=')
                ->execute();
            foreach ($user as $key) {
              db_update('commerce_order_revision')
                    ->fields(array('order_id' => $key->order_id))
                    ->condition('revision_uid', $key->uid, '=')
                    ->execute();
              db_update('commerce_payment_transaction')
                    ->fields(array('order_id' => $key->order_id))
                    ->condition('uid', $key->uid, '=')
                    ->execute();
              $transaction_revision = db_select('commerce_payment_transaction', 't')
                ->fields('t')
                ->condition('order_id', $key->order_id, '=')
                ->execute();
              foreach ($transaction_revision as $trans) {
                db_update('commerce_payment_transaction_revision')
                    ->fields(array('transaction_id' => $trans->transa))
                    ->condition('revision_uid', $key->uid, '=')
                    ->execute();
              }
            }
            $info = 4;
          }
          else {
            $web_device = db_select('device', 'd')
              ->fields('d')
              ->condition('uid', $web_user['uid'], '=')
              ->execute();

            if (isset($web_device) && $web_device->rowCount() > 0) {
              $data = array(
                'strategi' => 5,
                'mail' => $form_state['values']['mail_user'],
                'merge_to' => $merge_to[$form_state['values']['merge_to']],
              );

              $result = drupal_get_form('device_mail_merge_admin_confirm_form',
                          $device,
                          $web_device->fetchAssoc(),
                          $data);
            }
            else {
              device_mail_verification_admin_merge(
                $form_state['values']['mail_user'],
                $form_state['values']['device_did'],
                $device->uid,
                '',
                $web_user['uid'],
                $merge_to[$form_state['values']['merge_to']]);

              $info = 5;
            }
          }
        }
        else {
          $info = 3;
        }
      }
      else {
        $info = 2;
      }
    }
    else {
      $info = 1;
    }
  }

  switch ($info) {
    case 1:
      drupal_set_message(t("Unverified device."), 'error');
      break;

    case 2:
      drupal_set_message(t("Please enter valid Email."), 'error');
      break;

    case 3:
      drupal_set_message(t("Email : !mail , not found .",
        array("!mail" => $form_state['values']['mail_user'])), 'error');
      break;

    case 4:
      drupal_set_message(t("Device with email : !mail has updated (Re-syncronize).",
        array("!mail" => $form_state['values']['mail_user'])), 'status');
      break;

    case 5:
      drupal_set_message(t("Device and email : !mail has Merged. Check log to see details.",
        array("!mail" => $form_state['values']['mail_user'])), 'status');
      break;
    
    default:
      drupal_set_message(t("Device not found."), 'error');
      break;
  }
  
  return (!empty($result)) ? $result : drupal_goto('admin/devices/update_manual');
}

/**
 * kasus 1 : merge to web account. 
 *            user device account di block.
 *            device id di form di arahkan ke web account. (device->uid pake uid email yang tertera)
 *            set email device yang baru
 *            kalau ada history device semua historynya dipindahin ke web.
 * kasus 2 : merge to device account. 
 *            device uid tetap.
 *            user web di block.
 *            change email device and user.
 *            kalau ada history web semua history di pidahin ke device.
 * kasus 3 : merge to web account with device.
 *            form yakinin.
 *            user device account di block.
 *            device id di form di block.
 *            device web biarkan.
 *            kalau ada history device, semua historynya dipindahin ke web.
 * kasus 4 : merge to device account with device.
 *            form yakinin. 
 *            device uid tetap.
 *            user web di block.
 *            block web device.
 *            change email device and user.
 *            kalau ada history web semua history di pidahin ke device.
 */
function device_mail_verification_admin_merge($mail = '', $device_did = '', $device_uid = '', $web_did = '',
  $web_uid ='', $merge_to = '') {
  if ($merge_to == 'web') {
    // Block device Account.
    $web_user_wrapper = entity_metadata_wrapper('user', $device_uid);
    $web_user_wrapper->status->set(0);
    $web_user_wrapper->save();

    if ($web_did == '') {
      // Set uid mail in form to device.
      $web_user_wrapper = entity_metadata_wrapper('device', $device_did);
      $web_user_wrapper->uid->set($web_uid);
      $web_user_wrapper->save();

      // Change email device.
      $device_wrapper = entity_metadata_wrapper('device', $device_did);
      $device_wrapper->device_mail->set($mail);
      $device_wrapper->save();
    }
    else {
      // Block device.
      $web_user_wrapper = entity_metadata_wrapper('device', $device_did);
      $web_user_wrapper->status->set(0);
      $web_user_wrapper->save();
    }

    // Log changed uid in commerce order.
    $order = db_select('commerce_order', 'co')
      ->fields('co', array('order_id'))
      ->condition('uid', $device_uid, '=')
      ->execute();
    foreach ($order as $result) {
      $changed_uid_order[] = $result->order_id;
    }

    if ($order->rowCount() > 0) {
      // Update UID in commerce_order.
      device_mail_verification_transfer_order($device_uid, $web_uid);
    }

    $satu = $device_uid;
    $dua = $web_uid;
  }
  else {
    // Block Web Account.
    $web_user_wrapper = entity_metadata_wrapper('user', $web_uid);
    $web_user_wrapper->status->set(0);
    $web_user_wrapper->save();

    // Change email device.
    $device_wrapper = entity_metadata_wrapper('device', $device_did);
    $device_wrapper->device_mail->set($mail);
    $device_wrapper->save();

    // Change email device user.
    $device_wrapper = entity_metadata_wrapper('user', $device_uid);
    $device_wrapper->mail->set($mail);
    $device_wrapper->save();

    if ($web_did != '') {
      // Block web device.
      $web_user_wrapper = entity_metadata_wrapper('device', $web_did);
      $web_user_wrapper->status->set(0);
      $web_user_wrapper->save();
    }

    // Log changed uid in commerce order.
    $order = db_select('commerce_order', 'co')
      ->fields('co', array('order_id'))
      ->condition('uid', $web_uid, '=')
      ->execute();
    foreach ($order as $result) {
      $changed_uid_order[] = $result->order_id;
    }

    if ($order->rowCount() > 0) {
      // Update UID in commerce_order.
      device_mail_verification_transfer_order($web_uid, $device_uid);
    }

    $satu = $web_uid;
    $dua = $device_uid;
  }

  watchdog('Device Mail Verification',
          'Merge User Strategi !strategi ->  UID: !satu merge to !merge_to UID: !dua , changed UID commerce_order : !changed_uid_order',
          array(
            '!merge_to' => $merge_to,
            '!strategi' => 5,
            '!satu' => $satu,
            '!dua' => $dua,
            '!changed_uid_order' => print_r($changed_uid_order, TRUE),));
}

/**
 * Function device_mail_merge_admin_confirm_form().
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
function device_mail_merge_admin_confirm_form($form = array(), &$form_state = array(), $device_user = NULL, $web_device_user = array(), $data = array()) {
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
  $msg = '<div style = "font-size:20px;"> ';
  if ($data['merge_to'] == 'web') {
    $msg .= '<b>' . t('Selected Device') . '</b> = <br>
              ' . t('IMEI Number') . ' : ' . $device_user->imei . '<br>
              ' . t('Agent') . ' : ' . $device_user->agent . '<br><br>
              ' . t('< Merge To >') . '<br><br>
              <b>' . t('Web User Device') . '</b> = <br>
              ' . t('IMEI Number') . ' : ' . $web_device_user['imei'] . '<br>
              ' . t('Agent') . ' : ' . $web_device_user['agent'] . '<br><br>
            <span style="color:red;">' . t('If you agree to this confirmation, the old device can not be used again.') . '</font>';
  }
  else {
    $msg .= '<b>' . t('Web User Device') . '</b> = <br>
              ' . t('IMEI Number') . ' : ' . $web_device_user['imei'] . '<br>
              ' . t('Agent') . ' : ' . $web_device_user['agent'] . '<br><br>
              ' . t('< Merge To >') . '<br><br>
              <b>' . t('Selected Device') . '</b> = <br>
              ' . t('IMEI Number') . ' : ' . $device_user->imei . '<br>
              ' . t('Agent') . ' : ' . $device_user->agent . '<br><br>
            <span style="color:red;">' . t('If you agree to this confirmation, the old device can not be used again.') . '</font>';
  }
  $msg .= ' </div>';
  $form = confirm_form($form,
                      t('Are you sure want to merge this device ?'),
                      '',
                      $msg);
  $form['actions']['submit']['#submit'] = array('device_mail_merge_admin_confirm_form_submit');
  return $form;
}

/**
 * Function device_mail_merge_admin_confirm_form_submit().
 */
function device_mail_merge_admin_confirm_form_submit($form, &$form_state) {
  device_mail_verification_admin_merge(
                $form_state['values']['mail'],
                $form_state['values']['device_did'],
                $form_state['values']['device_uid'],
                $form_state['values']['web_did'],
                $form_state['values']['web_uid'],
                $form_state['values']['merge_to']);

  drupal_set_message(t("Your device has been merged."), 'status');
  drupal_goto('admin/devices/update_manual');
}
