<?php
/**
 * @file
 * Implements the different hook required by Drupal Commerce module to display specific Checkout Panes
 */


/**
 * Form implementation set in userpoints_sepulsa_commerce_checkout_pane_info().
 * Calculate userpoints and add the userpoints line item in checkout form
 */
function userpoints_sepulsa_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
    $line_item_id = commerce_userpoints_discount_order_has_userpoint_line_item($order->order_id);
    $point_amount = userpoints_get_current_points($order->uid, 'all');
    // If there is no existing userpoints line item, we create a new one.
    if ($line_item_id == NULL && $point_amount > 0) {
        $order_total = $order->commerce_order_total[LANGUAGE_NONE][0]['amount'];
        if ($order_total > $point_amount) $amount = $point_amount;
        else $amount = $order_total;
        // Create the line item.
        $line_item = commerce_userpoints_discount_line_item_factory_new($order->order_id);

        // Create the userpoints transaction.
        $userpoints_params = array(
          'points' => $amount * -1,
          'uid' => $order->uid,
          'operation' => 'commerce_userpoints_discount_credit',
          'tid' => variable_get('commerce_userpoints_discount_default_termid', userpoints_get_default_tid()),
          'display' => FALSE,
          'entity_id' => $order->order_id,
          'entity_type' => 'commerce_order',
          'txn_id' => null,
        );

        $label = "User credit";
        commerce_userpoints_discount_line_item_populate(
          $line_item, $order->order_id,
          $userpoints_params, commerce_default_currency(), $label);

        commerce_userpoints_discount_line_item_save($line_item);
    }
}

