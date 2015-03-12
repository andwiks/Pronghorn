<?php

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_google_analytics_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = 'Send google analytics code on checkout completion';
  $rule->active = TRUE;
  $rule->event('commerce_checkout_complete')
    ->action('commerce_google_analytics_send_order', array(
      'commerce_order:select' => 'commerce_order',
      'method' => GA_PUSH_METHOD_DEFAULT,
    )
  );
  $configs['commerce_google_analytics_rule_ga'] = $rule;

  // Default rule for storing Analytics cid on order save.
  $rule = rules_reaction_rule();
  $rule->label = 'Store google analytics session to order on checkout begin';
  $rule->active = TRUE;
  $rule->event('commerce_order_presave')
    ->condition('data_is', array(
      'data:select' => 'commerce-order:status',
      'value' => 'checkout_checkout',
    ))
    ->action('commerce_google_analytics_save_ga_session_to_order', array(
      'commerce_order:select' => 'commerce_order',
    )
  );
  $configs['commerce_google_analytics_rule_save_session'] = $rule;

  return $configs;
}
