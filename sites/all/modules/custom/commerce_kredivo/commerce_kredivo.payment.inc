<?php
/**
 * @file
 * commerce_kredivo.payment.inc
 *
 * @author andre4s_y
 *
 * Payment Settings:
 * * Order Prefix (order_prefix).
 * * Payment Server URL (server_url).
 * * Payment Server Key (server_key).
 * Payment Options:
 * * Ask customer phone number at checkout.
 *
 * @todo: check for all t function, need to add context.
 */

/**
 * Callback Functions.
 */

/**
 * Function commerce_kredivo_settings_form().
 *
 * Payment method callback: settings form.
 *
 * @param array $settings
 *   Setting form array.
 *
 * @return array
 *   Form array.
 */
function commerce_kredivo_settings_form($settings = array()) {
  $form = array();

  // Order ID Prefix: usefull for different multiple deployment site.
  $form['order_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Order ID Prefix'),
    '#default_value' => (isset($settings['order_prefix'])) ? $settings['order_prefix'] : '',
    '#size' => 5,
    '#maxlength' => 5,
    '#field_suffix' => ' - [ORDERID]',
    '#description' => t('System will send this prefix together with order id. Input only alphabet characters.'),
  );

  // Set example payment server url.
  $url = url('http://dev.kredivo/com/kredivo/v1', array(
    'absolute' => TRUE,
    'external' => TRUE,
  ));
  // Payment server URL (128).
  $form['server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server URL'),
    '#default_value' => (isset($settings['server_url'])) ? $settings['server_url'] : '',
    '#maxlength' => 128,
    '#description' => t('Please input full url: https:// or http://.<br />Example: @url.', array(
      '@url' => $url,
    )),
    '#required' => TRUE,
    '#attributes' => array(
      'placeholder' => $url,
    ),
  );

  // Payment server key (30).
  $form['server_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Server Key'),
    '#default_value' => (isset($settings['server_key'])) ? $settings['server_key'] : '',
    '#maxlength' => 50,
    '#required' => TRUE,
    '#attributes' => array(
      'placeholder' => t('Server Key'),
    ),
  );

  // Payment options.
  $form['options'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Options'),
  );
  // Ask for customer phone number at checkout?
  $form['options']['checkout_phone'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ask for customer phone number at checkout page.'),
    '#default_value' => (isset($settings['options']['checkout_phone'])) ? $settings['options']['checkout_phone'] : FALSE,
    '#description' => t('Customer phone number input is optional.'),
  );

  // Options for payment debug.
  $form['debug'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Debug Options'),
  );
  // Ask to verify server?
  $form['debug']['disable_verify_https'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable HTTPS verify peer.'),
    '#default_value' => (isset($settings['debug']['verify_https'])) ? $settings['debug']['verify_https'] : FALSE,
    '#description' => t('At development environment, HTTPS may be a problem.'),
  );
  $form['debug']['request_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('HTTP(s) Request Timeout'),
    '#default_value' => (isset($settings['debug']['request_timeout']) && !empty($settings['debug']['request_timeout'])) ? $settings['debug']['request_timeout'] : 30,
    '#size' => 3,
    '#maxlength' => 3,
    '#field_suffix' => t('s'),
    '#description' => t('By default 30s.'),
  );

  return $form;
}

/**
 * Function commerce_kredivo_submit_form().
 *
 * Payment method callback: submit form.
 *
 * @param array $payment_method
 *   Payment Method Info array.
 * @param array $pane_values
 *   Checkout payment pane values.
 * @param array|null $checkout_pane
 *   Checkout pane array info, if null callback from terminal.
 * @param object $order
 *   Order object.
 *
 * @return array
 *   Payment method form array.
 */
function commerce_kredivo_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form = array();

  // Check payment settings.
  if (isset($payment_method['settings']) && !empty($payment_method['settings'])) {
    // Check checkout pane variable: form from terminal.
    if (empty($checkout_pane)) {
      // @todo: check whether terminal is possible.
      // Return form array.
      return $form;
    }

    // Create kredivo pane values tree.
    $form['kredivo'] = array(
      '#tree' => TRUE,
    );

    // Check whether need to ask customer phone number.
    if (isset($payment_method['settings']['options']['checkout_phone'])
      && !empty($payment_method['settings']['options']['checkout_phone'])
    ) {
      $form['kredivo']['phone'] = array(
        '#type' => 'textfield',
        '#title' => t('Phone Number'),
        '#default_value' => isset($order->data['customer_phone']) ? $order->data['customer_phone'] : '',
        '#attributes' => array('autocomplete' => 'off'),
        '#required' => TRUE,
        '#validated' => TRUE,
        '#maxlength' => 14,
        '#size' => 14,
        '#description' => t('Input your phone number.'),
      );
    }
  }
  else {
    // Show payment is not configured for user.
    drupal_set_message(t('@payment is not configured for use. Please contact an administrator to resolve this issue.', array(
      '@payment' => ucfirst($payment_method['title']),
    )), 'error');
    $form['error'] = array(
      '#markup' => theme('status_messages', array('display' => 'error')),
    );
  }

  return $form;
}

