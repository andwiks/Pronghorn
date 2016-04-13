<?php
/**
 * @file
 * jwt.pages.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function jwt_tester_verify_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function jwt_tester_verify_form($form = array(), $form_state = array()) {
  $form['result'] = array(
    '#prefix' => '<div id="result">',
    '#suffix' => '</div>',
    '#markup' => '',
  );
  $form['token'] = array(
    '#title' => t('Insert JWT Token'),
    '#type' => 'textarea',
    '#required' => TRUE,
    '#default_value' => isset($form['storage']['token']) ? $form['storage']['token'] : '',
    '#description' => t('You can insert encoded JWT token here.'),
  );
  // Get config options.
  $options = jwt_get_enabled_config();
  $form['config'] = array(
    '#type' => 'select',
    '#title' => t('Select JWT config'),
    '#options' => $options,
    '#required' => TRUE,
    '#default_value' => isset($form['storage']['config']) ? $form['storage']['config'] : key($options),
    '#description' => t('Options based on enabled JWT config. You can check the <a href="@link" target="_blank" title="List of JWT config.">list</a>.', array(
      '@link' => url(JWT_CONFIG_MENU_PATH),
    )),
  );
  $form['verify'] = array(
    '#type' => 'submit',
    '#value' => t('Verify'),
    '#ajax' => array(
      'callback' => 'jwt_tester_verify_submit_driven_callback',
      'wrapper' => 'result',
    ),
  );
  return $form;
}

/**
 * Function jwt_tester_verify_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_tester_verify_form_submit($form, &$form_state) {
  // Verify the token.
  $result = jwt_verify($form_state['values']['token'], $form_state['values']['config']);
  // Show result to user.
  if ($result) {
    drupal_set_message(t('Verified Signature'));
    drupal_set_message(t('Claims: <pre>@claims</pre>', array(
      '@claims' => print_r($result, TRUE),
    )));
  }
  else {
    drupal_set_message(t('Invalid Signature'), 'warning');
  }
  // Store values so next reload still available.
  $form_state['storage']['token'] = $form_state['values']['token'];
  $form_state['storage']['config'] = $form_state['values']['config'];
  // Rebuild the form.
  $form_state['rebuild'] = TRUE;
}

/**
 * Function jwt_tester_verify_submit_driven_callback().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_tester_verify_submit_driven_callback($form, &$form_state) {
  $form['result']['#markup'] = '';
  return $form['result'];
}
