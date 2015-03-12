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
  );
  
  return $actions;
}


/**
 * The action function for 'deduct_point_line_item_after_order'.
 */
function deduct_point_line_item_after_order($order, $line_item) {
    if ($line_item->type == "userpoints") {
        //drupal_set_message("<pre>".print_r($line_item, true)."</pre>"); 
        $amount = $line_item->commerce_unit_price[LANGUAGE_NONE][0]['amount'];
        // Create the userpoints transaction.
        $userpoints_params = array(
          'points' => $amount,
          'uid' => $order->uid,
          'operation' => 'commerce_userpoints_discount_credit',
          'tid' => variable_get('commerce_userpoints_discount_default_termid', userpoints_get_default_tid()),
          'display' => FALSE,
          'entity_id' => $order->order_id,
          'entity_type' => 'commerce_order',
        );

        $result = userpoints_userpointsapi($userpoints_params);
    }
}