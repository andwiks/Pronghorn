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
  return system_settings_form($form);
}
