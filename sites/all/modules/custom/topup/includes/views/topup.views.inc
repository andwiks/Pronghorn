<?php
/**
 * @file
 * topup.views.inc
 *
 * @author andre4s_y
 */

/**
 * Implements hook_views_data_alter().
 */
function topup_views_data_alter(&$data) {
  // Add topup operation button.
  $data['commerce_line_item']['topup_operation'] = array(
    'title' => t('Topup Operation'),
    'group' => t('Topup'),
    'help' => t('Provide a button todo topup line item operation.'),
    'real field' => 'line_item_id',
    'field' => array(
      'title' => t('Topup Operation'),
      'help' => t('Topup Operation'),
      'handler' => 'topup_handler_field_line_item_operation',
    ),
  );
}
