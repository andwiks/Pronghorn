<?php

function apps_services_v2_transaction_data_favorite($uid, $order_id){
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
      $return["message"] = "Favorite Order Id Success";
    }
    else{
      db_delete('favorite_transaction')
      ->condition('order_id', $order_id)
      ->execute();
      $return["message"] = "Remove Favorite Order Id Success";
    }
    
    return $return;
}