<?php
/**
 * @file
 * jwt_resources.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function jwt_resources_retrieve().
 *
 * @param string $type
 *   Type of jwt config.
 *
 * @return string
 *   JWT secret / public key as string or error message.
 */
function jwt_resources_retrieve($type = 'key') {
  // Get current resource jwt config.
  $config = jwt_resources_get_config();

  // Check whether config secret / public key is available.
  if (isset($config->public_key)) {
    // Add header for binary transfer.
    header('Content-Transfer-Encoding: binary');
    header('Content-Type: application/x-pem-file;');
    header('Content-Disposition: attachment; filename="public.pem"');
    exit($config->public_key);
  }
  // Other condition: not found.
  return services_error('Key Not Found', 404);
}

/**
 * Function jwt_resources_verify().
 *
 * @param string $token
 *   JWT string.
 *
 * @return bool
 *   TRUE if verified token, FALSE otherwise.
 */
function jwt_resources_verify($token = '') {
  // Get current resource jwt config.
  $config = jwt_resources_get_config();
  // Bundle this with try and catch for future unhandled throw exception.
  try {
    // Verify the token and return boolean value.
    return (jwt_verify($token, $config) !== FALSE) ? TRUE : FALSE;
  }
  catch (Exception $e) {
    // Log this condition.
    watchdog('jwt_resources', 'Unable to verify JWT string from service with message: @message. <pre>Trace: @trace</pre>', array(
      '@message' => $e->getMessage(),
      '@trace' => $e->getTraceAsString(),
    ), WATCHDOG_WARNING);
  }
  // Other condition return false.
  return FALSE;
}
