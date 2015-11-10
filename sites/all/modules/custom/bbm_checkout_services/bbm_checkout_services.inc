<?php

function bbm_checkout_order_services($data){
    $return = array();
    
    $return["sepulsa_order_id"] = 0;
    $return["status"] = "gagal";
    $return["keterangan"] = "";
    
    //set ip filter
    $whitelist = explode(",",variable_get("mos_ttl_whitelist", "127.0.0.1,::1"));
    $ip_user = (isset($_SERVER['HTTP_X_FORWARDED_FOR']) ? $_SERVER['HTTP_X_FORWARDED_FOR'] : $_SERVER['REMOTE_ADDR']);
    if(!in_array($ip_user, $whitelist)){
        $return["keterangan"] = "[" . date('Y-m-d H:i:s') . "] ip error. Ip address diluar akses yang diberikan, ip address: " . $ip_user;
        if(variable_get('bbm_checkout_services_whitelist_enable_watchdog', 1)){
            watchdog('BBM Checkout', 'BBM checkout api is ran from ip address outside white list, ip address: <pre>%ip</pre>' , array('%ip' => print_r($_SERVER, TRUE)), WATCHDOG_CRITICAL, $link = NULL);
        }
        return $return;
    }
    
    //set veritrans signature filter
    if(variable_get('bbm_checkout_services_check_signature', 1)){
        if(!isset($data["signature_key"]) || empty($data["signature_key"])){
            $return["keterangan"] = "Signature anda tidak ada dalam parameter";
            return $return;
        }
        else{
            $hash = hash('sha512', $data["order_id"] . $data["status_code"] . $data["gross_amount"] . variable_get("bbm_checkout_services_server_key", "VT-server-XS4LpMKEMvrl8NbKMx2hdWUe"));
            if($data["signature_key"] != $hash){
                $return["keterangan"] = "Signature anda tidak valid";
                return $return;
            }
        }
    }
    
    //set/split parameter from veritrans
    $data["container_product_id_and_email"] = explode(",", $data["product_id_and_email"]);
    $data["email"] = $data["container_product_id_and_email"][1];
    $product_container = array();
    if(isset($data["container_product_id_and_email"][0]) && !empty($data["container_product_id_and_email"][0] && isset($data["phone_number"]) && !empty($data["phone_number"]))){
        $product_container[0]["product_id"] = $data["container_product_id_and_email"][0];
        $product_container[0]["field_phone_number"] = $data["phone_number"];
        $product_container[0]['quantity'] = 1;
        $product_container[0]['type'] = "mobile_prepaid";
    }
    
    //set coupon data list from veritrans
    $data["container_coupon"] = explode(",", $data["coupon_id"]);
    $i = 1;
    foreach ($data["container_coupon"] as $value) {
        $product_container[$i]["product_id"] = $value;
        $product_container[$i]['quantity'] = 1;
        $product_container[$i]['type'] = "coupon";
        $i++;
    }
    
    //set empty email restriction
    if(!isset($data["email"]) || empty($data["email"])){
        $return["keterangan"] = "Mohon untuk memberikan data email";
        return $return;
    }
    
    //set product restriction
    if(!isset($product_container[0]) || empty($product_container[0]) || count($product_container[0]) < 1){
        $return["keterangan"] = "Produk pulsa tidak ditemukan";
        return $return;
    }
    
    //create user if email is not registered
    $account = user_load_by_mail($data["email"]);
    if(!$account){
        $username["username"] = explode("@", $data["email"]);
        $username["username"] = $username["username"][0];
        $fields = array(
          'name' => _hybridauth_make_username($username),
          'mail' => $data["email"],
          'init' => $data["email"],
          'pass' => user_password(variable_get('commerce_password_length', 8)),
          'roles' => array(),
          'status' => 1,
        );
        $account = user_save('', $fields);
    }
    
    /*$order = commerce_cart_order_load($account->uid);
    if ($order !== FALSE && commerce_cart_order_is_cart($order)) {
        $order = commerce_cart_order_empty($order);
    }*/
    $order = commerce_order_new($account->uid, 'cart');
    $order = commerce_cart_order_empty($order);
    
    foreach ($product_container as $value){
        $product = array();
        $product['product_id'] = $value["product_id"];
        $product['quantity'] = $value["quantity"];
        $product['type'] = $value["type"];
        if(isset($value["field_phone_number"]) && !empty($value["field_phone_number"])){
            $product['fields'][0]["field_phone_number"] = $value["field_phone_number"];
        }

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
                $line_item = commerce_cart_product_add($account->uid, $line_item);
                if (!$line_item && $product['type'] == "mobile_prepaid") {
                    $return["keterangan"] = "Gagal menambah produk pulsa";
                    return $return;
                }
            }
            else {
                if ($product['type'] == "mobile_prepaid") {
                    $return["keterangan"] = "Produk tidak ditemukan";
                    return $return;
                }
            }
        }
    }
    
    $order = commerce_order_load($order->order_id);
    
    if($order){
        //$order = commerce_order_status_update($order, 'checkout_payment', FALSE, NULL);
        $order = commerce_order_status_update($order, 'processing', FALSE, NULL);
        //commerce_checkout_complete($order);
        //tag that this order is made from BBM Checkout
        $order->field_order_source[LANGUAGE_NONE][0]['value'] = 'bbm_checkout';
        $order->data['payment_method'] = 'commerce_veritrans|commerce_payment_commerce_veritrans';
        
        $payment_method = commerce_payment_method_instance_load('commerce_veritrans|commerce_payment_commerce_veritrans');
        
        foreach (commerce_payment_transaction_load_multiple(array(), array('order_id' => $order->order_id)) as $transaction) {
          commerce_payment_transaction_delete($transaction->transaction_id);
        }
        
        $transaction = commerce_payment_transaction_new('commerce_veritrans', $order->order_id);
        $transaction->instance_id = $payment_method['instance_id'];
        $transaction->amount = $data['gross_amount'];
        $transaction->currency_code = 'IDR';
        $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;

        commerce_payment_transaction_save($transaction);

        commerce_payment_commerce_payment_transaction_insert($transaction);
        
        commerce_order_save($order);
        
        $return["sepulsa_order_id"] = $order->order_id;
        $return["status"] = "sukses";
        $return["keterangan"] = "";
        $order->return = $return;
        
        $order = commerce_order_load($order->order_id);
        
        return $order;
    }
    else{
        $return["keterangan"] = "Order anda tidak tercatat";
        return $return;
    }
    
    /*$order = commerce_order_new($account->uid, 'cart');
    commerce_order_save($order);

    // line item
    $product = commerce_product_load(12);
    $line_item = commerce_product_line_item_new($product, 1, $order->order_id, array(), 'mobile_prepaid');
    commerce_line_item_save($line_item);

    // wrap the order
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $order_wrapper->commerce_line_items[] = $line_item;
    
    $payment_method = commerce_payment_method_instance_load('commerce_veritrans|commerce_payment_commerce_veritrans');
        
        $transaction = commerce_payment_transaction_new('commerce_veritrans', $order->order_id);
        $transaction->instance_id = $payment_method['instance_id'];
        $transaction->amount = $data['gross_amount'];
        $transaction->currency_code = 'IDR';
        $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;

        commerce_payment_transaction_save($transaction);

        commerce_payment_commerce_payment_transaction_insert($transaction);

    // save the order
    commerce_order_save($order);
    
    return $order;*/
}