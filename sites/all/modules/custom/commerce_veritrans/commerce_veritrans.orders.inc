<?php
/**
 * @file
 * commerce_veritrans.orders.inc
 *
 * @author andre4s_y
 */

/**
 * Callback Functions.
 */

/**
 * Function commerce_veritrans_payment_approve_form_wrapper().
 *
 * @param object $order
 *   Order object.
 * @param object $transaction
 *   Payment transaction object.
 */
function commerce_veritrans_payment_approve_form_wrapper($order, $transaction) {
  // Get latest payment response.
  $response = (isset($order->data['commerce_veritrans']['response'])) ? $order->data['commerce_veritrans']['response'] : new stdClass();
  // Check whether payment transaction is pending.
  if ($transaction->status == COMMERCE_PAYMENT_STATUS_PENDING && !empty($response)) {
    // 1. Check whether fraud status is challenge.
    // 2. Check whether status code is 201.
    if (isset($response->status_code) && $response->status_code == '201'
      && isset($response->fraud_status) && strtolower($response->fraud_status) == 'challenge'
    ) {
      // Get payment method settings.
      $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
      // Check payment settings.
      if (isset($payment_method['settings']) && !empty($payment_method['settings'])) {
        // Return confirmation form.
        return drupal_get_form('commerce_veritrans_payment_approve_form', $order, $transaction, $payment_method);
      }
      else {
        drupal_set_message(t('Unable to approve payment because unable to get payment settings.'), 'error');
      }
    }
  }
  // Show message that can not approve the payment.
  drupal_set_message(t('Can not approve payment transaction because fraud status has status: %status.', array(
    '%status' => (isset($response->fraud_status)) ? $response->fraud_status : t('unknown'),
  )), 'warning');
  // Redirect to order payment transaction list.
  drupal_goto('admin/commerce/orders/' . $order->order_id . '/payment');
}

/**
 * Function commerce_veritrans_payment_approve_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $order
 *   Order object.
 * @param object $transaction
 *   Payment transaction object.
 * @param array $payment_method
 *   Payment method settings.
 *
 * @return array
 *   Form array.
 */
function commerce_veritrans_payment_approve_form($form, $form_state, $order, $transaction, $payment_method) {
  // Show payment information.
  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment Information'),
  );

  // Define payment response.
  $response = $order->data['commerce_veritrans']['response'];
  // Looping foreach payment response data.
  foreach ((array) $response as $key => $data) {
    $form['info'][$key] = array(
      '#type' => 'item',
      '#title' => $key,
      '#markup' => $data,
    );
  }

  // Show warning message.
  $form['warning'] = array(
    '#markup' => t('Are you sure want to approve this payment?'),
  );

  // Submit fieldset.
  $form['submit'] = array(
    '#type' => 'fieldset',
    '#title' => '',
  );

  // Submit form: approve.
  $form['submit']['approve'] = array(
    '#type' => 'submit',
    '#value' => t('Approve'),
  );

  $form['submit']['or'] = array(
    '#markup' => '<span class="button-operator">' . t('or') . '</span>&nbsp;&nbsp;',
  );

  // Cancel button.
  $form['submit']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Function commerce_veritrans_payment_approve_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function commerce_veritrans_payment_approve_form_validate($form, &$form_state) {
  // Get form operation.
  if (!(isset($form_state['values']['op'])
    && ($form_state['values']['op'] == $form['submit']['approve']['#value']
    || $form_state['values']['op'] == $form['submit']['cancel']['#value']))
  ) {
    form_set_error('op', t('Unknown form operation.'));
  }
}

