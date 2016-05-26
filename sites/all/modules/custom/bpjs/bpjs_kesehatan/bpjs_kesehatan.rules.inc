<?php

/**
 * @file
 */

/**
 * Implements hook_rules_action_info().
 */
function bpjs_kesehatan_rules_action_info() {
  $actions = array();

  $actions['bpjs_kesehatan_payment'] = array(
    'label' => t('BPJS Kesehatan payment'),
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
