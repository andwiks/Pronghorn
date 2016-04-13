<?php
/**
 * @file
 * device_services.admin.inc
 *
 * @author andreas@sepulsa.com
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
function device_services_settings($form = array(), $form_state = array()) {
  // Fieldset for JWT.
  $form['jwt'] = array(
    '#type' => 'fieldset',
    '#title' => t('JSON Web Token'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Device services JWT fieldset.
  $form['jwt']['device_services_default_claims'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Claims Value'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  // Load default device services claims variable.
  $default_value = variable_get('device_services_default_claims', array(
    'iss' => gethostname(),
    'sub' => '',
    'aud' => '',
  ));
  // Device services default Issuer (iss).
  $form['jwt']['device_services_default_claims']['iss'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Issuer'),
    '#default_value' => $default_value['iss'],
    '#size' => 20,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#description' => t('The "iss" (issuer) claim identifies the principal that issued the JWT.'),
  );
  // Device services default Subject (sub).
  $form['jwt']['device_services_default_claims']['sub'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Subject'),
    '#default_value' => $default_value['sub'],
    '#size' => 20,
    '#maxlength' => 32,
    '#description' => t('The "sub" (subject) claim identifies the principal that is the subject of the JWT.'),
  );
  // Device services default Audience (aud).
  $form['jwt']['device_services_default_claims']['aud'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Audience'),
    '#default_value' => $default_value['aud'],
    '#size' => 20,
    '#maxlength' => 32,
    '#description' => t('The "aud" (audience) claim identifies the recipients that the JWT is intended for.'),
  );
  // Get configuration for enable / disable claims.
  $claims = variable_get('device_services_enable_claims', array(
    'aud' => 0,
    'exp' => 'exp',
    'iat' => 'iat',
    'iss' => 'iss',
    'nbf' => 'nbf',
    'sub' => 0,
  ));
  // Sort by key.
  ksort($claims);
  // Enable / disable default claims.
  $form['jwt']['device_services_enable_claims'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array_keys($claims)),
    '#default_value' => $claims,
    '#title' => t('Enable / Disable Default Claims'),
  );
  // Return system settings form.
  return system_settings_form($form);
}