/**
 * Function commerce_veritrans_payment_approve_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function commerce_veritrans_payment_approve_form_submit($form, &$form_state) {
  // Cancel operation.
  if ($form_state['values']['op'] == $form['submit']['cancel']['#value']) {
    $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['build_info']['args'][0]->order_id . '/payment';
  }
  elseif ($form_state['values']['op'] == $form['submit']['approve']['#value']) {
    // Get order object.
    $order = $form_state['build_info']['args'][0];
    // Get payment transaction object.
    $transaction = $form_state['build_info']['args'][1];
    // Get payment settings.
    $payment_method = $form_state['build_info']['args'][2];

    // Define approve operation settings.
    $settings = array(
      'url' => $payment_method['settings']['url'] . '/' . $payment_method['settings']['prefix'] . $order->order_id,
      'server_key' => $payment_method['settings']['server_key'],
      'timeout' => $payment_method['settings']['timeout'],
      'op' => 'approve',
    );
    // Call payment api.
    $payment = commerce_veritrans_api('', $settings);

    // Decode the string.
    $payment = json_decode($payment);
    // Chek json decode result.
    if (json_last_error() == JSON_ERROR_NONE) {
      // Check payment status.
      if (isset($payment->status_code) && $payment->status_code == '200'
        && isset($payment->transaction_status) && strtolower($payment->transaction_status) == 'capture'
      ) {
        $status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        commerce_veritrans_token_new($order, $order->data['commerce_veritrans']['response'], $order->data['commerce_veritrans']['phone']);
      }
      elseif ((isset($payment->status_code) && $payment->status_code == '201')
        || (isset($payment->transaction_status) && strtolower($payment->transaction_status) == 'challenge')
      ) {
        $status = COMMERCE_PAYMENT_STATUS_PENDING;
      }
      else {
        $status = COMMERCE_PAYMENT_STATUS_FAILURE;
      }

      // Update payment transaction.
      $order->data['commerce_veritrans']['response'] = $payment;

      // Save order data.
      commerce_order_save($order);

      // Get order total.
      $total = field_get_items('commerce_order', $order, 'commerce_order_total');
      // Create new payment transaction.
      $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
      $transaction->instance_id = $payment_method['instance_id'];
      $transaction->remote_id = (isset($payment->order_id)) ? $payment->order_id : $order->order_id;
      $transaction->remote_status = $status;
      $transaction->amount = (isset($payment->gross_amount)) ? intval($payment->gross_amount) : 0;
      $transaction->currency_code = $total[0]['currency_code'];
      $transaction->status = $status;
      $transaction->message = 'Payment Data: <br />!data';
      $payment_data = '';
      foreach ((array) $payment as $key => $data) {
        $payment_data[] = str_replace('_', ' ', $key) . ' = ' . ((is_array($data)) ? '<ul><li>' . implode('</li><li>', $data) . '</li></ul>' : $data);
      }
      $transaction->message_variables = array(
        '!data' => implode('<br />', $payment_data),
      );
      commerce_payment_transaction_save($transaction);

      // Show notification to admin.
      drupal_set_message(t('Payment transaction: @message.', array(
        '@message' => (isset($payment->status_message)) ? $payment->status_message : t('approved'),
      )));
    }
    else {
      // Log this condition.
      watchdog('commerce_veritrans', 'Payment server return invalid json. Debug: <pre>@payment</pre> <pre>@pmethod</pre> <pre>@order</pre>', array(
        '@payment' => print_r($payment),
        '@pmethod' => print_r($payment_method),
        '@order' => print_r($order),
      ), WATCHDOG_ERROR, 'Veritrans Approve');
      // Payment API error: invalid json.
      drupal_set_message(t('Payment approve error: unable to interpret payment server return value.'), 'error');
    }

    // Redirect to order payment transaction list.
    $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
  }
}

/**
 * Function commerce_veritrans_payment_cancel_form_wrapper().
 *
 * @param object $order
 *   Order object.
 * @param object $transaction
 *   Payment transaction object.
 */
function commerce_veritrans_payment_cancel_form_wrapper($order, $transaction) {
  // Get latest payment response.
  $response = (isset($order->data['commerce_veritrans']['response'])) ? $order->data['commerce_veritrans']['response'] : new stdClass();
  // Check whether payment transaction is success.
  if ($transaction->status == COMMERCE_PAYMENT_STATUS_SUCCESS
    && !empty($response)
  ) {
    // Get unix timestamp from transaction time.
    $transaction_time = strtotime($response->transaction_time);
    // Check whether strtotime fail.
    if (empty($transaction_time)) {
      // String example: 2025-02-10 14:54:11 (Y-m-d H:i:s)
      $transaction_time = DateTime::createFromFormat('Y-m-d H:i:s', $response->transaction_time);
      $transaction_time = $transaction_time->format('U');
    }
    // 1. Check whether status code is 200.
    // 2. Check whether transaction status is capture.
    // 3. Payment transaction time must before 24 hours.
    if (isset($response->status_code) && $response->status_code == '200'
      && isset($response->transaction_status) && $response->transaction_status == 'capture'
      && $transaction_time + 86400 > time()
    ) {
      // Get payment method settings.
      $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
      // Check payment settings.
      if (isset($payment_method['settings']) && !empty($payment_method['settings'])) {
        // Return confirmation form.
        return drupal_get_form('commerce_veritrans_payment_cancel_form', $order, $transaction, $payment_method);
      }
    }
  }

  // Show message that can not cancel the payment.
  drupal_set_message(t('Can not cancel payment transaction. Current status: %status (@code - @tstatus). Maximum cancel time: @time', array(
    '%status' => $transaction->status,
    '@code' => (isset($response->status_code)) ? $response->status_code : 'XXX',
    '@tstatus' => (isset($response->transaction_status)) ? $response->transaction_status : t('unknown'),
    '@time' => (isset($response->transaction_time)) ? date('Y-m-d H:i:s', strtotime($response->transaction_time) + 86400) : t('unknown'),
  )), 'warning');
  // Redirect to order payment transaction list.
  drupal_goto('admin/commerce/orders/' . $order->order_id . '/payment');
}

/**
 * Function commerce_veritrans_payment_cancel_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $order
 *   Order object.
 * @param object $transaction
 *   Payment transaction object.
 * @param array $payment_method
 *   Payment method settings.
 *
 * @return array
 *   Form array.
 */
