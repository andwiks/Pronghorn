<?php
/**
 * @file
 * views_bank_transfer.views.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Implements hook_views_data_alter
 */
function views_bank_transfer_views_data_alter(&$data) {
  // Expose the transaction payment instance.
  $data['commerce_payment_transaction']['instance_id'] = array(
    'title' => t('Payment instance'),
    'help' => t('The payment instance of the transaction.'),
    'field' => array(
      'handler' => 'commerce_payment_handler_field_payment_instance',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'commerce_payment_handler_filter_payment_instance',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
}
