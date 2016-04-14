<?php
/**
 * @file
 * sepulsa_referral_campaign.admin.inc
 *
 * @author aizat@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function device_settings().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   System settings form.
 */

function sepulsa_referral_campaign_settings($form = array(), $form_state = array()) {
  // Device verification fieldset.
  $form['sepulsa_referral_campaign'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sepulsa Referral Campaign'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Device verification timeout.
  $form['sepulsa_referral_campaign']['test'] = array(
    '#type' => 'textfield',
    '#title' => t('test'),
    '#default_value' => '',
    '#description' => t('test'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}