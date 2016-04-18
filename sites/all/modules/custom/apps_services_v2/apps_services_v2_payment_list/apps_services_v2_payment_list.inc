<?php

/**
 * Function apps_services_v2_payment_list_format_order().
 * add some addtional data to order
 *
 * @return object
 *   Order object.
 */
function apps_services_v2_payment_list_format_order($order) {
  // Get order wrapper.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  // Proccess order line item: remove language none (und).
  $order->commerce_line_items = $order_wrapper->commerce_line_items->value();
  // Looping foreach line item.
  $temp_commerce_line_items = array();
  foreach ((array) $order->commerce_line_items as $key => $line_item) {
    $data_temp = order_services_line_item($line_item);
    $data_temp->id = $key;
    $temp_commerce_line_items[] = $data_temp;
  }
  $order->commerce_line_items = $temp_commerce_line_items;

  $order->commerce_coupons = $order_wrapper->commerce_coupons->value();
  foreach ((array) $order->commerce_coupons as &$coupon) {
    $coupon_wrapper = entity_metadata_wrapper('commerce_coupon', $coupon);
    $coupon->commerce_discount_reference = $coupon_wrapper->commerce_discount_reference->value();

    foreach ((array) $coupon->commerce_discount_reference as &$discount) {
      $discount_wrapper = entity_metadata_wrapper('commerce_discount', $discount);
      $discount->commerce_discount_offer = $discount_wrapper->commerce_discount_offer->value();
    }
    // $order->commerce_coupons[$key] = $coupon;
  }

  // Proccess order total: remove language none (und).
  $order->commerce_order_total = $order_wrapper->commerce_order_total->value();

  // Support for iOS pln prepaid.
  $flag_array_changed = 0;
  if (isset($order->commerce_line_items) && !empty($order->commerce_line_items)) {
    foreach ($order->commerce_line_items as $lid => $line_item) {
      if (isset($line_item->type) && $line_item->type == 'pln_prepaid') {
        $flag_array_changed = 1;
        $order->commerce_line_items[$lid]->type = 'electricity_prepaid';
        if (isset($order->commerce_line_items[$lid]->commerce_product->type)) {
          $order->commerce_line_items[$lid]->commerce_product->type = 'electricity_prepaid';
        }
      }
    }
  }
  
  if($flag_array_changed){
    //change line item format to default array
    $data_temp_commerce_line_items = array();
    foreach ($order->commerce_line_items as $lid => $line_item) {
      //set field value into null if data is empty
      foreach($line_item as $field_name=>$field_value){
        if(is_array($line_item->$field_name) && count($line_item->$field_name) < 1){
          $line_item->$field_name = null;
        }
        elseif(isset($line_item->$field_name) && empty ($line_item->$field_name)){
          $line_item->$field_name = null;
        }
      }
      $data_temp_commerce_line_items[] = $line_item;
    }
    $order->commerce_line_items = $data_temp_commerce_line_items;
  }
  
  return $order;
}

/*
 * update/refresh order based on payment method
 */
function apps_services_v2_payment_list_checkout_payment($payment_method, $use_credit, $source){
  global $user;
  
  $return = array();
  $return["status"] = false;
  $return["message"] = "";
  $return["data"] = null;
  
  //check credit enable/disable setting
  if(variable_get('apps_services_v2_payment_list_credit_option', 1)){
    if($use_credit > 0 && isset($_SESSION['use_credit'])){
      unset($_SESSION['use_credit']);
    }
    else{
      $_SESSION["use_credit"] = false;
    }
  }
  
  // Get user's order.
  $order = commerce_cart_order_load($user->uid);
  // Check whether order can checkout.
  if ($order !== FALSE && commerce_checkout_order_can_checkout($order)) {
    $order->data['payment_method'] = $payment_method;
    commerce_order_save($order);
    
    $return["status"] = true;
    commerce_cart_order_refresh($order);
    $return["data"] = apps_services_v2_payment_list_format_order($order);

    return $return;
  }

  // Reach this means error: invalid order.
  return services_error(t('Invalid order'), 406);
}

/* 
 * get payment list with credit options
 * joined version from the old payment list
 */
