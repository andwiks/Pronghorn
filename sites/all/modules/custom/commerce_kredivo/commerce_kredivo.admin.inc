<?php
/**
 * @file
 * commerce_kredivo.admin.inc
 *
 * @author andre4s_y
 */

/**
 * Callback Functions.
 */

/**
 * Function commerce_kredivo_cancel_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $order
 *   Order object.
 * @param unknown $transaction
 *   Payment transaction object.
 *
 * @return array
 *   Form array.
 */
function commerce_kredivo_cancel_form($form = array(), &$form_state = array(), $order = NULL, $transaction = NULL) {
  // Check order and transaction value.
  if (empty($order) || empty($transaction)) {
    drupal_set_message(t('Commerce Kredivo can not get valid order and transaction objects.'), 'error');
    drupal_goto('admin/commerce/orders');
  }

  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  // Define option for list transaction id.
  $options = array();
  if (isset($order->data['commerce_kredivo']['confirm'])) {
    foreach ($order->data['commerce_kredivo']['confirm'] as $confirm) {
      // Only for pending transaction.
      if ($confirm->transaction_status == 'pending') {
        $options[$confirm->transaction_id] = t('@tid - @status - @amount - @time', array(
          '@tid' => $confirm->transaction_id,
          '@status' => $confirm->transaction_status,
          '@amount' => $confirm->amount,
          '@time' => format_date($confirm->transaction_time),
        ));
      }
    }
  }

  // List Kredivo transaction ID.
  $form['transaction_id'] = array(
    '#type' => 'select',
    '#title' => t('Transaction ID'),
    '#options' => $options,
    '#description' => t('Select transaction ID to cancel: transaction id - status - amount - transaction time. Only show pending transactions.'),
    '#required' => TRUE,
    '#empty_option' => t('No available transaction'),
  );

  // Define confirm form.
  $form = confirm_form($form, t('Are you sure you want to cancel the transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    NULL,
    t('Cancel Transaction'),
    t('Abort')
  );

  return $form;
}

/**
 * Function commerce_kredivo_cancel_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function commerce_kredivo_cancel_form_validate($form, &$form_state) {
  // 1. Check payment kredivo.
  // 2. Check transaction status: pending.
  // 3. Check payment remote id: not zero.
  // 4. Check payment expiration time.
  // @todo: check requirement no. 4.
  if (!(isset($form_state['transaction']->payment_method)
    && $form_state['transaction']->payment_method == 'commerce_kredivo'
    &&  $form_state['transaction']->status == COMMERCE_PAYMENT_STATUS_PENDING
    && !empty($form_state['transaction']->remote_id)
  )) {
    form_set_error('', t('Unable to cancel Kredivo transaction because transaction is not qualified for cancelation.'));
  }
}

/**
 * Function commerce_kredivo_cancel_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function commerce_kredivo_cancel_form_submit($form, &$form_state) {
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];

  // Define kredivo order id.
  if (isset($payment_method['settings']['order_prefix']) && !empty($payment_method['settings']['order_prefix'])) {
    $kredivo_order_id = preg_replace('#[^a-zA-Z]#', '', $payment_method['settings']['order_prefix']) . '-' . $order->order_id;
  }
  else {
    $kredivo_order_id = $order->order_id;
  }
  // Define cancel data.
  $data = array(
    'server_key' => $payment_method['settings']['server_key'],
    'order_id' => $kredivo_order_id,
    'transaction_id' => $form_state['values']['transaction_id'],
  );
  $data = json_encode($data);

  // Build request option.
  $request_option = $payment_method['settings'];
  $request_option['method'] = 'POST';
  $request_option['url'] = 'cancel';

  // Do http request for cancel transaction.
  $result = commerce_kredivo_api($data, $request_option);

  // Define default success flag.
  $success = FALSE;
  // Check the result: suppose to be json string.
  if (isset($result['result']) && !empty($result['result'])) {
    // Decode the string.
    $kredivo = json_decode($result['result']);
    // Validate kredivo checkout data.
    // Kredivo: status => 'success'.
    if (!empty($kredivo)
      && isset($kredivo->status) && stripos($kredivo->status, 'ok') !== FALSE
      && isset($kredivo->transaction_id)
      && isset($kredivo->transaction_status) && stripos($kredivo->transaction_status, 'cancel') !== FALSE
      && isset($kredivo->transaction_time) && isset($kredivo->order_id)
      && isset($kredivo->amount)
    ) {
      // Change success flag.
      $success = TRUE;
      // Show message to admin.
      drupal_set_message(t('Cancel operation has been submitted.'));
      // Redirect to payment page.
      $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
    }
  }
  // Check if unable to do cancel operation.
  if (empty($success)) {
    drupal_set_message(t('Unable to do cancel operation.'), 'error');
  }
}

/**
 * Function commerce_kredivo_confirm_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $order
 *   Order object.
 * @param unknown $transaction
 *   Payment transaction object.
 *
 * @return array
 *   Form array.
 */
function commerce_kredivo_confirm_form($form = array(), &$form_state = array(), $order = NULL, $transaction = NULL) {
  // Check order and transaction value.
  if (empty($order) || empty($transaction)) {
    drupal_set_message(t('Commerce Kredivo can not get valid order and transaction objects.'), 'error');
    drupal_goto('admin/commerce/orders');
  }

  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  // Ask for transaction id.
  $form['transaction_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Transaction ID'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  // Ask for signature key.
  $form['signature_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Signature Key'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  // Define confirm form.
  $form = confirm_form($form, t('Are you sure you want to confirm the transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    NULL,
    t('Confirm Transaction'),
    t('Abort')
  );

  return $form;
}

/**
 * Function commerce_kredivo_confirm_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function commerce_kredivo_confirm_form_validate($form, &$form_state) {
  // 1. Check payment kredivo.
  // 2. Check transaction status: pending.
  // 3. Check payment remote id: not zero.
  // 4. Check payment expiration time.
  // @todo: check requirement no. 4.
  if (!(isset($form_state['transaction']->payment_method)
      && $form_state['transaction']->payment_method == 'commerce_kredivo'
  )) {
    form_set_error('', t('Unable to cancel Kredivo transaction because transaction is not qualified for cancelation.'));
  }
}

/**
 * Function commerce_kredivo_confirm_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function commerce_kredivo_confirm_form_submit($form, &$form_state) {
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];
  $transaction = $form_state['transaction'];
  $success = FALSE;

  // Define data to update.
  $data = array(
    'tr_id' => $form_state['values']['transaction_id'],
    'signature_key' => $form_state['values']['signature_key'],
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

      // There is possibility from settlement, status gone to cancel.
      if ($transaction->status == COMMERCE_PAYMENT_STATUS_SUCCESS
        && $status == COMMERCE_PAYMENT_STATUS_FAILURE
      ) {
        // Log this condition as error.
        watchdog('commerce_kredivo', 'Kredivo order settlement fo failure for order @oid. <pre>Confirm: @confirm</pre><pre>Transaction: @transaction</pre><pre>Order: @order</pre>', array(
          '@oid' => $order->order_id,
          '@confirm' => print_r($confirm, TRUE),
          '@transaction' => print_r($transaction, TRUE),
          '@order' => print_r($order, TRUE),
        ), WATCHDOG_ERROR, l(t('View Order @oid Revisions', array(
          '@oid' => $order->order_id,
        )), 'admin/commerce/orders/' . $order->order_id . '/revisions'));
      }
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
      watchdog('commerce_kredivo', 'Kredivo update payment transaction for order @oid. <pre>Result: @result</pre><pre>Order: @order</pre><pre>Payment: @pm</pre>', array(
        '@oid' => $order->order_id,
        '@transaction' => print_r($transaction, TRUE),
        '@order' => print_r($order, TRUE),
        '@pm' => print_r($payment_method, TRUE),
      ), WATCHDOG_INFO, l(t('View Order @oid Revisions', array(
        '@oid' => $order->order_id,
      )), 'admin/commerce/orders/' . $order->order_id . '/revisions'));

      // Show message to admin.
      drupal_set_message(t('Confirm operation has been submitted.'));
      // Change success flag.
      $success = TRUE;
      // Redirect to payment page.
      $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
    }
  }
  // Check if unable to do cancel operation.
  if (empty($success)) {
    drupal_set_message(t('Unable to do confirm operation.'), 'error');
  }
}
