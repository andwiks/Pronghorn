<?php
/**
 * @file
 * zenziva.admin.inc
 */

/**
 * Callback Functions
 */

/**
 * Function zenziva_settings_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   System setting form.
 */
function zenziva_settings_form($form = array(), $form_state = array()) {
  $settings = variable_get('zenziva_settings', array(
    'target' => 'https://reguler.zenziva.net/apps/smsapi.php',
    'userkey' => '',
    'passkey' => '',
    'type' => 'dc',
    'balance' => 'https://reguler.zenziva.net/apps/smsapibalance.php',
    'ssl_verify' => FALSE,
  ));

  $form['zenziva_settings'] = array(
    '#tree' => TRUE,
  );
  $form['zenziva_settings']['target'] = array(
    '#type' => 'textfield',
    '#title' => t('API URL'),
    '#required' => TRUE,
    '#default_value' => $settings['target'],
    '#element_validate' => array('zenziva_url_validate'),
  );
  $form['zenziva_settings']['ssl_verify'] = array(
    '#type' => 'checkbox',
    '#title' => t('SSL Verify OFF'),
    '#default_value' => $settings['ssl_verify'],
    '#disabled' => (strpos($settings['target'], 'https') === FALSE),
  );
  $form['zenziva_settings']['userkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Userkey'),
    '#required' => TRUE,
    '#size' => 20,
    '#default_value' => $settings['userkey'],
  );
  $form['zenziva_settings']['passkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Passkey'),
    '#size' => 20,
    '#default_value' => $settings['passkey'],
  );
  // Tipe: regular | masking.
  $form['zenziva_settings']['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array(
      'dc' => t("Don't Care"),
      'reguler' => t('Reguler'),
      'masking' => t('Masking'),
    ),
    '#required' => TRUE,
    '#default_value' => $settings['type'],
  );
  $form['zenziva_settings']['balance'] = array(
    '#type' => 'textfield',
    '#title' => t('Balance URL'),
    '#default_value' => $settings['balance'],
    '#description' => t('Set this url to allow balance check.'),
    '#element_validate' => array('zenziva_url_validate'),
  );
  // Show balance if there is balance url.
  if (!empty($settings['balance'])) {
    // Get current balance.
    $balance = zenziva_balance();
    if (isset($balance['value']) && isset($balance['text'])) {
      $balance_info = array(
        '@balance' => $balance['value'],
        '@date' => $balance['text'],
      );
    }
    else {
      $balance_info = array(
        '@balance' => t('Unknown'),
        '@date' => t('Unable get balance. Check log.'),
      );
    }
    // Show current balance to user.
    $form['zenziva_settings']['current'] = array(
      '#type' => 'item',
      '#title' => t('Current Balance'),
      '#markup' => t('@balance (@date)', $balance_info),
    );
  }
  return system_settings_form($form);
}

/**
 * Function zenziva_url_validate().
 *
 * @param array $element
 *   Form element array.
 * @param array $form_state
 *   Form state array.
 * @param array $form
 *   Form array.
 */
function zenziva_url_validate($element, &$form_state, $form) {
  if (!empty($element['#value']) && !valid_url($element['#value'], TRUE)) {
    form_error($element, t('You must input valid full URL to @target.', array(
      '@target' => $element['#title'],
    )));
  }
}

/**
 * Function zenziva_balance().
 *
 * Get current balance using API.
 *
 * @return array
 *   Zenziva balance message array (value and text).
 */
function zenziva_balance() {
  // Get zenziva settings.
  $settings = variable_get('zenziva_settings', array(
    'target' => 'https://reguler.zenziva.net/apps/smsapi.php',
    'userkey' => '',
    'passkey' => '',
    'type' => 'dc',
    'balance' => 'https://reguler.zenziva.net/apps/smsapibalance.php',
    'ssl_verify' => FALSE,
  ));
  // Define query url.
  $query_url = array(
    'userkey' => $settings['userkey'],
    'passkey' => $settings['passkey'],
  );
  // Define url to called: automatically encode the url.
  $url = url($settings['balance'], array(
    'query' => $query_url,
    'external' => TRUE,
  ));
  $context = array();
  if ($settings['ssl_verify'] === TRUE) {
    $context = array(
      'context' => stream_context_create(array(
        'ssl' => array(
          'SNI_enabled' => FALSE,
          'verify_peer' => FALSE,
          'verify_peer_name' => FALSE,
        ),
      )),
    );
  }
  // Request it.
  $result = drupal_http_request($url, $context);
  // Check the result.
  if (isset($result->data)) {
    // Return data is XML: load it using simplexml.
    $data = simplexml_load_string($result->data);
    // Valid xml must have message.
    if (isset($data->message)) {
      return (array) $data->message;
    }
  }
  // Log the result.
  watchdog('zenziva', 'Balance API Request Failed Result: <pre>!result</pre>', array(
    '!result' => print_r($result, TRUE),
  ), WATCHDOG_ERROR);
  // Return FALSE.
  return FALSE;
}
