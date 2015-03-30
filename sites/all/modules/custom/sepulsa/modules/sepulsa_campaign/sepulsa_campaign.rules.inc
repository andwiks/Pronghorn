<?php

/**
 * Implements hook_rules_condition_info().
 */
function sepulsa_campaign_rules_condition_info() {
  $conditions['sepulsa_campaign_method_instance_comparison'] = array(
    'label' => t('Payment method instance comparison'),
    'parameter' => array(
      'commerce_order' => array(
        'label' => t('Order'),
        'type' => 'commerce_order',
      ),
      'instance_id' => array(
        'type' => 'text',
        'label' => t('Payment method instance'),
        'options list' => 'sepulsa_campaign_rules_payment_method_instance_options_list',
        'restriction' => 'input',
      ),
    ),
    'group' => t('Commerce Payment'),
  );

  return $conditions;
}

function sepulsa_campaign_method_instance_comparison_execute($order, $instance_id) {
  if (!empty($order->data['payment_method'])) {
    list($selected_method_id, $selected_instance_id) = explode('|', $order->data['payment_method']);
  }
  else {
    $selected_instance_id = '-none-';
  }

  return $instance_id == $selected_instance_id;
}

function sepulsa_campaign_rules_payment_method_instance_options_list() {
  $rules = entity_load('rules_config', FALSE, array('active' => TRUE, 'event' => 'commerce_payment_methods'));

  $options = array();
  foreach ($rules as $rule) {
    $options[$rule->name] = $rule->label;
  }

  return $options;
}
