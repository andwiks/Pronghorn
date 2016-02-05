<?php
/**
 * @file
 * commerce_payment_field_payment_instance.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Field handler to translate a payment intance ID into its readable form.
 */
class commerce_payment_handler_field_payment_instance extends views_handler_field {

  /**
   * {@inheritDoc}
   * @see views_handler_field::render()
   *
   * Check for bank transfer list and how to display it.
   */
  function render($values) {
    // Get value from values.
    $value = $this->get_value($values);
    // Get bank transfer option list.
    $bank_transfer_instances = views_bank_transfer_options_list();
    // Check if value belong to bank transfer list.
    return isset($bank_transfer_instances[$value]) ? $this->sanitize_value($bank_transfer_instances[$value]) : '';
  }
}
