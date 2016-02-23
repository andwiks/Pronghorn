<?php

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
    $return["data"] = $order;

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
  
  //$user = user_load(43);

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
      
      //add to array payment
      $return["data"][] = $value_container[$key];
    }
    
    //check credit enable/disable setting
    if(variable_get('apps_services_v2_payment_list_credit_option', 1)){
      //get user's credit data
      $credit_totalamount = userpoints_get_current_points($user->uid, 'all');
      if(isset($credit_totalamount) && !empty($credit_totalamount) && $credit_totalamount > 0){
        $credit_formatted_totalamount = number_format($credit_totalamount,0,",",".");

        //add credit choice selection from payment list
        $credit_container = array();
        $credit_container["payment_code"] = "use_sepulsa_credit_option";
        $credit_container["subtitle"] = variable_get("apps_services_v2_payment_list_credit_subtitle_text", "gunakan sepulsa kredit untuk membayar");
        $credit_title = variable_get("apps_services_v2_payment_list_credit_title_text", "Sepulsa Kredit Rp @credit");
        $credit_title = str_replace("@credit", $credit_formatted_totalamount, $credit_title);
        $credit_container["title"] = $credit_title;
        $credit_container["credit_balance"] = $credit_totalamount;

        //add option to data container
        $return["data"][] = $credit_container;
      }
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