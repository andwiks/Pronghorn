<?php

/**
 * Implements hook_views_query_alter().
 */
function kupon_views_query_alter(&$view, &$query) {
  if ($view->name == 'coupon' && ($view->current_display == 'page' || $view->current_display == 'coupon')) {
    //getting total price on cart
    global $user;
    $order = commerce_cart_order_load($user->uid);
    if (empty($order->commerce_line_items)) {
      //don't allow access coupon page if cart empty
      //drupal_goto("<front>");
    }
    $wrapper = entity_metadata_wrapper('commerce_order', $order);
    $line_items = $wrapper->commerce_line_items;
    $total = commerce_line_items_total($line_items);
    
    //change the views SQL here
    $query->where[1]['conditions'][4]['value'] = $total['amount'];
  }
}
