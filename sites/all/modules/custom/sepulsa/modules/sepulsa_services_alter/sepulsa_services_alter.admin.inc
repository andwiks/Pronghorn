<?php
/**
 * @file
 * sepulsa_services_alter.admin.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function sepulsa_services_alter_settings_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function sepulsa_services_alter_settings_form($form = array(), $form_state = array()) {
  $options = array();
  // Load all available service endpoints.
  $endpoints = services_endpoint_load_all();
  // Looping foreach endpoint.
  foreach ($endpoints as $key => $data) {
    // Only for endpoint with has rest server setting.
    if ($data->server == 'rest_server') {
      $options[$key] = $data->name;
    }
  }
  // Check options: can not empty.
  if (!empty($options)) {
    // Checkboxes: endpoint.
    $form['sepulsa_services_alter_endpoints'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#title' => t('Choose services endpoint'),
      '#default_value' => variable_get('sepulsa_services_alter_endpoints', array()),
      '#description' => t('Select service endpoint which need output alteration.'),
    );
    // Checkboxes: alter also handle error code.
    $form['sepulsa_services_alter_error'] = array(
      '#type' => 'checkboxes',
      '#options' => array(
        '404' => t('404 Not Found'),
        '406' => t('406 Not Acceptable'),
        '500' => t('500 Internal Server Error'),
      ),
      '#title' => t('Choose error code'),
      '#default_value' => variable_get('sepulsa_services_alter_error', array()),
      '#description' => t('Select error code which need output alteration.'),
    );
  }
  else {
    $form['note'] = array(
      '#markup' => t('Currently there are no endpoints to configure.'),
    );
  }
  // Return: system setting form array.
  return system_settings_form($form);
}
