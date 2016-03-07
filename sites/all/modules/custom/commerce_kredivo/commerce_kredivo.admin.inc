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
  // Define cancel data.
  $data = array(
    'server_key' => $payment_method['settings']['server_key'],
    'order_id' => preg_replace('#[^a-zA-Z]#', '', $payment_method['settings']['order_prefix']) . '-' . $order->order_id,
    'transaction_id' => $form_state['values']['transaction_id'],
  );
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
  $result = drupal_http_request($payment_method['settings']['server_url'] . '/cancel', $request_option);

  // Log this result.
  watchdog('commerce_kredivo', 'Kredivo request cancel transaction result for debug: <pre>@result</pre>', array(
    '@result' => print_r($result, TRUE),
  ), WATCHDOG_DEBUG, l(t('View Order @oid Revisions', array(
    '@oid' => $order->order_id,
  )), 'admin/commerce/orders/' . $order->order_id . '/revisions'));

  // Define default success flag.
  $success = FALSE;
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
      && isset($kredivo->status) && stripos($kredivo->status, 'ok') !== FALSE
      && isset($kredivo->transaction_id)
      && isset($kredivo->transaction_status) && stripos($kredivo->transaction_status, 'cancel') !== FALSE
      && isset($kredivo->transaction_time) && isset($kredivo->order_id)
      && isset($kredivo->amount)
    ) {
      $success = TRUE;
      drupal_set_message(t('Cancel operation has been submitted.'));
    }
  }
  // Check if unable to do cancel operation.
  if (empty($success)) {
    drupal_set_message(t('Unable to do cancel operation.'), 'error');
  }
}
