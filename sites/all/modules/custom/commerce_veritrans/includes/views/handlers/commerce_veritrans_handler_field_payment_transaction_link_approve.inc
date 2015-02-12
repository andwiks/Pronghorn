<?php
/**
 * Field handler to present an approve payment transaction link.
 */
class commerce_veritrans_handler_field_payment_transaction_link_approve extends commerce_payment_handler_field_payment_transaction_link {

  function render($values) {
    // Get transaction id.
    $transaction_id = $this->get_value($values, 'transaction_id');
    // Get order id.
    $order_id = $this->get_value($values, 'order_id');
    // Get transaction data.
    $transaction = commerce_payment_transaction_load($transaction_id);
    // Check permission and order status.
    if (commerce_payment_transaction_access('update', $transaction)
      && $transaction->status == COMMERCE_PAYMENT_STATUS_PENDING
    ) {
      // Return approve link.
      return l(t('approve'), 'admin/commerce/orders/' . $order_id . '/payment/' . $transaction_id .'/approve', array('query' => drupal_get_destination()));
    }
  }
}