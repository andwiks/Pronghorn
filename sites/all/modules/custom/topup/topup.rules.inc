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
    // Get line item object.
    $line_item = $line_item_wrapper->value();
    // Only for line item: topup_lineitem_type.
    if ($line_item->type == variable_get('topup_lineitem_type', 'mobile_prepaid')) {
      // Get phone number: only one number.
      $field_topup_phone_number = field_get_items('commerce_line_item', $line_item, variable_get('topup_field_phone_number', 'field_phone_number'));

      // Get product object.
      $product = $line_item_wrapper->commerce_product->value();
      // Get Topup provider.
      $field_topup_provider = field_get_items('commerce_product', $product, variable_get('topup_field_provider', 'field_api_provider'));
      // Get topup code.
      $field_topup_code = field_get_items('commerce_product', $product, variable_get('topup_field_code', 'field_package_code'));

      // Looping for each quantity.
      for ($i = 0; $i < $line_item->quantity; $i++) {
        // Store it in the container: for each quantity.
        $topup[] = array(
          'topup_id' => $i,
          'topup_total' => $line_item->quantity,
          'try' => 1,
          'line_item_id' => $line_item->line_item_id,
          'field_topup_provider' => $field_topup_provider[0]['value'],
          'field_topup_phone_number' => $field_topup_phone_number[0]['value'],
          'field_topup_code' => $field_topup_code[0]['value'],
        );
      }
    }
  }

  // Check whether pulsa container is available.
  if (!empty($topup)) {
    // Get the queue.
    $queue = DrupalQueue::get('topup_process');
    // Looping for each pulsa container.
    foreach ($topup as $item) {
      $queue->createItem($item);
    }
  }
}
