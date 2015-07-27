<?php
/**
 * @file
 * topup.rules.inc
 *
 * @author andre4s_y
 */

/**
 * Hook Implementations
 */

/**
 * Implements hook_rules_event_info().
 */
function topup_rules_event_info() {
  // So that we can use the entity_rules_events_variables() helper function.
  module_load_include('inc', 'entity', 'entity.rules');

  $events = array();

  $events['all_topup_success'] = array(
    'label' => t('When all topup is success'),
    'group' => t('Topup'),
    'variables' => entity_rules_events_variables('commerce_order', t('Order', array(), array('context' => 'a drupal commerce order'))),
  );

  return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function topup_rules_action_info() {
  $actions = array();

  $actions['topup_queue_request'] = array(
    'label' => t('Queue Topup Request'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Pending/Payment Received Order'),
      ),
    ),
    'group' => t('Topup'),
    'callbacks' => array(
      'execute' => 'topup_queue_request',
    ),
  );

  $actions['topup_complete'] = array(
    'label' => t('Topup Complete'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Topup Order Complete'),
      ),
    ),
    'group' => t('Topup'),
    'callbacks' => array(
      'execute' => 'topup_complete',
    ),
  );

  return $actions;
}

/**
 * Callback Functions.
 */

/**
 * Function topup_queue_request().
 *
 * Queue top up data.
 *
 * @param object $order
 *   Order object.
 */
function topup_queue_request($order) {
  // Define default topup container.
  $topup = array();

  // Get order wrapper.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  // Looping for each line item.
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    // Get topup array and merge it with current array.
    $topup_data = topup_get_topup_from_line_item_id($line_item_wrapper->line_item_id->value());
    // Check whether topup data is available.
    if (!empty($topup_data)) {
      // Merge it with current topup data.
      $topup = array_merge($topup, $topup_data);
    }
  }

  // Check whether pulsa container is available.
  if (!empty($topup)) {
    $providers = topup_providers();
    // Get the queue.
    $queue = DrupalQueue::get('topup_process');
    // Looping for each pulsa container.
    foreach ($topup as $item) {
      if (empty($providers[$item['field_topup_provider']]['no_queue'])) {
        // Update topup status: in queue.
        topup_update_status($item['line_item_id'], TOPUP_INQUEUE);
        // Queue it.
        $queue->createItem($item);
      }
      else {
        topup_update_status($item['line_item_id'], TOPUP_PENDING);
        topup_queue_process($item);
      }
    }
  }
}

/**
 * Function topup_complete().
 *
 * Dummy action callback to update order status into complete.
 *
 * @param object $order
 *   Order Object.
 */
function topup_complete($order) {
  // Update order status.
  commerce_order_status_update($order, 'completed', FALSE, NULL, t('Order state updated via Rules (Topup Complete).'));
}
