<?php

/**
 * Function apps_services_v2_services_sepulsa_operator_list().
 * get operator data from server
 *
 * Resources callback for sepulsa resource index.
 */
function apps_services_v2_services_sepulsa_operator_list($source) {
  // Define service friendly return value.
  $return = array();
  
  //set default return value
  $return["status"] = false;
  $return["message"] = "Data Operator tidak ditemukan";
  $return["data"] = null;
  
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
 * get product data from server (defined by type, example electricity)
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
  $return["data"] = null;

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
 * add product to cart
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
  $return["data"] = null;

  // Backward compatibilty for old electricity product.
  if (module_exists('pln_prepaid') && $product['type'] == 'electricity' || $product['type'] == 'electricity_prepaid') {
    $product['type'] = 'pln_prepaid';
    $product['fields']['field_customer_number'] = $product['fields']['electricity_customer_number'];
    $product['fields']['field_phone_number'] = $product['fields']['electricity_phone_number'];
    unset($product['fields']['electricity_customer_number']);
    unset($product['fields']['electricity_phone_number']);
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
        // Looping for each field.
        foreach ((array) $product['fields'] as $field_name => $field_value) {
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
      // Return line item object.
      try {
        $line_item = commerce_cart_product_add($user->uid, $line_item);
        // Get the order.
	$order = commerce_cart_order_load($user->uid);
        // Refresh it: so cart index will get new order data.
        commerce_cart_order_refresh($order);
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
 * Function apps_services_v2_services_resource_add_multiple().
 * add more than one product to cart
 *
 * @param array $product
 *   Product data.
 *
 * @return mixed
 *   Line item object, or string contain error message otherwise.
 */
function apps_services_v2_services_resource_add_multiple($products, $source) {
  global $user;
  
  //set return container and default value of return
  $return = array();
  $return["status"] = false;
  $return["message"] = "Mohon masukkan produk";
  $return["data"] = null;
  $temporary_data = null;
  
  foreach ($products as $product) {
    // Backward compatibilty for old electricity product.
    if (module_exists('pln_prepaid') && $product['type'] == 'electricity' || $product['type'] == 'electricity_prepaid') {
      $product['type'] = 'pln_prepaid';
      $product['fields']['field_customer_number'] = $product['fields']['electricity_customer_number'];
      $product['fields']['field_phone_number'] = $product['fields']['electricity_phone_number'];
      unset($product['fields']['electricity_customer_number']);
      unset($product['fields']['electricity_phone_number']);
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
          // Looping for each field.
          foreach ((array) $product['fields'] as $field_name => $field_value) {
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
        // Return line item object.
        try {
          $line_item = commerce_cart_product_add($user->uid, $line_item);
          if ($line_item !== FALSE) {
            $return["status"] = true;
            $return["message"] = "";
            $temporary_data = $line_item;
          }
          else {
            $return["status"] = false;
            $return["message"] = "Gagal menambahkan produk";
            return $return;
          }
        } catch (Exception $e) {
          $return["status"] = false;
          $return["message"] = $e->getMessage();
          return $return;
        }
      }
      else {
        // Return service error: product not found.
        $return["status"] = false;
        $return["message"] = "Produk tidak diketemukan";
        return $return;
      }
    }
  }
  
  if(!empty($temporary_data)){
    $return["data"] = order_services_line_item($temporary_data);
  }
  return $return;
}

/**
 * Function commerce_cart_services_resource_index().
 * get current cart data
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
    $order = apps_services_v2_services_format_order($order);
    
    //set return container and default value of return
    $return["status"] = true;
    $return["data"] = $order;
    return $return;
  }

  // Reach this means error: unable to get user's shopping cart.
  return services_error(t('Cart missing'), 406);
}

/**
 * Function apps_services_v2_services_format_order().
 * add some addtional data to order
 *
 * @return object
 *   Order object.
 */
function apps_services_v2_services_format_order($order) {
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
  if (isset($order->commerce_line_items) && !empty($order->commerce_line_items)) {
    foreach ($order->commerce_line_items as $lid => $line_item) {
      if (isset($line_item->type) && $line_item->type == 'pln_prepaid') {
        $order->commerce_line_items[$lid]->type = 'electricity_prepaid';
        if (isset($order->commerce_line_items[$lid]->commerce_product->type)) {
          $order->commerce_line_items[$lid]->commerce_product->type = 'electricity_prepaid';
        }
      }
    }
  }
  
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
  
  return $order;
}

/**
 * Function apps_services_v2_services_resource_coupon_redeem().
 * redeem promo code sepulsa on checkout
 *
 * @return object
 *   Order object.
 */
function apps_services_v2_services_resource_coupon_redeem($code, $source) {
  //set return container and default value of return
  $return = array();
  $return["status"] = false;
  $return["message"] = "";
  $return["data"] = null;
  
  $error = '';
  $uid = $GLOBALS['user']->uid;
  $order = commerce_cart_order_load($uid);
  $coupon = commerce_coupon_redeem_coupon_code($code, $order, $error);

  if ($coupon) {
    $order = commerce_order_load($order->order_id);
    commerce_cart_order_refresh($order);
  }

  if (!$error) {
    // If a coupon was invalidated during the cart refresh (e.g. if its
    // discounts failed their conditions), an error message will have been
    // set.
    $error = &drupal_static('commerce_coupon_error_' . strtolower($code));
  }

  if (!$error) {
    // Allow modules/rules to act when a coupon has been successfully added
    // to the cart.
    rules_invoke_all('commerce_coupon_applied_to_cart', $coupon, $order);
    $return_data = array(
      'coupon' => $coupon,
      'offer' => entity_metadata_wrapper('commerce_coupon', $coupon)->commerce_discount_reference->get(0)->commerce_discount_offer->value(),
    );
    
    $return["status"] = true;
    $return["data"] = $return_data;
    return $return_data;
  }
  else {
    return services_error($error);
  }
}

/*
 * remove promo code from user's order
 */
function apps_services_v2_services_resource_coupon_remove($coupon_id, $source) {
  $uid = $GLOBALS['user']->uid;
  $order = commerce_cart_order_load($uid);
  $coupon = commerce_coupon_load($coupon_id);

  commerce_coupon_remove_coupon_from_order($order, $coupon);
  commerce_cart_order_refresh($order);
  
  //set return container and default value of return
  $return = array();
  $return["status"] = true;
  $return["message"] = "Kode discount telah dihapus";
  $return["data"] = array();

  return $return;
}

/**
 * Function apps_services_v2_services_resource_complete().
 * get complete checkout message from service
 *
 * @param int $order_id
 *   Order id.
 *
 * @return mixed
 *   TRUE if success, error message otherwise.
 */
function apps_services_v2_services_resource_complete($order_id, $source) {
  //set return container and default value of return
  $return = array();
  $return["status"] = false;
  $return["message"] = "";
  $return["data"] = null;
  
  $return_value = commerce_checkout_services_resource_complete($order_id);
  
  if(isset($return_value["message"]) && !empty($return_value["message"])){
    $description = array();
    $description["description"] = $return_value[];

    $return["status"] = true;
    $return["data"] = $description;

    return $return;
  }
  
  // Reach this means error: invalid order.
  return services_error(t('Invalid order'), 406);
}

/*
 * Get token to get saved card from veritrans
 */
function apps_services_v2_services_get_saved_card() {
  //set return container and default value of return
  $return = array();
  $return["status"] = true;
  $return["message"] = "";
  $return["data"] = commerce_veritrans_token_get($GLOBALS['user']->uid);
  
  $count_token = count($return["data"]);
  if($count_token < 1){
    //if empty token then return empty array
    $return["status"] = false;
    $return["data"] = null;
  }
  
  return $return;
}