/**
 * Function commerce_kredivo_submit_form_validate().
 *
 * Payment method callback: submit form validation.
 *
 * @param array $payment_method
 *   Payment Method Info array.
 * @param array $pane_form
 *   Checkout pane form array.
 * @param array $pane_values
 *   Checkout pane form values.
 * @param object $order
 *   Order object.
 * @param array $form_parents
 *   The identifier of the base element of the payment pane.
 *
 * @return bool
 *   FALSE will show correct form error.
 */
function commerce_kredivo_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  // Define form element name prefix: to provide valid form element name.
  $prefix = implode('][', array_merge($form_parents, array('kredivo')));

  // Validate payment settings.
  if (!(isset($payment_method['settings']) && !empty($payment_method['settings'])
    && isset($payment_method['settings']['server_url']) && !empty($payment_method['settings']['server_url'])
    && isset($payment_method['settings']['server_key']) && !empty($payment_method['settings']['server_key'])
    && valid_url($payment_method['settings']['server_url'], TRUE)
  )) {
    drupal_set_message(t('@payment is not configured for use. Please contact an administrator to resolve this issue.', array(
      '@payment' => ucfirst($payment_method['title']),
    )), 'error');
    return FALSE;
  }

  // Define order wrapper: to get total order amount.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  // Get order line item.
  $line_items = $order_wrapper->commerce_line_items->value();
  // Check order must have value.
  // In case not checked at payment rules condition.
  if (empty($line_items)
    || $order_wrapper->commerce_order_total->amount->value() <= 0
  ) {
    drupal_set_message(t('@payment can not continue: cart can not empty or payment amount can not 0.', array(
      '@payment' => ucfirst($payment_method['title']),
    )), 'error');
    return FALSE;
  }

  // Validate customer phone.
  if (isset($pane_values['kredivo']['phone']) && !empty($pane_values['kredivo']['phone'])) {
    if (!preg_match('#^[\+|0]?\d+$#', $pane_values['kredivo']['phone'])) {
      form_set_error($prefix . '][phone', t('@payment can not continue: please input valid phone number.', array(
        '@payment' => ucfirst($payment_method['title']),
      )));
      return FALSE;
    }
  }

  // Other condition: return TRUE.
  return TRUE;
}

/**
 * Function commerce_kredivo_submit_form_submit().
 *
 * Payment method callback: submit form submission.
 *
 * @param array $payment_method
 *   Payment Method Info array.
 * @param array $pane_form
 *   Checkout pane form array.
 * @param array $pane_values
 *   Checkout pane form values.
 * @param object $order
 *   Order object.
 * @param array $charge
 *   Amount and currency array (if available).
 *
 * @return bool
 *   FALSE will rebuild the payment checkout form.
 */
