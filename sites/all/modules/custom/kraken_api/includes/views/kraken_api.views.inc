<?php

/**
 * @file
 * kraken_api.views.inc
 */

/**
 * Implements hook_views_data_alter().
 */
function kraken_api_views_data_alter(&$data) {
  $data['commerce_line_item']['kraken_data'] = array(
    'title' => t('Kraken data'),
    'help' => t('Serialized kraken data.'),
    'real field' => 'data',
    'field' => array(
      'handler' => 'kraken_api_views_handler_kraken_data',
    ),
  );
}
