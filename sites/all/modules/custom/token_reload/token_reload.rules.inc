<?php

/**
 * Implements hook_rules_action_info().
 */
function token_reload_rules_action_info() {
  $actions = array();

  $actions['token_reload_queue_request'] = array(
    'label' => t('Queue Token Reload Request'),
     'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Pending/Payment Received Order'),
       ),
     ),
     'group' => t('Token Reload'),
     'callbacks' => array(
       'execute' => 'token_reload_queue_request',
     ),
   );


  return $actions;
}

/**
 * Callback Functions.
 */

function token_reload_queue_request($order) {
  $queue = DrupalQueue::get('token_reload_process');
  foreach (entity_metadata_wrapper('commerce_order', $order)->commerce_line_items as $line_item_wrapper) {
    if ($line_item_wrapper->getBundle() == 'electricity_prepaid') {
      $queue->createItem(array('line_item_id' => $line_item_wrapper->getIdentifier()));
    }
  }
}
