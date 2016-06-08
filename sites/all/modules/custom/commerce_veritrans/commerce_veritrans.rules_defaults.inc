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
            "instance_ids" : { "value" : { "rules_permata_virtual_account" : "rules_permata_virtual_account" } }
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