function apps_services_v2_payment_list_data($source){
  global $user;
  
  //set default return value
  $return = array();
  $return["status"] = false;
  $return["message"] = "";
  $return["data"] = null;

  // Get user's order: do not care the result, only to satisfy drupal get form.
  $order = commerce_cart_order_load($user->uid);

  // Get checkout page object.
  $checkout_page = commerce_checkout_page_load('checkout');

  // Load commerce_checkout.pages.inc file.
  module_load_include('inc', 'commerce_checkout', 'includes/commerce_checkout.pages');

  // Load checkout form: based on order and checkout page object.
  $form = drupal_get_form('commerce_checkout_form_checkout', $order, $checkout_page);

  // Filter sensitive data.
  $form = apps_services_v2_payment_list_filter_data($form);
  
  //set default parameter as /checkout/commerce_payment/payment_methods/value
  $pane = "commerce_payment";
  $field = "payment_methods";
  $attribute = "value";
  
  //get value data
  $value_container = "";
  if (isset($form[$pane][$field][$attribute])) {
    $value_container = $form[$pane][$field][$attribute];
  }
  elseif (isset($form[$pane][$field]['#' . $attribute])) {
    $value_container = $form[$pane][$field]['#' . $attribute];
  }
  
  //set default parameter as /checkout/commerce_payment/payment_method/options
  $pane = "commerce_payment";
  $field = "payment_method";
  $attribute = "options";
  
  //get option data
  $option_container = "";
  if (isset($form[$pane][$field][$attribute])) {
    $option_container = $form[$pane][$field][$attribute];
  }
  elseif (isset($form[$pane][$field]['#' . $attribute])) {
    $option_container = $form[$pane][$field]['#' . $attribute];
  }
  
  //add the option title
  if(!empty($value_container)){
    foreach($value_container as $key=>$value){
      if(isset($option_container[$key]) && !empty($option_container[$key])){
        $value_container[$key]["title"] = $option_container[$key];
      }
      else{
        $value_container[$key]["title"] = "";
      }
      $value_container[$key]["type"] = "payment_option";
      $value_container[$key]["payment_code"] = $key;
      
      //set field value into null if data is empty
      foreach($value_container[$key] as $field_name=>$field_value){
        if(is_array($value_container[$key][$field_name]) && count($value_container[$key][$field_name]) < 1){
          $value_container[$key][$field_name] = null;
        }
        elseif(isset($value_container[$key][$field_name]) && empty (($value_container[$key][$field_name]))){
          $value_container[$key][$field_name] = null;
        }
      }
      
      //add to array payment
      $return["data"][] = $value_container[$key];
    }
    
    //check credit enable/disable setting
    if(variable_get('apps_services_v2_payment_list_credit_option', 1)){
      //get user's credit data
      $credit_totalamount = userpoints_get_current_points($user->uid, 'all');
      $credit_formatted_totalamount = number_format($credit_totalamount,0,",",".");

      //add credit choice selection from payment list
      $credit_container = array();
      $credit_container["payment_code"] = "use_sepulsa_credit_option";
      $credit_container["amount"] = $credit_totalamount;
      $credit_container["formatted_amount"] = $credit_formatted_totalamount;
      $credit_container["currency_code"] = commerce_default_currency();

      //add option to data container
      $return["data"][] = $credit_container;
    }
    
    //return the data with value and option title in it
    $return["status"] = true;
    return $return;
  }
  
  $return["message"] = "Data Pembayaran tidak ditemukan";
  return $return;
}

/**
 * Function apps_services_v2_payment_list_filter_data().
 *
 * Filter internal form value and Veritrans Server Key.
 *
 * @param $value
 *   Checkout form array.
 *
 * @return array
 *   Filtered form array.
 */
function apps_services_v2_payment_list_filter_data($form) {
  // Looping for each form dat.
  foreach ($form as $key => $value) {
    // Check for internal form values.
    if (preg_match('/^\#|^form/', $key)) {
      unset($form[$key]);
    }
    // Check for VT secret key.
    if (isset($form['commerce_payment']['payment_methods']['#value'])) {
      // Looping for each payment methods.
      foreach ($form['commerce_payment']['payment_methods']['#value'] as $payment_key => $payment_value) {
        if (isset($payment_value['settings']['server_key'])) {
          unset($form['commerce_payment']['payment_methods']['#value'][$payment_key]['settings']['server_key']);
        }
      }
    }
  }
  // Return filtered form.
  return $form;
}