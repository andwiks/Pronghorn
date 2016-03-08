<?php
/**
 * @file
 * bank_transfer_notification.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function bank_transfer_notification_default_rules_configuration() {
  $configs = array();
  $rule = '{ "rules_bank_transfer_remainder" : {
      "LABEL" : "Bank Transfer Remainder",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "remainder" ],
      "REQUIRES" : [ "rules", "mimemail", "bank_transfer_notification" ],
      "ON" : { "bank_transfer_notification_queue_email" : [] },
      "DO" : [
        { "mimemail" : {
            "key" : "bank_transfer_remainder",
            "to" : [ "commerce-order:mail" ],
            "subject" : "Order ID [commerce-order:order-id] belum di bayar",
            "body" : "Order ini akan dicancel secara automatis bila proses pembayaran tidak diselesaikan dalam waktu 1 jam. Untuk menyelesaikan proses pembayaran silahkan transfer ke :\r\nNama Bank : \u003Cstrong\u003E[commerce-order:account_bank]\u003C\/strong\u003E\r\nNomor rekening : \u003Cstrong\u003E[commerce-order:account_number]\u003C\/strong\u003E\r\nAtas nama : \u003Cstrong\u003E[commerce-order:account_owner]\u003C\/strong\u003E\r\nKantor cabang : \u003Cstrong\u003E[commerce-order:account_branch]\u003C\/strong\u003E\r\n[commerce-order:policy]",
            "language" : [ "commerce-order:state" ]
          }
        }
      ]
    }
  }';
  $configs['rules_bank_transfer_remainder'] = rules_import($rule);
  return $configs;
}
