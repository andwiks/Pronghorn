<?php

/*
 * cek if order id is user's or not
 */
function apps_services_v2_transaction_check_if_current_active_user_owned_order($user, $order_id){
  if ($order = commerce_order_load($order_id)) {
    // Return the account's access to update the order.
    if(commerce_order_access('view', $order, $user)){
      return $order;
    }
  }
  
  return false;
}

/* 
 * process favorited past transaction of user
 * if already favorite then remove favorite
 */
function apps_services_v2_transaction_data_favorite($order_id, $source){
  global $user;
  
  //get user uid
  $uid = $user->uid;
  
  //do not allow user anonymous to enter the service
  if(!isset($uid) || empty($uid)){
    return services_error('Harap login terlebih dahulu', 406);
  }
  
  //validate order id, check if it actually owned by active user
  $validate_order_id = apps_services_v2_transaction_check_if_current_active_user_owned_order($user, $order_id);
  if(!$validate_order_id){
    return services_error('Order tidak sesuai dengan user aktif', 406);
  }
  
  $return = array();
  $return["status"] = false;
  $return["message"] = "";
  $return["data"] = null;
    
  //add or remove favorite transaction
  //check if order_id already exist
  $num_favorited_order = db_select('favorite_transaction', 'ft')
  ->fields('ft')
  ->condition('order_id', $order_id)
  ->countQuery()
  ->execute()
  ->fetchField();

  //insert data if not exist
  if($num_favorited_order < 1){
    db_insert('favorite_transaction')
    ->fields(array(
      'order_id' => $order_id,
      'uid' => $uid,
      'created' => REQUEST_TIME,
    ))
    ->execute();
    $return["status"] = true;
    $return["message"] = "Order difavoritkan";
  }
  else{
    //delete data if exist
    db_delete('favorite_transaction')
    ->condition('order_id', $order_id)
    ->execute();
    $return["status"] = true;
    $return["message"] = "Order telah tidak difavoritkan";
  }

  return $return;
}

/*
 * change the format of order listing
 */
