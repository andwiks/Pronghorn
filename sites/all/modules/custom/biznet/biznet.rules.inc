<?php

/**
 * Implements hook_rules_action_info().
 */
function biznet_rules_action_info() {
  $actions = array();

  $actions['biznet_topup'] = array(
    'label' => t('Biznet topup'),
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
