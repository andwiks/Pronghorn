<?php

/**
 * @file
 * Referral Sepulsa Rules code: actions.
 */

/**
 * Implements hook_rules_action_info().
 *
 * Declares any meta-data about actions for Rules in a big, associative, nested
 * array. See also hook_rules_action_info in the rules.api.php file, and the
 * online documentation at http://drupal.org/node/878928.
 */
function userpoints_sepulsa_rules_action_info() {
  $actions = array(
    'userpoints_sepulsa_apply' => array(
      'label' => t('Apply userpoints to an order'),
      'group' => t('Sepulsa'),
      'parameter' => array(
        'order' => array(
          'label' => t('Commerce order'),
          'type' => 'commerce_order',
          'wrapped' => TRUE,
        ),
        'user' => array(
          'label' => t('User'),
          'type' => 'user',
        ),
      ),
    ),
    'deduct_point_line_item_after_order' => array(
      'label' => t('Deduct Point Line Item'),
      'group' => t('Sepulsa'),
      'parameter' => array(
        'order' => array(
          'type' => 'commerce_order',
          'label' => t('Commerce Order'),
        ),
        'line_item' => array(
          'type' => 'commerce_line_item',
          'label' => t('Commerce Line Item'),
        ),
      ),
    ),
    'reassign_point_line_item' => array(
      'label' => t('Reassign Point Line Item'),
      'group' => t('Sepulsa'),
      'parameter' => array(
        'order' => array(
          'type' => 'commerce_order',
          'label' => t('Commerce Order'),
        ),
        'line_item' => array(
          'type' => 'commerce_line_item',
          'label' => t('Commerce Line Item'),
        ),
      ),
    ),
  );

  return $actions;
}

/**
 * Implements hook_rules_event_info().
 */
function userpoints_sepulsa_rules_event_info() {
  $items = array();

  $items['userpoints_sepulsa_order'] = array(
    'label' => t('Apply userpoints to a given order'),
    'group' => t('Sepulsa'),
    'variables' => entity_rules_events_variables('commerce_order', t('Order', array(), array('context' => 'a drupal commerce order'))),
    'access callback' => 'commerce_order_rules_access',
  );

  return $items;
}

function userpoints_sepulsa_apply(EntityDrupalWrapper $order_wrapper, $user = NULL) {
  $point_amount = userpoints_get_current_points($user->uid, 'all');
  $order_total = $order_wrapper->commerce_order_total->amount->value();
  $amount = $order_total > $point_amount ? $point_amount : $order_total;

  if ($amount) {
    $line_item = commerce_userpoints_discount_line_item_factory_new($order_wrapper->getIdentifier());

    // Create the userpoints transaction.
    $userpoints_params = array(
      'points' => $amount * -1,
      'uid' => $user->uid,
      'operation' => 'commerce_userpoints_discount_credit',
      'tid' => variable_get('commerce_userpoints_discount_default_termid', userpoints_get_default_tid()),
      'display' => FALSE,
      'entity_id' => $order_wrapper->getIdentifier(),
      'entity_type' => 'commerce_order',
      'txn_id' => NULL,
    );

    $line_item = commerce_userpoints_discount_line_item_populate($line_item, $order_wrapper->getIdentifier(), $userpoints_params, commerce_default_currency(), t('User credit'));
    commerce_line_item_save($line_item);
    $order = $order_wrapper->value();
    $delta = $order_wrapper->commerce_line_items->count();
    $order->commerce_line_items[LANGUAGE_NONE][]['line_item_id'] = $line_item->line_item_id;
  }
}

/**
 * The action function for 'deduct_point_line_item_after_order'.
 */
function deduct_point_line_item_after_order($order, $line_item) {
  $amount = $line_item->commerce_unit_price[LANGUAGE_NONE][0]['amount'];
  userpoints_line_item_transaction($order, $line_item, $amount);
}

/**
 * The action function for 'reassign_point_line_item'.
 * Reassign point to user if the order is being canceled
 */
function reassign_point_line_item($order, $line_item) {
  $amount = $line_item->commerce_unit_price[LANGUAGE_NONE][0]['amount'];
  userpoints_line_item_transaction($order, $line_item, $amount*-1);
}

function userpoints_line_item_transaction($order, $line_item, $amount=0){
  if ($line_item->type == "userpoints") {
	//drupal_set_message("<pre>".print_r($line_item, true)."</pre>");
	// Create the userpoints transaction.
	$userpoints_params = array(
	  'points' => $amount,
	  'uid' => $order->uid,
	  'operation' => 'commerce_userpoints_discount_credit',
	  //'tid' => variable_get('commerce_userpoints_discount_default_termid', userpoints_get_default_tid()),
	  'display' => FALSE,
	  'entity_id' => $order->order_id,
	  'entity_type' => 'commerce_order',
	);

	$result = userpoints_userpointsapi($userpoints_params);
  }
}
