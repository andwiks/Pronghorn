<?php

/**
 * @file
 * Referral Sepulsa Rules code: conditions.
 */

/**
 * Implements hook_rules_condition_info().
 */
function referral_sepulsa_rules_condition_info() {
  $conditions = array(
    'user_first_paid_in_full_order' => array(
      'label' => t('User First Order'),
      'group' => t('Sepulsa'),
      'parameter' => array(
        'order' => array(
          'type' => 'commerce_order',
          'label' => t('Commerce Order'),
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('User who referred to sepulsa'),
        ),
      ),
    ),
  );
  
  return $conditions;
}

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
function referral_sepulsa_rules_action_info() {
  $actions = array(
    'assign_point_to_referrer_user' => array(
      'label' => t('Assign Point to Referrer User'), 
      'group' => t('Sepulsa'),
      'parameter' => array(
        'order' => array(
          'type' => 'commerce_order',
          'label' => t('Commerce Order'),
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('User who referred to sepulsa'),
        ),
        'point_percentage' => array(
          'type' => 'integer',
          'label' => t('Point Percentage'),
          'description' => t('Percentage of commerce order total convert into points (eg. 10)'),
        ),
        'category' => array(
          'type' => 'text',
          'label' => t('Userpoint Category'),
          'description' => t('Select Userpoint Category'),
          'options list' => 'referral_sepulsa_userpoint_category_options',
          'restriction' => 'input',
        ),
      ),
    ),
    'add_point_line_item' => array(
      'label' => t('Add Point Line Item'), 
      'group' => t('Sepulsa'),
      'parameter' => array(
           'order' => array(
          'type' => 'commerce_order',
          'label' => t('Commerce Order'),
        ),
      ),
    ),
  );
  
  return $actions;
}

/**
 * The condition function for 'user_first_paid_in_full_order'.
 */
function user_first_paid_in_full_order($order, $user) {
    $order_id_prev = db_query_range("SELECT order_id FROM {commerce_order} WHERE (status = 'pending' OR status = 'invoiced' OR status = 'completed') AND uid = :uid AND order_id != :order_id", 0, 1, array('uid' => $user->uid, 'order_id' => $order->order_number))->fetchField();
    
    // if order id prev exists, it means this is not user first order
    if ($order_id_prev) return FALSE;
    return TRUE;
}

/**
 * The action function for 'assign_point_to_referrer_user'.
 */
function assign_point_to_referrer_user($order, $user, $point_percentage, $category) {
    // select table user_referral, get user referrer from current user
    $referrer_uid = db_query_range("SELECT referral_uid FROM {referral} WHERE uid = :uid", 0, 1, array('uid' => $user->uid))->fetchField();
    
    if ($referrer_uid) {
        // assign point to this referrer
        $order_total = $order->commerce_order_total["und"][0]["amount"];
        $points = ceil($order_total/100 * $point_percentage); 
        $params = array (
          'uid' => $referrer_uid,
          'points' => $points,
          'tid' => $category,
          'description' => 'User '.$user->uid.' has made first purchase.'
        );
        userpoints_userpointsapi($params); 
    }
}

function referral_sepulsa_userpoint_category_options() {
    $list = array();
    $vocabulary = taxonomy_vocabulary_machine_name_load('userpoints');
    $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
    foreach ($terms as $category) {
        $list[$category->tid] = $category->name;
    }
    //drupal_set_message("<pre>".print_r($list, true)."</pre>");
 
    return $list;
}

/**
 * The action function for 'assign_point_to_referrer_user'.
 */
function add_point_line_item($order) {
    drupal_set_message("<pre>".print_r($order, true)."</pre>");
    //commerce_userpoints_discount_use_credit_callback($order, 10000, 'IDR');
    /*$line_item_id = commerce_userpoints_discount_order_has_userpoint_line_item($order->order_id);
    
    $amount = 100;
    // If there is no existing userpoints line item, we create a new one.
    if ($line_item_id == NULL) {
        // Create the line item.
        $line_item = commerce_userpoints_discount_line_item_factory_new($order->order_id);
  
        // Create the userpoints transaction.
        $userpoints_params = array(
          'points' => $amount * -1,
          'uid' => $order->uid,
          'operation' => 'commerce_userpoints_discount_credit',
          'tid' => variable_get('commerce_userpoints_discount_default_termid', userpoints_get_default_tid()),
          'display' => FALSE,
          'entity_id' => $order->order_id,
          'entity_type' => 'commerce_order',
        );

        $result = userpoints_userpointsapi($userpoints_params);
        
        drupal_set_message("<pre>".print_r($result, true)."</pre>");
        $label = "User credit";
        commerce_userpoints_discount_line_item_populate(
          $line_item, $order->order_id,
          $result['transaction'], commerce_default_currency(), $label);

        commerce_userpoints_discount_line_item_save($line_item);
    } else {
        // If there is an existing userpoints line item for this order, we load it to get its userpoints transaction id.
        $line_item = commerce_line_item_load($line_item_id);

        // Get the existing transaction.
        $txn_field = field_get_items('commerce_line_item', $line_item, 'commerce_userpoints_discount_transaction');

        // Clear out the old $line_item.
        commerce_userpoints_discount_clear_order($order->order_id);
        unset($line_item);

        // Update the userpoints transaction.
        $userpoints_params = array(
          'points' => intval($amount) * -1,
          'txn_id' => $txn_field[0]['value'],
          'status' => 0,
        );
        $result = userpoints_userpointsapi($userpoints_params);
    
        // Create the new line item.
        $line_item = commerce_userpoints_discount_line_item_factory_new($order->order_id);

        $label = t("User credit");
        commerce_userpoints_discount_line_item_populate(
          $line_item, $order->order_id,
          $result['transaction'], commerce_default_currency(), $label);

        commerce_userpoints_discount_line_item_save($line_item);
    }*/
}