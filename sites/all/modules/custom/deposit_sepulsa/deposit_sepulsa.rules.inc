<?php

/**
 * @file
 * Deposit Sepulsa Rules code: actions.
 */

/**
 * Implements hook_rules_action_info().
 *
 * Declares any meta-data about actions for Rules in a big, associative, nested
 * array. See also hook_rules_action_info in the rules.api.php file, and the
 * online documentation at http://drupal.org/node/878928.
 */
function deposit_sepulsa_rules_action_info() {
  $actions = array(
    'topup_credit_deposit_action' => array(
      'label' => t('Topup Credit Deposit'),
      'group' => t('Sepulsa'),
      'parameter' => array(
        'order' => array(
          'type' => 'commerce_order',
          'label' => t('Commerce Order'),
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('User who made the deposit'),
        ),
        'category' => array(
          'type' => 'text',
          'label' => t('Userpoint Category'),
          'description' => t('Select Userpoint Category'),
          'options list' => 'deposit_sepulsa_userpoint_category_options',
          'restriction' => 'input',
        ),
      ),
    ),
  );

  return $actions;
}

/**
 * The action function for 'topup_credit_deposit'.
 */
function topup_credit_deposit_action($order, $user, $category) {
  // Define default amount.
  $amount = 0;
  // Bundle using try and catch.
  try {
    // Get order wrapper.
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    // Get order line items.
    $line_items = $order_wrapper->commerce_line_items->value();
    // Looping for each line item.
    foreach ($line_items as $line_item) {
      // Only if type: deposit_sepulsa.
      if ($line_item->type == 'deposit_sepulsa') {
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
        $line_item_price = $line_item_wrapper->commerce_unit_price->value();
        if (isset($line_item_price['data']) && !empty($line_item_price['data'])
          && isset($line_item_price['data']['components'][0]['price']['amount'])
          && !empty($line_item_price['data']['components'][0]['price']['amount'])
        ) {
          $amount += intval($line_item_price['data']['components'][0]['price']['amount']);
        }
      }
    }
  }
  catch (Exception $e) {
    // Log it as watchdog error.
    watchdog('deposit_sepulsa', 'Unable to get valid deposit pulsa amount. Debug: <pre>Error: @error</pre><pre>Order: @order</pre><pre>User: @user</pre>', array(
      '@error' => $e->getTraceAsString(),
      '@order' => print_r($order, TRUE),
      '@user' => print_r($user, TRUE),
    ), WATCHDOG_ERROR);
  }

  // Only queue it if amount is not empty.
  if (!empty($amount)) {
    $queue = DrupalQueue::get('topup_credit_deposit');
    $queue_item = array('uid' => $user->uid, 'amount' => $amount, 'tid' => $category, 'order' => $order);
    $queue->createItem($queue_item);
  }
}

function deposit_sepulsa_userpoint_category_options() {
    $list = array();
    $vocabulary = taxonomy_vocabulary_machine_name_load('userpoints');
    $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
    foreach ($terms as $category) {
        $list[$category->tid] = $category->name;
    }

    return $list;
}
