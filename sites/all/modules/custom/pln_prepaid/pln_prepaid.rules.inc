<?php

/**
 * @file
 */

/**
 * Implements hook_rules_action_info().
 */
function pln_prepaid_rules_action_info() {
  $actions = array();

  $actions['pln_prepaid_payment'] = array(
    'label' => t('PLN prepaid payment'),
    'group' => t('Kraken'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Pending/Payment Received Order'),
        'wrapped' => TRUE,
      ),
    ),
  );

  return $actions;
}
