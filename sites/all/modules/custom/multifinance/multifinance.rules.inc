<?php

/**
 * Implements hook_rules_action_info().
 */
function multifinance_rules_action_info() {
  $actions = array();

  $actions['multifinance_payment'] = array(
    'label' => t('Multifinance payment'),
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