function commerce_veritrans_payment_cancel_form($form, $form_state, $order, $transaction, $payment_method) {
  // Show payment information.
  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment Information'),
  );

  // Define payment response.
  $response = $order->data['commerce_veritrans']['response'];
  // Looping foreach payment response data.
  foreach ((array) $response as $key => $data) {
    $form['info'][$key] = array(
      '#type' => 'item',
      '#title' => $key,
      '#markup' => $data,
    );
  }

  // Show warning message.
  $form['warning'] = array(
    '#markup' => t('Are you sure want to cancel this payment?'),
  );

  // Submit fieldset.
  $form['submit'] = array(
    '#type' => 'fieldset',
    '#title' => '',
  );

  // Submit form: cancel.
  $form['submit']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Yes'),
  );

  $form['submit']['or'] = array(
    '#markup' => '<span class="button-operator">' . t('or') . '</span>&nbsp;&nbsp;',
  );

  // Cancel button.
  $form['submit']['return'] = array(
    '#type' => 'submit',
    '#value' => t('No'),
  );

  return $form;
}

/**
 * Function commerce_veritrans_payment_cancel_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function commerce_veritrans_payment_cancel_form_validate($form, &$form_state) {
  // Get form operation.
  if (!(isset($form_state['values']['op'])
    && ($form_state['values']['op'] == $form['submit']['cancel']['#value']
    || $form_state['values']['op'] == $form['submit']['return']['#value']))
  ) {
    form_set_error('op', t('Unknown form operation.'));
  }
}

/**
 * Function commerce_veritrans_payment_cancel_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function commerce_veritrans_payment_cancel_form_submit($form, &$form_state) {
  // Return operation.
  if ($form_state['values']['op'] == $form['submit']['return']['#value']) {
    $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['build_info']['args'][0]->order_id . '/payment';
  }
  elseif ($form_state['values']['op'] == $form['submit']['cancel']['#value']) {
    // Get order object.
    $order = $form_state['build_info']['args'][0];
    // Get payment transaction object.
    $transaction = $form_state['build_info']['args'][1];
    // Get payment settings.
    $payment_method = $form_state['build_info']['args'][2];

    // Define cancel operation settings.
    $settings = array(
      'url' => $payment_method['settings']['url'] . '/' . $payment_method['settings']['prefix'] . $order->order_id,
      'server_key' => $payment_method['settings']['server_key'],
      'op' => 'cancel',
      'timeout' => $payment_method['settings']['timeout'],
    );
    // Call payment api.
    $payment = commerce_veritrans_api('', $settings);

    // Decode the string.
    $payment = json_decode($payment);
    // Chek json decode result.
    if (json_last_error() == JSON_ERROR_NONE) {
      // Check payment status.
      if (isset($payment->status_code) && $payment->status_code == '200'
        && isset($payment->transaction_status) && strtolower($payment->transaction_status) == 'cancel'
      ) {
        // Update transaction from success to failed.
        $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        commerce_payment_transaction_save($transaction);

        // Update payment transaction.
        $order->data['commerce_veritrans']['response'] = $payment;

        // Save order data.
        commerce_order_save($order);

        // Get order total.
        $total = field_get_items('commerce_order', $order, 'commerce_order_total');
        // Add new failed transaction.
        $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
        $transaction->instance_id = $payment_method['instance_id'];
        $transaction->remote_id = (isset($payment->order_id)) ? $payment->order_id : $order->order_id;
        $transaction->remote_status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        $transaction->amount = 0;
        $transaction->currency_code = $total[0]['currency_code'];
        $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        $transaction->message = 'Payment Data: <br />!data';
        $payment_data = '';
        foreach ((array) $payment as $key => $data) {
          $payment_data[] = str_replace('_', ' ', $key) . ' = ' . ((is_array($data)) ? '<ul><li>' . implode('</li><li>', $data) . '</li></ul>' : $data);
        }
        $transaction->message_variables = array(
          '!data' => implode('<br />', $payment_data),
        );
        commerce_payment_transaction_save($transaction);

        // Show notification to admin.
        drupal_set_message(t('Payment transaction: @message.', array(
          '@message' => (isset($payment->status_message)) ? $payment->status_message : t('canceled'),
        )));
      }
      else {
        // Log this condition.
        watchdog('commerce_veritrans', 'Payment server return invalid json. Debug: <pre>@payment</pre> <pre>@pmethod</pre> <pre>@order</pre>', array(
          '@payment' => print_r($payment),
          '@pmethod' => print_r($payment_method),
          '@order' => print_r($order),
        ), WATCHDOG_ERROR, 'Veritrans Cancel');
        // Unknown return value.
        drupal_set_message(t('Unknown cancel reponse: !response', array(
          '!response' => '<pre>'.print_r($payment, TRUE).'</pre>',
        )), 'warning');
      }
    }
    else {
      // Log this condition.
      watchdog('commerce_veritrans', 'Payment server return invalid json. Debug: <pre>@payment</pre> <pre>@pmethod</pre> <pre>@order</pre>', array(
        '@payment' => print_r($payment),
        '@pmethod' => print_r($payment_method),
        '@order' => print_r($order),
      ), WATCHDOG_ERROR, 'Veritrans Cancel');
      // Payment API error: invalid json.
      drupal_set_message(t('Payment cancel error: unable to interpret payment server return value.'), 'error');
    }

    // Redirect to order payment transaction list.
    $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
  }
}
