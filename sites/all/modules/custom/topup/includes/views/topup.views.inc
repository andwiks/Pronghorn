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
  // Add topup requeue button.
  $data['commerce_line_item']['topup_requeue'] = array(
    'title' => t('Topup Requeue'),
    'group' => t('Topup'),
    'help' => t('Provide a button to requeue topup line item.'),
    'real field' => 'line_item_id',
    'field' => array(
      'title' => t('Topup Requeue'),
      'help' => t('Topup Requeue'),
      'handler' => 'topup_handler_field_line_item_requeue',
    ),
  );
}
