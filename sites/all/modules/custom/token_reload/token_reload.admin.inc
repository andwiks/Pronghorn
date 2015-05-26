<?php

function token_reload_settings_form($form, &$form_state) {
  $settings = variable_get('token_reload_settings', array()) + token_reload_default_settings();

  $form['token_reload_settings'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  // Environment: dev or prod.
  $form['token_reload_settings']['environment'] = array(
    '#type' => 'select',
    '#title' => t('Environment'),
    '#options' => array(
      'dev' => t('Development'),
      'prod' => t('Production'),
    ),
    '#default_value' => $settings['environment'],
    '#description' => t('Development environment will not send real data and get response with dummy data.'),
  );

  // API Endpoint.
  $form['token_reload_settings']['endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint'),
    '#default_value' => $settings['endpoint'],
    '#description' => t('Bimasakti endpoint. Please input using full url.'),
    '#required' => TRUE,
  );

  $form['token_reload_settings']['timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint Timeout'),
    '#default_value' => $settings['timeout'],
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('Bimasakti server endpoint timeout in seconds.'),
    '#required' => TRUE,
  );

  // UID number.
  $form['token_reload_settings']['uid'] = array(
    '#type' => 'textfield',
    '#title' => t('UID'),
    '#default_value' => $settings['uid'],
    '#size' => 20,
    '#maxlength' => 20,
    '#description' => t('Registered User ID at Bimasakti server system.'),
    '#required' => TRUE,
  );
  // PIN number.
  $form['token_reload_settings']['pin'] = array(
    '#type' => 'textfield',
    '#title' => t('PIN'),
    '#default_value' => $settings['pin'],
    '#size' => 20,
    '#maxlength' => 20,
    '#description' => t('PIN for token reload transaction.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
