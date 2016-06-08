<?php
/**
 * @file
 * Commerce_veritrans.rules_defaults.inc.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_veritrans_default_rules_configuration() {
  $configs = array();

  $conditions = array(
    'event' => 'commerce_payment_methods',
    'plugin' => 'reaction rule',
    'active' => TRUE,
    'owner' => 'rules',
  );
  $commerce_payment_methods = entity_load('rules_config', FALSE, $conditions);
  $rules = array();
  foreach ($commerce_payment_methods as $key) {
    $payment = commerce_payment_method_instance_load('commerce_veritrans|' . $key->name);
    if (!empty($payment['settings'])) {
      $settings = $payment['settings'];
      if ($settings['type'] == 'bank_transfer' && !empty($settings['bank_transfer']['bank'])) {
        $rules[] = $key->name;
      }
    }
  }

  for ($i = 0, $total = count($rules); $i < $total; $i++) {
    if (!empty($value)) {
      $value = '"value" : { "' . $rules[$i] . '" : "' . $rules[$i] . '"';
    }
    else {
      $value .= ' , "value" : { "' . $rules[$i] . '" : "' . $rules[$i] . '"';
    }
  }

  $value_rules = '';

  for ($i = 0, $total = count($rules); $i < $total; $i++) {
    if (empty($value_rules)) {
      $value_rules = '"' . $rules[$i] . '" : "' . $rules[$i] . '"';
    }
    else {
      $value_rules .= ' , "' . $rules[$i] . '" : "' . $rules[$i] . '"';
    }
  }

  $rule = '{ "rules_permata_virtual_account_notification" : {
      "LABEL" : "Permata Virtual Account Notification",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Permata", "VA" ],
      "REQUIRES" : [ "sepulsa_campaign", "rules", "mimemail", "commerce_checkout" ],
      "ON" : { "commerce_checkout_complete" : [] },
      "IF" : [
        { "sepulsa_campaign_method_instance_comparison" : {
            "commerce_order" : [ "commerce_order" ],
            "instance_ids" : { "value" : { ' . $value_rules . ' } }
          }
        }
      ],
      "DO" : [
        { "mimemail" : {
            "key" : "commerce_checkout_complete",
            "to" : [ "commerce-order:mail" ],
            "from_name" : "Test Dev Permata",
            "subject" : "Order [commerce-order:order-number] at [site:name]",
            "body" : "Email Pengingat (bisa di ganti).\r\nBank : permata\r\nVA Account : [commerce-order:veritrans-virtual-account]\r\ntotalnya : [commerce-order:commerce-order-total]\r\n\r\njangan lupa bayar ya",
            "language" : [ "commerce-order:state" ]
          }
        }
      ]
    }
  }';
  $configs['rules_permata_virtual_account_notification'] = rules_import($rule);
  return $configs;
}
