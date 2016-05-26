<?php
/**
 * @file
 * auth_jwt.rules.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Hook Implementations.
 */

/**
 * Implements hook_rules_event_info().
 */
function auth_jwt_rules_event_info() {
  return array(
    'auth_jwt_verified' => array(
      'label' => t('Service Request JWT Verified'),
      'group' => t('JWT'),
      'help' => t('Do something with JWT claims after service request verification success.'),
      'access callback' => 'rules_system_integration_access',
      'variables' => array(
        'jwt_claims' => array(
          'label' => 'Claims',
          'type' => 'jwt_claims',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_data_info().
 */
function auth_jwt_rules_data_info() {
  return array(
    'jwt_claims' => array(
      'label' => t('jwt claims'),
    ),
  );
}