function apps_services_v2_transaction_change_format_order($order_container, $type=null){
  
  //get all available operator data
  $operator = sepulsa_get_all_operator_data();
  //reorganize the returned operator for easy process
  $operator_array = array();
  foreach($operator as $data_operator){
    foreach($data_operator["prefix"] as $prefix){
      $operator_array[$prefix] = $data_operator["operator"];
    }
  }
  
  //change return value of order container
  $selected_returned_value = array();
  foreach($order_container as $order){
    $temp_array_data = array();
    $temp_array_data["order_id"] = $order->order_id;
    $temp_array_data["mail"] = $order->mail;
    $temp_array_data["order_status"] = $order->status;
    $temp_array_data["created"] = $order->created;
    //if accessed from home listing use first product price instead
    if($type == "home" && isset($order->commerce_line_items[0]->commerce_product->commerce_price["und"][0]["amount"]) && $order->commerce_line_items[0]->commerce_product->commerce_price["und"][0]["amount"] > 0){
      $temp_array_data["amount"] = $order->commerce_line_items[0]->commerce_product->commerce_price["und"][0]["amount"];
    }
    else{
      $temp_array_data["amount"] = $order->commerce_order_total['amount'];
    }
    $temp_array_data["product_amount"] = $order->commerce_line_items[0]->commerce_product->commerce_price["und"][0]["amount"];
    $temp_array_data["currency_code"] = $order->commerce_order_total['currency_code'];
    $temp_array_data["response"] = $order;
    $temp_array_data["product_id"] = $order->commerce_line_items[0]->commerce_product->product_id;
    $temp_array_data["title"] = $order->commerce_line_items[0]->commerce_product->title;
    
    //get type order based on many case
    if(isset($order->commerce_line_items[0]->type) && !empty($order->commerce_line_items[0]->type)){
      $temp_array_data["type"] = $order->commerce_line_items[0]->type;
    }
    elseif(isset($order->commerce_line_items->type) && !empty($order->commerce_line_items->type)){
      $temp_array_data["type"] = $order->commerce_line_items->type;
    }
    
    //get phone number order based on many case
    if(isset($order->commerce_line_items[0]->field_phone_number) && !empty($order->commerce_line_items[0]->field_phone_number)){
      $temp_array_data["field_phone_number"] = $order->commerce_line_items[0]->field_phone_number;
    }
    elseif(isset($order->commerce_line_items->field_phone_number) && !empty($order->commerce_line_items->field_phone_number)){
      $temp_array_data["field_phone_number"] = $order->commerce_line_items->field_phone_number;
    }
    else{
      $temp_array_data["field_phone_number"] = null;
    }
    
    //get electricity customer number order based on many case
    if(isset($order->commerce_line_items[0]->electricity_customer_number) && !empty($order->commerce_line_items[0]->electricity_customer_number)){
      $temp_array_data["electricity_customer_number"] = $order->commerce_line_items[0]->electricity_customer_number;
    }
    elseif(isset($order->commerce_line_items->electricity_customer_number) && !empty($order->commerce_line_items->electricity_customer_number)){
      $temp_array_data["electricity_customer_number"] = $order->commerce_line_items->electricity_customer_number;
    }
    else{
      $temp_array_data["electricity_customer_number"] = null;
    }
    
    //if the one exist is field_customer_number use it for electricity_customer_number
    if(isset($order->commerce_line_items[0]->field_customer_number) && !empty($order->commerce_line_items[0]->field_customer_number)){
      $temp_array_data["electricity_customer_number"] = $order->commerce_line_items[0]->field_customer_number;
    }
    elseif(isset($order->commerce_line_items->field_customer_number) && !empty($order->commerce_line_items->field_customer_number)){
      $temp_array_data["electricity_customer_number"] = $order->commerce_line_items->field_customer_number;
    }
    
    //get electricity phone number
    if(isset($order->commerce_line_items[0]->electricity_phone_number) && !empty($order->commerce_line_items[0]->electricity_phone_number)){
      $temp_array_data["electricity_phone_number"] = $order->commerce_line_items[0]->electricity_customer_number;
    }
    elseif(isset($order->commerce_line_items->electricity_phone_number) && !empty($order->commerce_line_items->electricity_phone_number)){
      $temp_array_data["electricity_phone_number"] = $order->commerce_line_items->electricity_phone_number;
    }
    else{
      if($temp_array_data["type"] == "electricity_prepaid"){
        $temp_array_data["electricity_phone_number"] = $temp_array_data["field_phone_number"];
      }
      else{
        $temp_array_data["electricity_phone_number"] = null;
      }
    }
    
    //add operator returned value based on phone number
    if(isset($temp_array_data["type"]) && $temp_array_data["type"] == "mobile_prepaid"){
      $first_4_digit = substr($temp_array_data["field_phone_number"], 0, 4);
      if(isset($operator_array[$first_4_digit]) && !empty($operator_array[$first_4_digit])){
        $temp_array_data["operator_title"] = $operator_array[$first_4_digit];
      }
      else{
        $temp_array_data["operator_title"] = "Pulsa Prabayar";
      }
    }
    elseif(isset($temp_array_data["type"]) && ($temp_array_data["type"] == "electricity_prepaid" || $temp_array_data["type"] == "pln_prepaid")){
      $temp_array_data["operator_title"] = "Listrik Prabayar";
    }
    elseif(isset($temp_array_data["type"]) && $temp_array_data["type"] == "deposit_sepulsa"){
      $temp_array_data["operator_title"] = "Deposit Sepulsa";
    }
    else{
      $temp_array_data["operator_title"] = "Transaksi Sepulsa";
    }
    
    //set favorite based on list data
    if(isset($order->type) && !empty($order->type) && $order->type == "favorite"){
      $temp_array_data["favorite"] = true;
    }
    elseif(isset($order->type) && !empty($order->type) && $order->type == "order"){
      $temp_array_data["favorite"] = false;
    }
    else{
      $temp_array_data["favorite"] = false;
    }
    
    $temp_array_data["revision_timestamp"] = $order->revision_timestamp;
    
    $selected_returned_value[] = $temp_array_data;
  }
  
  return $selected_returned_value;
}

