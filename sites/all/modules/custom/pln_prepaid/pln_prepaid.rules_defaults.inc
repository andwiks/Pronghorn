<?php

/**
 * @file
 */

/**
 * Implements hook_default_rules_configuration().
 */
function pln_prepaid_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = 'PLN prepaid payment';
  $rule->event('commerce_payment_order_paid_in_full')
    ->condition('commerce_order_contains_product_type', array('commerce_order:select' => 'commerce_order', 'product_type' => array('pln_prepaid' => 'pln_prepaid'), 'operator' => '>=', 'value' => 1))
    ->action('pln_prepaid_payment', array('commerce_order:select' => 'commerce_order'));

  $configs ['pln_prepaid_payment'] = $rule;
  return $configs;
}
