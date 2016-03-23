<?php
/**
 * @file
 * jwt_auth.inc
 */

/**
 * Callback Functions.
 */

/**
 * Function _jwt_auth_authenticate_call().
 *
 * Authenticates a call using JWT to verify the request.
 *
 * @param array $settings
 *   The settings for the authentication set at security settings.
 * @param array $method
 *   The method that's being called
 * @param array $args
 *   The arguments that are being used to call the method.
 *
 * @return void|string
 *   Returns nothing, or a error message if authentication fails
 */
function _jwt_auth_authenticate_call($settings, $method, $args) {
  // Check whether the method need to authenticate: optional or required.
  if (isset($method['endpoint']['jwt_auth']['token'])
    && in_array($method['endpoint']['jwt_auth']['token'], array('optional', 'required'))
  ) {
    // Check Authorization Bearer header.
    if (isset($_SERVER['HTTP_AUTHORIZATION']) && preg_match('#^Bearer\s+(.+)$#i', $_SERVER['HTTP_AUTHORIZATION'], $match)) {
      // Trim it.
      $match[1] = trim($match[1]);
      // Verify JWT using selected config.
      $result = jwt_verify($match[1], $settings['config']);
      // Check the result.
      if ($result !== FALSE) {
        // Define invalid flag.
        $invalid = FALSE;
        // Define DateTime based on Request time.
        $now = new DateTime('@' . REQUEST_TIME);
        // Check settings claim validation: exp.
        if ($settings['validation']['exp'] == 'exp' && isset($result['exp'])) {
          // Get current DateTime object from exp claim.
          $expiration = new DateTime('@' . $result['exp']);
          // Check it: is now newer than expiration time ?
          if ($now > $expiration) {
            // Token already expired.
            $invalid = TRUE;
          }
        }
        // Check settings claim validation: nbf.
        if ($settings['validation']['nbf'] == 'nbf' && isset($result['nbf'])) {
          // Get current DateTime object from nbf claim.
          $before = new DateTime('@' . $result['nbf']);
          // Check it: is now older than not before time ?
          if ($now < $before) {
            // Token will valid later.
            $invalid = TRUE;
          }
        }
        // Check invalid flag.
        if (!$invalid) {
          // Set JWT auth scope: for further validation.
          jwt_auth_scope($method);
          // Set JWT auth claims: for further resource usage.
          jwt_auth_claims($result);
          // Invoke hook: jwt_auth_verified.
          module_invoke_all('jwt_auth_verified', $result);
          // Create Rules Event.
          rules_invoke_event('jwt_auth_verified', $result);
          return;
        }
      }
    }
    // Other condition: if required, return error message.
    if ($method['endpoint']['jwt_auth']['token'] == 'required') {
      return t('Invalid Access Token.');
    }
  }
}

/**
 * Function _jwt_auth_security_settings().
 *
 * @param array $settings
 *   Default settings.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function _jwt_auth_security_settings($settings, &$form_state) {
  $form = array();

  // Get JWT config options.
  $options = jwt_get_enabled_config();
  $form['config'] = array(
    '#type' => 'select',
    '#title' => t('Select JWT config'),
    '#options' => $options,
    '#required' => TRUE,
    '#default_value' => isset($settings['config']) ? $settings['config'] : key($options),
    '#description' => t('Options based on enabled JWT config. You can check the <a href="@link" target="_blank" title="List of JWT config.">list</a>.', array(
      '@link' => url(JWT_CONFIG_MENU_PATH),
    )),
  );

  // Option: Does JWT auth need to validate these claims?
  $form['validation'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Validate claims.'),
    '#options' => array(
      'exp' => t('Expiration time (exp) claim.'),
      'nbf' => t('Not before (nbf) claim.'),
    ),
    '#default_value' => isset($settings['validation']) ? $settings['validation'] : array(),
  );

  return $form;
}

/**
 * Function _jwt_auth_controller_settings().
 *
 * Configuration for each service controller.
 *
 * @param array $settings
 *   Default settings.
 * @param array $controller
 *   Service controller definition.
 * @param string $endpoint
 *   "jwt_auth"
 * @param string $class
 *   Resource type.
 * @param string $name
 *   Resource name.
 *
 * @return array
 *   Form array.
 */
function _jwt_auth_controller_settings($settings, $controller, $endpoint, $class, $name) {
  $form = array();
  // Define default value.
  $default_value = 'none';
  if (isset($controller['jwt']['token'])) {
    $default_value = $controller['jwt']['token'];
  }
  if (isset($settings['token'])) {
    $default_value = $settings['token'];
  }
  // Option: Does this controller need JWT auth?
  $form['token'] = array(
    '#type' => 'select',
    '#options' => array(
      'none' => t('None'),
      'optional' => t('Optional'),
      'required' => t('Required'),
    ),
    '#default_value' => $default_value,
    '#title' => t('Required token'),
    '#description' => t('Authorization levels will <em>not</em> be applied if the consumer is not required to supply a token.'),
  );

  return $form;
}