/*
 * get the list of order history
 */
function apps_services_v2_transaction_history_list($page, $size, $source){
  global $user;
  
  //get current active user uid
  $uid = $user->uid;
  
  //do not allow user anonymous to enter the service
  if(!isset($uid) || empty($uid)){
    return services_error('Harap login terlebih dahulu', 406);
  }
  
  //set variable container
  $return = array();
  $return["status"] = false;
  $return["message"] = "";
  $return["data"] = null;
  $favorite_order_id = array();
  
  //get favorite transaction order id from the database
  $result = db_select('favorite_transaction', 'ft')
            ->fields('ft', array('order_id', 'uid', 'created'))
            ->condition('uid', $uid)
            ->orderBy('created', 'DESC')
            ->execute();
  foreach ($result as $usage) {
    $favorite_order_id[] = $usage->order_id;
  }
  
  //set allowed order status list
  $allowed_cart_status = array("pending", "processing", "invoiced", "completed", "canceled");
  
  //get order list
  $order_list = commerce_order_load_multiple(array(), array('uid' => $uid));
  
  //set order desc
  $order_list = array_reverse($order_list,true);
  
  $start = ($page-1) * $size;
  $stop = $start + $size;
  
  //loop order to get order data
  $favorite_order_container = array();
  $latest_order_container = array();
  $i = 1;
  
  foreach ($order_list as $key => $value) {
    //include only allowed status order
    if(isset($value->status) && in_array($value->status, $allowed_cart_status)){
      if($i > $start && $i <= $stop){
        if (in_array($key, $favorite_order_id)){
            //set type favorite
            $value->type = "favorite";
            //get favorite order value
            $favorite_order_container[] = apps_services_v2_transaction_format_order_home($value);
        }
        else {
          //set type order
          $value->type = "order";
          //get latest order value if favorite is more than 0
          $latest_order_container[] = apps_services_v2_transaction_format_order_home($value);
        }
      }
      $i++;
    }
  }
  
  //merge favorite and latest order data into one container
  $order_container = array_merge($favorite_order_container,$latest_order_container);

  //change the returned format
  $order_container = apps_services_v2_transaction_change_format_order($order_container);
  
  if(count($order_container)){
    $return["status"] = true;
    $return["message"] = "Order list history pengguna";
    $return["data"] = $order_container;
  }
  else{
    $return["status"] = false;
    $return["message"] = "Order list tidak ditemukan";
    $return["data"] = null;
  }
  
  return $return;
}

/*
 * get the list of favorite order or latest order (maximum 5)
 * might also have product promotion data on it (currently excluded for now)
 */
