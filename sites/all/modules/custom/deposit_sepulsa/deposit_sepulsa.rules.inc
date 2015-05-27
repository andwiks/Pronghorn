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
  $amount = $order->commerce_order_total[LANGUAGE_NONE][0]['amount'];
  $queue = DrupalQueue::get('topup_credit_deposit');
  $queue_item = array('uid' => $user->uid, 'amount' => $amount, 'tid' => $category);
  $queue->createItem($queue_item);
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