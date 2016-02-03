<?php
/**
 * @file
 * newrelic_alerts.admin.inc
 *
 * @author andreas@sepulsa.com
 * @since Februaru 3rd, 2016
 */

/**
 * Callback Functions.
 */

/**
 * Function newrelic_alerts_settings().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   System setting form array.
 */
function newrelic_alerts_settings($form = array(), $form_state = array()) {
  $form = array();

  // Description.
  $form['description'] = array(
    '#markup' => t('Check the New Relic data at !link.', array(
      '!link' => l(t('New Relic API Keys'), 'https://rpm.newrelic.com/accounts/908709/integrations', array(
        'query' => array('page' => 'api_keys'),
        'attributes' => array(
          'target' => '_blank',
        ),
      )),
    )),
  );

  // New Relic account id.
  $form['newrelic_alerts_acc_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Account ID'),
    '#default_value' => variable_get('newrelic_alerts_acc_id', ''),
    '#size' => 12,
    '#maxlength' => 12,
    '#required' => TRUE,
  );

  // New Relic application id.
  $form['newrelic_alerts_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Application ID'),
    '#default_value' => variable_get('newrelic_alerts_app_id', ''),
    '#size' => 12,
    '#maxlength' => 12,
    '#required' => TRUE,
  );

  // New Relic API key.
  $form['newrelic_alerts_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('newrelic_alerts_api_key', ''),
    '#size' => 60,
    '#maxlength' => 60,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