function apps_services_v2_transaction_home_list($size, $favorite_only, $source){
  global $user;
  
  //get current active user uid
  $uid = $user->uid;
  
  //do not allow user anonymous to enter the service
  if(!isset($uid) || empty($uid)){
    return services_error('Harap login terlebih dahulu', 406);
  }
  
  //set variable container
  $return = array();
  $return["status"] = false;
  $return["message"] = "";
  $return["data"] = null;
  $favorite_order_id = array();
  
  //get favorite transaction order id from the database
  $result = db_select('favorite_transaction', 'ft')
            ->fields('ft', array('order_id', 'uid', 'created'))
            ->condition('uid', $uid)
            ->orderBy('created', 'DESC')
            ->execute();
  foreach ($result as $usage) {
    $favorite_order_id[] = $usage->order_id;
  }
  
  //get order list
  $order_list = commerce_order_load_multiple(array(), array('uid' => $uid));
  //set order desc
  $order_list = array_reverse($order_list,true);
  
  //set allowed order status list
  $allowed_cart_status = array("pending", "processing", "invoiced", "completed");
  
  //check how many favourite available in the return order list
  $count_favorite_order = 0;
  foreach ($order_list as $key => $value) {
    if (in_array($key, $favorite_order_id)){
      $count_favorite_order++;
    }
  }
  
  //check if favorite order already more than "$size", if still less then get latest order to complement
  $number_of_latest_order_needed = 0;
  if($count_favorite_order < $size){
    $number_of_latest_order_needed = $size - $count_favorite_order;
  }
  
  //loop order to get order data
  $favorite_order_container = array();
  $latest_order_container = array();
  $count_favorite_container = 0;
  foreach ($order_list as $key => $value) {
    //include only allowed status order
    if(isset($value->status) && in_array($value->status, $allowed_cart_status)){
      if (in_array($key, $favorite_order_id)){
        if($count_favorite_container < $size){
          //set type favorite
          $value->type = "favorite";
          //get favorite order value
          $favorite_order_container[] = apps_services_v2_transaction_format_order_home($value);
          $count_favorite_container++;
        }
      }
      elseif ($favorite_only == 0 && $number_of_latest_order_needed > 0) {
        //set type order
        $value->type = "order";
        //get latest order value if favorite is more than 0
        $latest_order_container[] = apps_services_v2_transaction_format_order_home($value);
        $number_of_latest_order_needed--;
      }
    }
  }
  
  //merge favorite and latest order data into one container
  $order_container = array_merge($favorite_order_container,$latest_order_container);

  //fill the order with latest order data as last filter, if actually no favorite order found in the current list
  $i = 0;
  if($favorite_only == 0 && !count($order_container)){  
    foreach ($order_list as $key => $value) {
      //include only allowed status order
      if(isset($value->status) && in_array($value->status, $allowed_cart_status)){
        if ($i < $size){
          //set type order
          $value->type = "order";
          //get latest order value
          $order_container[] = apps_services_v2_transaction_format_order_home($value);
          $i++;
        }
      }
    }
  }
  
  //change the returned format
  $order_container = apps_services_v2_transaction_change_format_order($order_container,"home");
  
  if(count($order_container)){
    $return["status"] = true;
    $return["message"] = "Order list halaman home pengguna";
    $return["data"] = $order_container;
  }
  else{
    $return["status"] = false;
    $return["message"] = null;
    $return["data"] = null;
  }
  
  //check if needed to include email request
  //temporary disable email request api, since not used
  /*$return["request_email"] = "";
  if(isset($user->mail) && !empty($user->mail)){
    $mail = explode("@", $user->mail);
    if (isset($mail[1]) && !empty($mail[1]) && strpos($mail[1], '.dummy') !== false) {
      $return["request_email"] = "Masukan email anda<br />untuk mendapatkan info-info dan fitur-fitur menarik";
    }
  }*/
  
  //get credit balance
  $return["credit_balance"] = userpoints_get_current_points($uid, 'all');
  
  return $return;
}

function apps_services_v2_transaction_format_order_home($order) {
  // Get order wrapper.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  // Proccess order line item: remove language none (und).
  $order->commerce_line_items = $order_wrapper->commerce_line_items->value();
  // Looping foreach line item.
  foreach ((array) $order->commerce_line_items as $key => $line_item) {
    $order->commerce_line_items[$key] = order_services_line_item($line_item);
  }

  // Proccess order total: remove language none (und).
  $order->commerce_order_total = $order_wrapper->commerce_order_total->value();

  // Support for iOS pln prepaid.
  if (isset($order->commerce_line_items) && !empty($order->commerce_line_items)) {
    foreach ($order->commerce_line_items as $lid => $line_item) {
      if (isset($line_item->type) && $line_item->type == 'pln_prepaid') {
        $order->commerce_line_items[$lid]->type = 'electricity_prepaid';
        if (isset($order->commerce_line_items[$lid]->commerce_product->type)) {
          $order->commerce_line_items[$lid]->commerce_product->type = 'electricity_prepaid';
        }
      }
  elseif(isset($line_item->type) && ($line_item->type == 'coupon' || $line_item->type == 'userpoints' || $line_item->type == 'fee')){
        unset($order->commerce_line_items[$lid]);
      }
    }
  }
  
  return $order;
}