<?php

/**
 * @file
 * token_reload.views.inc
 */

/**
 * Implements hook_views_data_alter().
 */
function token_reload_views_data_alter(&$data) {
  $data['commerce_line_item']['inquiry'] = array(
    'title' => t('Inquiry'),
    'help' => t('Serialized inquiry data.'),
    'real field' => 'data',
    'field' => array(
      'handler' => 'token_reload_views_handler_inquiry',
    ),
  );

  // Add topup operation button.
  $data['token_reload']['token_reload_operation'] = array(
    'title' => t('Token Reload Operation'),
    'group' => t('Token Reload'),
    'help' => t('Provide a button todo token reload operation.'),
    'real field' => 'trid',
    'field' => array(
      'title' => t('Token Reload Operation'),
      'help' => t('Token Reload Operation'),
      'handler' => 'token_reload_views_handler_token_reload_operation',
    ),
  );
}
