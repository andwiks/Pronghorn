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
}
