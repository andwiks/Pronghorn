<?php
/**
 * @file
 * zenziva.rules.inc
 */

/**
 * Implements hook_rules_action_info().
 */
function zenziva_rules_action_info() {
  return array(
    'zenziva_api' => array(
      'label' => t('Send sms via Zenziva API.'),
      'group' => t('SMS'),
      'base' => 'zenziva_api',
      'parameter' => array(
        'phone' => array(
          'label' => t('Phone number'),
          'type' => 'text',
        ),
        'message' => array(
          'label' => t('Message'),
          'type' => 'text',
        ),
      ),
    ),
  );
}
