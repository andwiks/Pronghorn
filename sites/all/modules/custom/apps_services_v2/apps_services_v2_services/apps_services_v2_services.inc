<?php

/**
 * Function apps_services_v2_services_sepulsa_operator_list().
 *
 * Resources callback for sepulsa resource index.
 */
function apps_services_v2_services_sepulsa_operator_list($source) {
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

/**
 * Function apps_services_v2_services_sepulsa_product_index().
 *
 * Resources callback for sepulsa get product listing index.
 */
function apps_services_v2_services_sepulsa_product_index($type, $source) {
  // Backward compatibilty for old electricity product.
  if (module_exists('pln_prepaid') && $type == 'electricity') {
    $type = 'pln_prepaid';
  }
  
  //set return container and default value of return
  $return = array();
  $return["status"] = false;
  $return["message"] = "";
  $return["data"] = array();

  $products = &drupal_static(__FUNCTION__, array());
  
  //create cache name based on type and source
  $cache_name = 'apps_services_v2_services_sepulsa_all_products_' . $type . '_' . $source;

  if (empty($products)) {
    //get data from cache
    $cache = cache_get($cache_name, 'cache');

    if (empty($cache)) {
      $nodes = node_load_multiple(FALSE, array('type' => array_keys(commerce_product_reference_node_types())));
      $views = node_view_multiple($nodes, 'default');

      foreach ($views['nodes'] as $nid => $view) {
        if (isset($nodes[$nid])) {
          //generate temporary product container
          $product = array();
          
          $field_product = $view['field_product'][0];
          
          //set product name
          $product['product_name'] = $nodes[$nid]->type;
          
          //set returned data as type only if type parameter is not empty
          if(isset($type) && !empty($type) && $product['product_name'] != $type){
            continue;
          }
          
          //set container product detail
          $product['product'] = array();

          //set container product detail content
          $product['product']['title'] = $nodes[$nid]->title;
          $product['product']['id'] = $nid;
          
          //set product id container
          $product['product']['product_type'] = array();
          
          //get product id content
          if(isset($field_product["product_id"]['#options']) && count($field_product["product_id"]['#options']) > 0){
            foreach ($field_product["product_id"]['#options'] as $id_product_id => $name_product_id) {
              //set temporary container for product id content
              $product_id_container = array();
              $product_id_container['id'] = $id_product_id;
              $product_id_container['name'] = $name_product_id;
              $product['product']['product_type'][] = $product_id_container;
            }
          }
          elseif(isset($field_product['product_id']['#value']) && !empty($field_product['product_id']['#value'])){
            $product['product']['product_type']['id'] = $field_product['product_id']['#value'];
            $product['product']['product_type']['name'] = null;
          }
          
          //set quantity data
          $product['product']['quantity'] = null;
          if(isset($field_product['quantity']['#value']) && !empty($field_product['quantity']['#value'])){
            $product['product']['quantity'] = $field_product['quantity']['#value'];
          }
          
          $products[] = $product;
        }
      }
      
      //check if products data available after process
      //set before the cache so empty product will not be saved
      if(count($products) < 1){
        if(isset($type) && !empty($type)){
          return services_error(t('@type product tidak diketemukan.', array('@type' => $type)), 406);
        }
        else{
          //return failed
          $return["status"] = false;
          $return["message"] = "Data product tidak diketemukan";

          return $return;
        }
      }

      //set the cache
      cache_set($cache_name, $products, 'cache', CACHE_PERMANENT);
    }
    else {
      $products = $cache->data;
    }
  }

  //return success data
  $return["status"] = true;
  $return["data"] = $products;

  return $return;
}

/**
 * Function apps_services_v2_services_resource_add().
 *
 * @param array $product
 *   Product data.
 *
 * @return mixed
 *   Line item object, or string contain error message otherwise.
 */
function apps_services_v2_services_resource_add($product, $source) {
  global $user;
  
  //set return container and default value of return
  $return = array();
  $return["status"] = false;
  $return["message"] = "";
  $return["data"] = array();

  // Backward compatibilty for old electricity product.
  if (module_exists('pln_prepaid') && $product['type'] == 'electricity' || $product['type'] == 'electricity_prepaid') {
    $product['type'] = 'pln_prepaid';
    $product['fields'][0]['field_customer_number'] = $product['fields'][0]['electricity_customer_number'];
    $product['fields'][0]['field_phone_number'] = $product['fields'][0]['electricity_phone_number'];
    unset($product['fields'][0]['electricity_customer_number']);
    unset($product['fields'][0]['electricity_phone_number']);
  }

  // Check input product data:
  // 1. product_id: (int) valid product id loaded by commerce_product_load.
  // 2. type: (string) valid line item type. (optional: product).
  // 2. quantity: (int) how many quantity need to be added (optional: 1).
  // 3. fields: (array) key: field name, value: field value (optional: empty).
  if (isset($product['product_id']) && !empty($product['product_id'])) {
    // Make sure that product id input is int.
    $product['product_id'] = intval($product['product_id']);
    // Load commerce product.
    $commerce_product = commerce_product_load($product['product_id']);
    // Check whether commerce product is available.
    if ($commerce_product !== FALSE) {
      // Check input quantity: by default 1.
      $quantity = (isset($product['quantity']) && !empty($product['quantity'])) ? intval($product['quantity']) : 1;
      // Get all line item types.
      $types = commerce_line_item_types();
      // Check input type: by default product.
      $line_item_type = (isset($product['type']) && isset($types[$product['type']])) ? $product['type'] : 'product';
      // Create new line item based on product above.
      $line_item = commerce_product_line_item_new($commerce_product, $quantity, 0, array(), $line_item_type);
      // Get line item wrapper.
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
      // Check input fields.
      if (isset($product['fields']) && !empty($product['fields'])) {
        // Looping for each product field.
        foreach ((array) $product['fields'] as $field) {
          // Looping for each field.
          foreach ((array) $field as $field_name => $field_value) {
            try {
              $line_item_wrapper->$field_name->set($field_value);
            }
            catch (EntityMetadataWrapperException $exception) {
              // Log this condition.
              watchdog('commerce_cart_services', 'Set line item field with name @name.<br />Value:<pre>@value</pre> Trace:<pre>@trace</pre>', array(
                '@name' => $field_name,
                '@value' => print_r($field_value, TRUE),
                '@trace' => $exception->getTraceAsString(),
              ), WATCHDOG_ERROR, t('Cart: add'));
              // Return service cart product field error.
              return services_error(t('Invalid product fields'), 406);
            }
          }
        }
      }
      // Return line item object.
      try {
        $line_item = commerce_cart_product_add($user->uid, $line_item);
        if ($line_item !== FALSE) {
          $return["status"] = true;
          $return["data"] = order_services_line_item($line_item);
          return $return;
        }
        else {
          return services_error(t('Unable to add product'), 406);
        }
      } catch (Exception $e) {
        return services_error($e->getMessage(), 406);
      }
    }
    else {
      // Return service error: product not found.
      return services_error(t('Product not found'), 406);
    }
  }
  // Reach this means error: probably invalid product input.
  return services_error(t('Product is required'), 406);
}

/**
 * Function commerce_cart_services_resource_index().
 *
 * @return object
 *   Order object.
 */
function apps_services_v2_services_resource_index($source) {
  global $user;
  
  //set return container and default value of return
  $return = array();
  $return["status"] = false;
  $return["message"] = "";
  $return["data"] = array();

  // Get user's order.
  $order = commerce_cart_order_load($user->uid);
  // Check whether commerce cart can get user's shopping cart.
  if ($order !== FALSE && commerce_cart_order_is_cart($order)) {
    // Return order object which already language compatible.
    //set return container and default value of return
    $return["status"] = true;
    $return["data"] = $order;
    return $return;
  }

  // Reach this means error: unable to get user's shopping cart.
  return services_error(t('Cart missing'), 406);
}