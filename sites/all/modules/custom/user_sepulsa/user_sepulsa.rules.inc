<?php

/**
 * @file
 * User Sepulsa Rules code: actions.
 */

/**
 * Implements hook_rules_action_info().
 *
 * Declares any meta-data about actions for Rules in a big, associative, nested
 * array. See also hook_rules_action_info in the rules.api.php file, and the
 * online documentation at http://drupal.org/node/878928.
 */
function user_sepulsa_rules_action_info() {
  $actions = array(
    'add_payment_order_to_user' => array(
      'label' => t('Add Payment Order to User'),
      'group' => t('Sepulsa'),
      'parameter' => array(
        'order_id' => array(
          'type' => 'integer',
          'label' => t('Order ID'),
        ),
        'user_id' => array(
          'type' => 'integer',
          'label' => t('User ID'),
        ),
      ),
    ),
  );

  return $actions;
}

/**
 * The action function for 'add_payment_order_to_user'.
 */
function add_payment_order_to_user($order_id, $user_id) {
	$user = user_load($user_id);
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'commerce_payment_transaction')
		->propertyCondition('order_id', $order_id)
		//->propertyCondition('uid', $user_id)
		->propertyCondition('status', 'success');

	$results = $query->execute();
	if (!empty($results['commerce_payment_transaction'])) {
		$transactions = entity_load('commerce_payment_transaction', array_keys($results['commerce_payment_transaction']));
		foreach ($transactions as $transaction) {
			$insertpayment = true;
			$payment_method = $transaction->payment_method;
			if (!empty($user->field_payment_order)) {
				foreach($user->field_payment_order[LANGUAGE_NONE] as $user_payment) {
					if ($user_payment['value'] == $payment_method) $insertpayment = false;
				}
			}
			if ($insertpayment == true) { // insert payment order into user
				$user->field_payment_order[LANGUAGE_NONE][] = array('value' => $payment_method, 'format' => '', 'safe_value' => $payment_method);
				user_save($user);
			}
			//drupal_set_message("<pre>".print_r($user, true)."</pre>");
		}
	}
}
