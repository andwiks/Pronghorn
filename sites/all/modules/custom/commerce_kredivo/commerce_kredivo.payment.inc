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
 * @todo: check discount line item name.
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
  // Debug mode?
  $form['debug']['debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug Mode'),
    '#default_value' => (isset($settings['debug']['debug_mode'])) ? $settings['debug']['debug_mode'] : FALSE,
    '#description' => t('Produce more log for debug.'),
  );
  // Ask to verify server?
  $form['debug']['disable_verify_https'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable HTTPS verify peer.'),
    '#default_value' => (isset($settings['debug']['disable_verify_https'])) ? $settings['debug']['disable_verify_https'] : FALSE,
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

    // Give kredivo description below payment method option.
    // @todo: create settingable.
    $sepulsa_kredivo_desc = '

<span style="font-size: 14px; font-weight: bolder;"><strong>Beli sekarang, bayar nanti dengan Kredivo</strong></span><br /><br />
<p>
Khusus pengguna <strong>Android</strong> dengan penghasilan <strong>minimal 3 juta perbulan</strong>, Anda sekarang dapat membeli produk di Sepulsa dan <strong>membayar dalam 30 hari, tanpa biaya tambahan dengan Kredivo</strong>. Ambil kesempatan ini segera dan mulai berbelanja sekarang!
</p>
<p>
Berikut cara pembayaran dengan Kredivo:
<ol style="list-style: decimal; margin-left: 13px; line-height: 1.5em;">
<li>Anda akan di arahkan ke halaman Checkout Kredivo dengan rincian pembelian Anda di Sepulsa.com.</li>
<li>Jika nominal transaksi telah sesuai silahkan Login untuk melanjutkan transaksi.</li>
<li>Login dengan cara masukkan <strong>No.Handphone yang terdaftar</strong> pada kolom yang tersedia.</li>
<li>Masukkan PIN Anda pada kolom yang tersedia, lalu klik tombol &quot;<strong>Masuk</strong>&quot;.</li>
<li>Sistem akan mengirimkan kode OTP ke <strong>No.Handphone yang terdaftar</strong>, lalu masukkan kedalam kolom yang tersedia, lalu klik tombol &quot;<strong>Confirm OTP</strong>&quot;.</li>
<li>Transaksi selesai, cek kembali status transaksi Anda di akun Sepulsa pada halaman &quot;<strong>Order Transaksi</strong>&quot;.</li>
</ol>
<br /><br />
Cara mendaftar akun Krevido, ' . l(t('klik di sini'), 'metode-pembayaran-kredivo', array('attributes' => array('target'=>'_blank'))) . '.
</p>
';

    // Give kredivo description.
    $form['kredivo']['desc'] = array(
      '#markup' => $sepulsa_kredivo_desc,
    );
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

  // Check previous payment pending if any.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'commerce_payment_transaction')
    ->propertyCondition('order_id', $order->order_id, '=')
    ->propertyOrderBy('revision_id', 'DESC')
    ->propertyCondition('remote_id', '0')
    ->propertyCondition('remote_status', COMMERCE_PAYMENT_STATUS_PENDING)
    ->propertyCondition('amount', $order_total, '=')
    ->range(0, 1);
  $result = $query->execute();
  if (isset($result['commerce_payment_transaction']) && !empty($result['commerce_payment_transaction'])) {
    // Load previous payment transaction.
    $transaction = commerce_payment_transaction_load(key($result['commerce_payment_transaction']));
  }
  else {
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
  }
  // Request checkout URL here: before redirect to checkout url.
  // Define kredivo order id.
  if (isset($payment_method['settings']['order_prefix']) && !empty($payment_method['settings']['order_prefix'])) {
    $kredivo_order_id = preg_replace('#[^a-zA-Z]#', '', $payment_method['settings']['order_prefix']) . '-' . $order->order_id;
  }
  else {
    $kredivo_order_id = $order->order_id;
  }
  // Define request data.
  $data = array(
    'server_key' => $payment_method['settings']['server_key'],
    'order_id' => $kredivo_order_id,
    'user_email' => $order->mail,
    'user_phone' => isset($order->data['customer_phone']) ? $order->data['customer_phone'] : '',
    'amount' => $order_total,
    'items' => $items,
    'push_uri' => url('commerce_kredivo/notification/' . $order->order_id . '/' . $transaction->transaction_id, array('absolute' => TRUE)),
    'user_cancel_uri' => url('checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
    'back_to_store_uri' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
  );
  // Encode the data with json.
  $data = json_encode($data);

  // Build request option.
  $request_option = $payment_method['settings'];
  $request_option['method'] = 'POST';
  $request_option['url'] = 'get_checkout_url';

  // Do http request for checkout url.
  $result = commerce_kredivo_api($data, $request_option);

  // Define checkout message.
  $checkout_message = '';
  // Check the result: suppose to be json string.
  if (isset($result['result']) && !empty($result['result'])) {
    // Decode the string.
    $kredivo = json_decode($result['result']);
    // Validate kredivo checkout data.
    // Kredivo: status => 'success'.
    if (!empty($kredivo)
      && isset($kredivo->status) && $kredivo->status == 'OK'
      && isset($kredivo->code) && $kredivo->code == 200
      && isset($kredivo->redirect_url) && valid_url($kredivo->redirect_url, TRUE)
    ) {
      // Check if there is message returned from kredivo.
      if (isset($kredivo->message)) {
        $checkout_message = $kredivo->message;
      }
      // Set checkout url inside order data.
      $order->data['checkout_url'] = $kredivo->redirect_url;
      // Set transaction id inside order data.
      $order->data['transaction_id'] = $transaction->transaction_id;
      // Return true: continue to redirect form.
      return TRUE;
    }
  }

  // Check for result error.
  if (empty($checkout_message) && !empty($result['error'])) {
    $checkout_message = $result['error'];
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
    $sepulsa_redirect_description = '
<div style="margin: 15px auto; text-align: justify; width: 600px; line-height: 1.5em; font-size: 14px;">
<div>
<span style="font-size: 16px;"><strong>Pastikan telah memiliki Akun Kredivo yang bisa dipakai untuk transaksi</strong></span>
<ul style="list-style: square; margin-left: 13px;">
<li>Silahkan klik Tombol &quot;<strong>OK, Lanjutkan</strong>&quot; jika Anda ingin melanjutkan transaksi dengan menggunakan Kredivo.</li>
<li>Atau klik Tombol &quot;<strong>Cancel</strong>&quot;, untuk kembali ke halaman &quot;Metode Pembayaran&quot; untuk memilih opsi pembayaran lainnya.</ul>
</ul>
</div>
<div style="margin-top: 10px;">
Kredivo menawarkan angsuran pembiayaan yang transparan dan sederhana untuk pembelian online Anda hanya dalam beberapa klik:
<ol style="list-style: decimal; margin-left: 13px;">
<li>Pilih opsi pembayaran Kredivo pada halaman checkout merchant dan login ke akun Anda. Tinjau rincian transaksi dan konfirmasi pembelian. Selesai!</li>
<li>Jika Anda bukan pelanggan Kredivo, download aplikasi dan luangkan beberapa menit untuk melalui proses persetujuan. Dibutuhkan tidak lebih dari 2‐3 menit! Ketika Anda disetujui, Anda perlu untuk meng‐upload beberapa dokumen dasar KYC dan kami akan mengaktifkan akun Anda dalam waktu tidak lebih dari 24 jam dari penerimaan dokumen, setelah itu Anda dapat menggunakan Kredivo segera.</li>
</ol>
</div>
<div style="margin-top: 10px;">
<strong>Apa yang diperlukan untuk memiliki akun Kredivo?</strong>
<ol style="list-style: decimal; margin-left: 13px;">
<li>Warga Negara Indonesia</li>
<li>Berusia minimal 18 tahun</li>
<li>Memiliki rekening bank</li>
<li>Memiliki penghasilan minimal Rp 3 juta per bulan (baik karyawan maupun wiraswasta)</li>
<li>Memberikan nomor ponsel Indonesia yang valid</li>
<li>Memberikan nama lengkap Anda, tanggal lahir, informasi basic akun Facebook, dan nomor KTP Anda untuk membantu kami memverifikasi identitas Anda.</li>
</ol>
</div>
</div>
';
    // Add redirect description.
    $form['description'] = array(
      '#markup' => $sepulsa_redirect_description,
    );
    // Add submit button.
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('OK, Lanjutkan'),
      '#weight' => 10,
    );
    global $base_url;
    // Add cancel button.
    $form['cancel'] = array(
      '#prefix' => '&nbsp;',
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#weight' => 11,
      '#attributes' => array(
        'onclick' => array('window.location.href = "' . base_path() . 'checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'] . '"; return false;'),
      ),
    );
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
  unset($order->data['payment_redirect_key']);
  unset($order->data['checkout_url']);

  // Remove payment transaction: if pending.
  if (isset($order->data['transaction_id']) && !empty($order->data['transaction_id'])) {
    // Load transaction.
    $transaction = commerce_payment_transaction_load($order->data['transaction_id']);
    // Check transaction data.
    if ($transaction->status == COMMERCE_PAYMENT_STATUS_PENDING
      && $transaction->remote_id == 0
      && $transaction->remote_status == COMMERCE_PAYMENT_STATUS_PENDING
    ) {
      commerce_payment_transaction_delete($order->data['transaction_id']);
    }
  }

  // Send them back to the shopping cart page instead of the previous page in
  // the checkout process.
  commerce_order_status_update($order, 'cart', FALSE, NULL, t('Customer canceled using @payment.', array(
    '@payment' => ucfirst($payment_method['title']),
  )));
  drupal_goto('checkout/' . $order->order_id);
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

  // Decode token if available.
  if (isset($input['get']['token']) && !empty($input['get']['token'])) {
    $input_data = base64_decode($input['get']['token']);
    // Explode using &
    $input_data = explode('&', $input_data);
    // Looping foreach input data.
    foreach ($input_data as $datum) {
      $valid_input = explode('=', $datum);
      if (isset($valid_input[0]) && isset($valid_input[1])) {
        $input['get'][$valid_input[0]] = $valid_input[1];
      }
    }
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

  // Check if there is settlement status.
  if (isset($input['get']['tr_id']) && !empty($input['get']['tr_id'])
    && isset($input['get']['tr_status']) && !empty($input['get']['tr_status'])
    && isset($input['get']['sign_key']) && !empty($input['get']['sign_key'])
    && isset($order->data['transaction_id'])
  ) {
    // Save the confirm data.
    $order->data['commerce_kredivo']['redirect'] = $input['get'];
    // Save order data.
    commerce_order_save($order);

    // Define data to update.
    $data = array(
      'tr_id' => $input['get']['tr_id'],
      'signature_key' => $input['get']['sign_key'],
    );
    $data = drupal_http_build_query($data);

    // Build request option.
    $request_option = $payment_method['settings'];
    $request_option['method'] = 'GET';
    $request_option['url'] = 'update';

    // Confirm the order status: tell kredivo the OK.
    $result = commerce_kredivo_api($data, $request_option);

    // Check the result: suppose to be json string.
    if (isset($result['result']) && !empty($result['result'])) {
      // Decode the string.
      $confirm = json_decode($result['result']);
      // Validate kredivo checkout data.
      if (!empty($confirm) && isset($confirm->status) && $confirm->status == 'OK'
        && isset($confirm->transaction_id) && isset($confirm->transaction_status)
        && isset($confirm->transaction_time) && isset($confirm->order_id)
        && isset($confirm->amount) && isset($confirm->fraud_status)
      ) {
        // Save the confirm data.
        $order->data['commerce_kredivo']['confirm'][] = $confirm;

        // Save order data.
        commerce_order_save($order);

        // Define payment transaction status.
        $confirm->transaction_status = strtolower($confirm->transaction_status);
        switch ($confirm->transaction_status) {
          case 'settlement':
            $status = COMMERCE_PAYMENT_STATUS_SUCCESS;
            break;

          case 'pending':
            $status = COMMERCE_PAYMENT_STATUS_PENDING;
            break;

          case 'deny':
          case 'cancel':
          default:
            $status = COMMERCE_PAYMENT_STATUS_FAILURE;
            break;
        }

        // Load transaction.
        $transaction = commerce_payment_transaction_load($order->data['transaction_id']);
        $transaction->remote_id = $confirm->transaction_id;
        $transaction->remote_status = $status;
        $transaction->amount = intval($confirm->amount);
        $transaction->status = $status;
        $transaction->message = '!status. Payment Data: <br />!data';
        $payment_data = '';
        foreach ((array) $confirm as $key => $data) {
          $payment_data[] = str_replace('_', ' ', $key) . ' = ' . $data;
        }
        $transaction->message_variables = array(
          '!status' => $confirm->transaction_status,
          '!data' => implode('<br />', $payment_data),
        );
        // Save payment transaction.
        commerce_payment_transaction_save($transaction);

        // Log the result.
        watchdog('commerce_kredivo', 'Kredivo update payment transaction for order @oid. <pre>Result: @result</pre><pre>Transaction: @transaction</pre><pre>Order: @order</pre><pre>Payment: @pm</pre>', array(
          '@oid' => $order->order_id,
          '@result' => print_r($result, TRUE),
          '@transaction' => print_r($transaction, TRUE),
          '@order' => print_r($order, TRUE),
          '@pm' => print_r($payment_method, TRUE),
        ), WATCHDOG_INFO, l(t('View Order @oid Revisions', array(
          '@oid' => $order->order_id,
        )), 'admin/commerce/orders/' . $order->order_id . '/revisions'));
      }
    }
  }

  // Check if there is confirm transaction from kredivo with negative status.
  if (isset($confirm->transaction_status) && in_array($confirm->transaction_status, array('cancel', 'deny'))) {
    commerce_order_status_update($order, 'canceled', FALSE, NULL, t('Kredivo payment status @status.', array(
      '@status' => $confirm->transaction_status,
    )));
    drupal_set_mesage(t('Your payment request has been canceled.'), 'error');
  }
  else {
    // Check whether order has paid.
    if (!(isset($order->data['commerce_payment_order_paid_in_full_invoked'])
      && !empty($order->data['commerce_payment_order_paid_in_full_invoked'])
    )) {
      // Notify customer that kredivo is processing the order.
      drupal_set_message(t('Your payment request has been submitted.'));
    }
  }
}
