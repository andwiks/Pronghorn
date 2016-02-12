<?php

/*
 * cek if order id is user's or not
 */
function apps_services_v2_transaction_check_if_current_active_user_owned_order($user, $order_id, $source){
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
function apps_services_v2_transaction_data_favorite($order_id){
  global $user;
  
  //get user uid
  $uid = $user->uid;
  
  //do not allow user anonymous to enter the service
  if(!isset($uid) || empty($uid)){
    return services_error('Harap login terlebih dahulu', 406);
  }
  
  //validate order id, check if it actually owned by active user
  $validate_order_id = apps_services_v2_transaction_check_if_current_active_user_owned_order($order_id);
  if(!$validate_order_id){
    return services_error('Order tidak sesuai dengan user aktif', 406);
  }
  
  $return = array();
    
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
      $return["message"] = "Order difavoritkan";
    }
    else{
      //delete data if exist
      db_delete('favorite_transaction')
      ->condition('order_id', $order_id)
      ->execute();
      $return["message"] = "Order telah tidak difavoritkan";
    }
    
    return $return;
}

/*
 * get the list of favorite order or latest order (maximum 5)
 * might also have product promotion data on it (currently excluded for now)
 */
function apps_services_v2_transaction_home_list($size,$favorite_only, $source){
  global $user;
  
  //get current active user uid
  $uid = $user->uid;
  
  //do not allow user anonymous to enter the service
  if(!isset($uid) || empty($uid)){
    return services_error('Harap login terlebih dahulu', 406);
  }
  
  //set variable container
  $return = array();
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
  
  //check how many favourite available in the return order list
  $count_favorite_order = 0;
  foreach ($order_list as $key => $value) {
    if (in_array($key, $favorite_order_id)){
      $count_favorite_order++;
    }
  }
  
  //check if favorite order already more than "$size", if still less then get latest order to complement
  $latest_order_list = "";
  $number_of_latest_order_needed = 0;
  if($count_favorite_order < $size){
    $number_of_latest_order_needed = $size - $count_favorite_order;
  }
  
  //loop order to get order data
  $favorite_order_container = array();
  $latest_order_container = array();
  $count_favorite_container = 0;
  foreach ($order_list as $key => $value) {
    if (in_array($key, $favorite_order_id)){
      if($count_favorite_container < $size){
        //set type favorite
        $value->type = "favorite";
        //get favorite order value
        $favorite_order_container[] = $value;
        $count_favorite_container++;
      }
    }
    elseif ($favorite_only == 0 && $number_of_latest_order_needed > 0) {
      //set type order
      $value->type = "order";
      //get latest order value if favorite is more than 0
      $latest_order_container[] = $value;
      $number_of_latest_order_needed--;
    }
  }
  
  //merge favorite and latest order data into one container
  $order_container = array_merge($favorite_order_container,$latest_order_container);

  //fill the order with latest order data as last filter, if actually no favorite order found in the current list
  $i = 0;
  if($favorite_only == 0 && !count($order_container)){  
    foreach ($order_list as $key => $value) {
      if ($i < $size){
        //set type order
        $value->type = "order";
        //get latest order value
        $order_container[] = $value;
        $i++;
      }
    }
  }
  
  if(count($order_container)){
    $return["message"] = "Order list halaman home pengguna";
    $return["data"] = $order_container;
  }
  else{
    $return["message"] = "Order list halaman home tidak ditemukan";
    $return["data"] = array();
  }
  
  //check if needed to include email request
  $return["request_email"] = "";
  if(isset($user->mail) && !empty($user->mail)){
    $mail = explode("@", $user->mail);
    if (isset($mail[1]) && !empty($mail[1]) && strpos($mail[1], '.dummy') !== false) {
      $return["request_email"] = "Masukan email anda<br />untuk mendapatkan info-info dan fitur-fitur menarik";
    }
  }
  
  return $return;
}