function commerce_kredivo_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // Update the order to reference the payment method.
  // @see commerce_payment_redirect_pane_checkout_form().
  $order->data['payment_method'] = $payment_method['instance_id'];
  // Always regenerate payment redirect key.
  $order->data['payment_redirect_key'] = drupal_hash_base64(time());

  // Save customer phone.
  if (isset($pane_values['kredivo']['phone']) && !empty($pane_values['kredivo']['phone'])) {
    $order->data['customer_phone'] = $pane_values['kredivo']['phone'];
  }

  // Get order wrapper.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  // Define order total.
  $order_total = (int) $order_wrapper->commerce_order_total->amount->value();

  // Generate item string: Nama_Item,harga_satuan,Qty,SubTotal;
  $items = array();
  // Get order line items.
  $line_items = $order_wrapper->commerce_line_items->value();
  // Looping for each line item.
  foreach ($line_items as $line_item) {
    // Get line item wrapper.
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    // Get product price.
    $unit_price = $line_item_wrapper->commerce_unit_price->value();
    // Check currency: if not IDR.
    if ($unit_price['currency_code'] != 'IDR') {
      $unit_price['amount'] = commerce_currency_convert($unit_price['amount'], $unit_price['currency_code'], 'IDR');
    }
    // Get item name: by default is line item label (SKU).
    $item_name = commerce_line_item_title($line_item);
    $item_name = (empty($item_name)) ? $line_item->line_item_label : $item_name;
    // Define item.
    $item = array(
      'product_sku' => $line_item->line_item_label,
      'product_name' => $item_name,
      'product_price' => (int) $unit_price['amount'],
      'quantity' => (int) $line_item->quantity,
    );
    // Store it inside items.
    $items[] = $item;
  }

  // Create new payment transaction.
  $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  // Zero-ed remote id: still have no id yet.
  $transaction->remote_id = 0;
  $transaction->remote_status = COMMERCE_PAYMENT_STATUS_PENDING;
  $transaction->amount = $order_total;
  $transaction->currency_code = 'IDR';
  $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
  $transaction->message = 'Checkout';
  commerce_payment_transaction_save($transaction);

  // Request checkout URL here: before redirect to checkout url.
  // Define request data.
  $data = array(
    'server_key' => $payment_method['settings']['server_key'],
    'order_id' => preg_replace('#[^a-zA-Z]#', '', $payment_method['settings']['order_prefix']) . '-' . $order->order_id,
    'user_email' => $order->mail,
    'user_phone' => isset($order->data['customer_phone']) ? $order->data['customer_phone'] : '',
    'amount' => $order_total,
    'items' => $items,
    'push_uri' => url('commerce_kredivo/notification/' . $order->order_id . '/' . $transaction->transaction_id, array('absolute' => TRUE)),
    'back_to_store_uri' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
  );
  // Define stream context: for https only.
  $stream_context = array();
  if (isset($payment_method['settings']['debug']['verify_https'])
    && !empty($payment_method['settings']['debug']['verify_https'])
    && strpos($payment_method['settings']['server_url'], 'https') === 0
  ) {
    $stream_context = stream_context_create(array(
      'ssl' => array(
        'verify_peer' => FALSE,
        'verify_host' => FALSE,
      ),
    ));
  }
  // Build request option.
  $request_option = array(
    'headers' => array(
      'Content-Type' => 'application/json',
      'Accept' => 'application/json',
    ),
    'method' => 'POST',
    'max_redirects' => 0,
    'timeout' => (isset($payment_method['settings']['debug']['request_timeout']) && !empty($payment_method['settings']['debug']['request_timeout'])) ? (float) $payment_method['settings']['debug']['request_timeout'] : 30.0,
    'data' => (!empty($data)) ? json_encode($data) : '',
    'context' => $stream_context,
  );
  // Sanitize $payment_method['settings']['server_url'].
  $payment_method['settings']['server_url'] = preg_replace('#\/$#', '', $payment_method['settings']['server_url']);
  // Do http request for checkout url.
  $result = drupal_http_request($payment_method['settings']['server_url'] . '/get_checkout_url', $request_option);

  // Log this result.
  watchdog('commerce_kredivo', 'Kredivo request checkout URL result for debug: <pre>@result</pre>', array(
    '@result' => print_r($result, TRUE),
  ), WATCHDOG_DEBUG, l(t('View Order @oid Revisions', array(
    '@oid' => $order->order_id,
  )), 'admin/commerce/orders/' . $order->order_id . '/revisions'));

  // Define checkout message.
  $checkout_message = '';
  // Check the result: suppose to be json string.
  // 1. Must have 200 http code.
  // 2. Must have data.
  if (isset($result->code) && $result->code == 200
    && isset($result->data) && !empty($result->data)
  ) {
    // Decode the string.
    $kredivo = json_decode($result->data);
    // Validate kredivo checkout data.
    // Kredivo: status => 'success'.
    if (!empty($kredivo)
      && isset($kredivo->status) && $kredivo->status == 'success'
      && isset($kredivo->code) && $kredivo->code == 200
      && isset($kredivo->redirect_url) && valid_url($kredivo->redirect_url, TRUE)
    ) {
      // Check if there is message returned from kredivo.
      if (isset($kredivo->message)) {
        $checkout_message = $kredivo->message;
      }
      // Set checkout url inside order data.
      $order->data['checkout_url'] = $kredivo->redirect_url;
      // Return true: continue to redirect form.
      return TRUE;
    }
  }

  // Show message to customer.
  if (!empty($checkout_message)) {
    // Redirect back to payment selection with checkout message.
    drupal_set_message(t('@payment currently unavailable: @reason.', array(
      '@payment' => ucfirst($payment_method['title']),
      '@reason' => $checkout_message,
    )), 'error');
  }
  else {
    // Other condition will result to redirect back to payment selection.
    drupal_set_message(t('@payment currently unavailable.', array(
      '@payment' => ucfirst($payment_method['title']),
    )), 'error');
  }

  return FALSE;
}

