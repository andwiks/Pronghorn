<?php

/**
 * Implements hook_rules_condition_info().
 */
function commerce_veritrans_rules_condition_info() {
  $conditions['commerce_veritrans_method_instance_comparison'] = array(
    'label' => t('Payment method instance comparison'),
    'parameter' => array(
      'commerce_payment_transaction' => array(
        'label' => t('Payment Transaction'),
        'type' => 'commerce_payment_transaction',
      ),
      'instance_id' => array(
        'type' => 'text',
        'label' => t('Payment method instance'),
        'options list' => 'commerce_veritrans_rules_payment_method_instance_options_list',
        'restriction' => 'input',
      ),
    ),
    'group' => t('Commerce Payment'),
    'callbacks' => array(
      'execute' => 'commerce_veritrans_rules_compare_selected_payment_method_instance',
    ),
  );

  return $conditions;
}

function commerce_veritrans_rules_payment_method_instance_options_list() {
  $rules = entity_load('rules_config', FALSE, array('active' => TRUE, 'event' => 'commerce_payment_methods'));

  $options = array();
  foreach ($rules as $rule) {
    $options[$rule->name] = $rule->label;
  }

  return $options;
}

function commerce_veritrans_rules_compare_selected_payment_method_instance($payment_transaction, $instance_id) {
  return $payment_transaction->instance_id ==  implode('|', array($payment_transaction->payment_method, $instance_id));
}
