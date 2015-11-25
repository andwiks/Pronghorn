<?php

/**
 * @file
 */

function kraken_api_settings($form, $form_state) {
  $settings = variable_get('kraken_api_settings', array());

  $form['kraken_api_settings'] = array(
    '#tree' => TRUE,
  );

  // API Endpoint.
  $form['kraken_api_settings']['endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint'),
    '#default_value' => isset($settings['endpoint']) ? $settings['endpoint'] : '',
    '#size' => 60,
    '#maxlength' => 60,
    '#description' => t('Kraken endpoint. Please input using full url.'),
    '#required' => TRUE,
  );

  // Partner username.
  $form['kraken_api_settings']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Partner username'),
    '#default_value' => isset($settings['username']) ? $settings['username'] : '',
    '#size' => 20,
    '#maxlength' => 20,
    '#required' => TRUE,
  );

  // Partner secret.
  $form['kraken_api_settings']['secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Partner secret key'),
    '#default_value' => isset($settings['secret']) ? $settings['secret'] : '',
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