/**
 * Function commerce_kredivo_redirect_form().
 *
 * Payment method callback: redirect form.
 *
 * @param array $form
 *   Redirect form array: auto generated from drupal_get_form().
 * @param array $form_state
 *   Redirect form state array: auto generated from drupal_get_form().
 * @param object $order
 *   Order object.
 * @param array $payment_method
 *   Payment Method Info array.
 *
 * @return array
 *   Form array.
 */
function commerce_kredivo_redirect_form($form, &$form_state, $order, $payment_method) {
  // Check if checkout redirect target url is available.
  if (isset($order->data['checkout_url'])) {
    // Set checkout redirect target url.
    $form['#action'] = $order->data['checkout_url'];
    // Return the form.
    return $form;
  }

  // Target url for checkout is not available.
  // Log this condition: customer should not come to this section.
  watchdog('commerce_kredivo', 'Order @oid redirect to checkout URL failed: missing checkout URL. <pre>Order: @order</pre><pre>Payment Method: @pm</pre>', array(
    '@oid' => $order->order_id,
    '@order' => print_r($order, TRUE),
    '@pm' => print_r($payment_method, TRUE),
  ), WATCHDOG_NOTICE, l(t('View Order @oid Revisions', array(
    '@oid' => $order->order_id,
  )), 'admin/commerce/orders/' . $order->order_id . '/revisions'));
  // Show an error message to customer.
  drupal_set_message(t('Redirect to @payment Checkout failed. Please try again or contact an administrator to resolve the issue.', array(
    '@payment' => ucfirst($payment_method['title']),
  )), 'error');
  // Go back a page and log to order revision and history.
  commerce_payment_redirect_pane_previous_page($order, t('Redirect to @payment Checkout failed.', array(
    '@payment' => ucfirst($payment_method['title']),
  )));
}

/**
 * Function commerce_kredivo_redirect_form_back().
 *
 * Payment method callback: redirect form back callback.
 *
 * @param object $order
 *   Order object.
 * @param array $payment_method
 *   Payment Method Info array.
 */
function commerce_kredivo_redirect_form_back($order, $payment_method) {
  // There is no cancel uri at kredivo, but need to handle this.
  // Display a message indicating the customer initiatied cancellation.
  drupal_set_message(t('You have canceled checkout using @payment.', array(
    '@payment' => ucfirst($payment_method['title']),
  )));

  // Remove the payment information from the order data array.
  unset($order->data['payment_method']);
  unset($order->data['checkout_url']);

  // Send them back to the shopping cart page instead of the previous page in
  // the checkout process.
  commerce_order_status_update($order, 'cart', FALSE, NULL, t('Customer canceled using @payment.', array(
    '@payment' => ucfirst($payment_method['title']),
  )));
  drupal_goto('cart');
}

/**
 * Function commerce_kredivo_redirect_form_validate().
 *
 * Payment method callback: redirect form return validation.
 *
 * @param object $order
 *   Order object.
 * @param array $payment_method
 *   Payment Method Info array.
 *
 * @return bool
 *   Customer should proceed to checkout completion or go back.
 */
function commerce_kredivo_redirect_form_validate($order, $payment_method) {
  // When reach this page, checkout at Kredivo has finished, but with no status.
  // Just allow the customer to reach next step.
  return TRUE;
}

/**
 * Function commerce_kredivo_redirect_form_submit().
 *
 * Payment method callback: redirect form return submission.
 *
 * @param object $order
 *   Order object.
 * @param array $payment_method
 *   Payment Method Info array.
 */
function commerce_kredivo_redirect_form_submit($order, $payment_method) {
  $input = array();
  // Check input: GET.
  if (isset($_GET) && !empty($_GET)) {
    $input['get'] = $_GET;
  }
  // Check input: POST.
  $input_post = file_get_contents("php://input");
  if (!empty($input_post)) {
    $input['post'] = $input_post;
  }

  // Log this condition.
  watchdog('commerce_kredivo', 'Customer order @oid returned from Kredivo. <pre>Input: @input</pre><pre>Order: @order</pre><pre>Payment Method: @pm</pre>', array(
    '@oid' => $order->order_id,
    '@input' => print_r($input, TRUE),
    '@order' => print_r($order, TRUE),
    '@pm' => print_r($payment_method, TRUE),
  ), WATCHDOG_INFO, l(t('View Order @oid Revisions', array(
    '@oid' => $order->order_id,
  )), 'admin/commerce/orders/' . $order->order_id . '/revisions'));

  // Check whether order has paid.
  if (!(isset($order->data['commerce_payment_order_paid_in_full_invoked'])
    && !empty($order->data['commerce_payment_order_paid_in_full_invoked'])
  )) {
    // Notify customer that kredivo is processing the order.
    drupal_set_message(t('Your payment request has been submitted.'), 'warning');
  }
}
