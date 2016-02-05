<?php
/**
 * @file
 * commerce_payment_handler_filter_payment_instance.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Filter by payment instance.
 */
class commerce_payment_handler_filter_payment_instance extends views_handler_filter_in_operator {

  /**
   * {@inheritDoc}
   * @see views_handler_filter_in_operator::get_value_options()
   *
   * Check for filter option, if not available, get from option list.
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Bank Transfer');
      $this->value_options = views_bank_transfer_options_list();
    }
  }
}
