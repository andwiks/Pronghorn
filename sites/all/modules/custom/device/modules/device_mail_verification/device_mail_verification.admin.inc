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

function device_mail_update_manual($form = array(), $form_state = array()) {
  $form['update_manual'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update Manual User Device'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['update_manual']['mail_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Email User : '),
    '#default_value' => '',
    '#description' => t('The user will be merged.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proses'),
    '#submit' => array('device_mail_update_manual_submit'),
  );
  return $form;
}

function device_mail_update_manual_submit($form, &$form_state) {
  if (valid_email_address($form_state['values']['mail_user'])) {
    $user = db_select('users', 'u')
            ->fields('u')
            ->condition('mail', $form_state['values']['mail_user'],'=')
            ->condition('status', 1, '=')
            ->execute()
            ->fetchAssoc();

    if (isset($user) && !empty($user)) {
      $device_account = db_select('device', 'd')
            ->fields('d')
            ->condition('uid', $user['uid'], '=')
            ->condition('status', 1, '=')
            ->execute()
            ->fetchAssoc();
      if (isset($device_account) && !empty($device_account)) {
        $user = db_select('commerce_order', 'co')
            ->fields('co')
            ->condition('uid', $user['uid'], '=')
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
        drupal_set_message(t("Device with email : !mail has updated.",
          array("!mail" => $form_state['values']['mail_user'])), 'status');
      }
      else {
        drupal_set_message(t("Device with email : !mail , not found !.",
          array("!mail" => $form_state['values']['mail_user'])), 'error');
      }
    }
    else {
      drupal_set_message(t("Email : !mail , not found !.",
      array("!mail" => $form_state['values']['mail_user'])), 'error');
    }
  }
  else {
    drupal_set_message(t("Please enter valid Email."), 'error');
  }

  drupal_goto('admin/devices/update_manual');
}
