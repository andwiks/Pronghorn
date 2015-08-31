<?php

function token_reload_settings_form($form, &$form_state) {
  $settings = variable_get('token_reload_settings', array()) + token_reload_default_settings();

  $form['token_reload_settings'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  // API Endpoint.
  $form['token_reload_settings']['endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint'),
    '#default_value' => $settings['endpoint'],
    // '#description' => t('Bimasakti endpoint. Please input using full url.'),
    '#required' => TRUE,
  );

  $form['token_reload_settings']['timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint Timeout'),
    '#default_value' => $settings['timeout'],
    '#size' => 4,
    '#maxlength' => 4,
    // '#description' => t('Bimasakti server endpoint timeout in seconds.'),
    '#required' => TRUE,
  );

  $form['token_reload_settings']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => $settings['username'],
    '#size' => 20,
    '#maxlength' => 20,
    // '#description' => t('Registered User ID at Bimasakti server system.'),
    '#required' => TRUE,
  );
  $form['token_reload_settings']['secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#default_value' => $settings['secret'],
    // '#size' => 20,
    // '#maxlength' => 20,
    // '#description' => t('PIN for token reload transaction.'),
    '#required' => TRUE,
    // do13sDuR-rr3gie16f5_F_EAM-OOFHam_2i9SRglQSM
  );

  return system_settings_form($form);
}
