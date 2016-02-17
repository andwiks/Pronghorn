<?php

/**
 * Function apps_services_v2_services_sepulsa_operator_list().
 *
 * Resources callback for sepulsa resource index.
 */
function apps_services_v2_services_sepulsa_operator_list() {
  // Define service friendly return value.
  $return = array();
  
  //set default return value
  $return["status"] = false;
  $return["message"] = "Data Operator tidak ditemukan";
  $return["data"] = array();
  
  // Set operator container data
  $data_container = array();
  
  // Get operator data.
  $operator = sepulsa_get_all_operator_data();
  
  // Looping for each operator data: contain term id and other operator data.
  foreach ($operator as $id => $data) {
    // Return only available cardtype and packet.
    if (isset($data['cardtype']) && !empty($data['cardtype'])
      && isset($data['packet']) && !empty($data['packet'])
    ) {
      
      //remove cardtype id from key then put it as parameter
      foreach ($data['cardtype'] as $id_cardtype => $data_cardtype) {
        $data['cardtype'] = array();
        $data['cardtype']['id'] = $id_cardtype;
        $data['cardtype']['type'] = $data_cardtype;
      }
      
      //remove packet id
      foreach ($data['packet'] as $id_packet => $data_packet) {
        $data['packet'] = $data_packet;
      }
      
      // Include id into operator data.
      $data_container[] = array('id' => $id) + $data;
      
    }
  }
  
  //set value return if data is not empty
  if(count($data_container) > 0){
    $return["status"] = true;
    $return["message"] = "";
    $return["data"] = $data_container;
  }
  
  // Return service friendly return value.
  return $return;
}