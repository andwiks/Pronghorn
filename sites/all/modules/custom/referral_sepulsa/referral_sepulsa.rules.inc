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
  );
  
  return $actions;
}

/**
 * The condition function for 'user_first_paid_in_full_order'.
 */
function user_first_paid_in_full_order($order, $user) {
    $order_id_prev = db_query_range("SELECT order_id FROM {commerce_order} WHERE (status = 'invoiced' OR status = 'completed') AND uid = :uid AND order_id != :order_id", 0, 1, array('uid' => $user->uid, 'order_id' => $order->order_number))->fetchField();
    
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
