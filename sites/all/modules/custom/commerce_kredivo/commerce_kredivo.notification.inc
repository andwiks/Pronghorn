<?php
/**
 * @file
 * commerce_kredivo.notification.inc
 *
 * @author andre4s_y
 */

/**
 * Callback Functions.
 */

/**
 * Function commerce_kredivo_notification().
 */
function commerce_kredivo_notification() {
  // Log this condition.
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
  watchdog('commerce_kredivo', 'Kredivo Push Notification Data. <pre>Input: @input</pre>', array(
    '@input' => print_r($input, TRUE),
  ), WATCHDOG_INFO);

  // Check input post from Kredivo.
  if (isset($input['post']) && !empty($input['post'])) {
    // Input post: json string.
    $kredivo = json_decode($input['post']);
    // Check whether kredivo send valid push notification data.
    if (!empty($kredivo) && isset($kredivo->status)
      && isset($kredivo->transaction_id) && isset($kredivo->transaction_status)
      && isset($kredivo->transaction_time) && isset($kredivo->order_id)
      && isset($kredivo->amount) && isset($kredivo->signature_key)
    ) {
      // Get order id: remove prefix if available.
      $order_id = preg_replace('#\D#', '', $kredivo->order_id);
      $order_id = intval($order_id);

      // Load order object.
      $order = commerce_order_load($order_id);

      // Check if payment is really using Kredivo.
      if (isset($order->data['payment_method']) && strpos($order->data['payment_method'], 'kredivo') !== FALSE) {
        // Save the confirm data.
        $order->data['commerce_kredivo']['notification'][] = $kredivo;
        // Save order data.
        commerce_order_save($order);

        // Save this push notification data inside order.
        watchdog('commerce_kredivo', 'Kredivo Push Notification Debug <pre>Input: @input</pre><pre>Kredivo: @kredivo</pre><pre>Order: @order</pre>', array(
          '@input' => print_r($input, TRUE),
          '@kredivo' => print_r($kredivo, TRUE),
          '@order' => print_r($order, TRUE),
        ), WATCHDOG_DEBUG);

        // Load payment method data.
        $payment_method = commerce_payment_method_instance_load($order->data['payment_method']);

        // Sanitize $payment_method['settings']['server_url'].
        $payment_method['settings']['server_url'] = preg_replace('#\/$#', '', $payment_method['settings']['server_url']);

        // Define target update url: add query data.
        $url = url($payment_method['settings']['server_url'] . '/update', array(
          'query' => array(
            'transaction_id' => $kredivo->transaction_id,
            'signature_key' => $kredivo->signature_key,
          ),
          'external' => TRUE,
        ));

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
          'timeout' => (isset($payment_method['settings']['debug']['request_timeout']) && !empty($payment_method['settings']['debug']['request_timeout'])) ? (float) $payment_method['settings']['debug']['request_timeout'] : 30.0,
          'context' => $stream_context,
        );

        // Confirm the order status: tell kredivo the OK.
        $result = drupal_http_request($url, $request_option);

        // Log the result.
        watchdog('commerce_kredivo', 'Kredivo confirm order status result for order @oid. <pre>Result: @result</pre><pre>Order: @order</pre><pre>Payment: @pm</pre>', array(
          '@oid' => print_r($order->order_id, TRUE),
          '@result' => print_r($result, TRUE),
          '@order' => print_r($order, TRUE),
          '@pm' => print_r($payment_method, TRUE),
        ), WATCHDOG_INFO);

        // Check the result: suppose to be json string.
        // 1. Must have 200 http code.
        // 2. Must have data.
        if (isset($result->code) && $result->code == 200
          && isset($result->data) && !empty($result->data)
        ) {
          // Decode the string.
          $confirm = json_decode($result->data);
          // Validate kredivo checkout data.
          if (!empty($confirm) && isset($confirm->status) && $confirm->status == 'OK'
            && isset($confirm->transaction_id) && isset($confirm->transaction_status)
            && isset($confirm->transaction_time) && isset($confirm->order_id)
            && isset($confirm->amount) && isset($confirm->fraud_status)
          ) {
            // Get order id: remove prefix if available.
            $order_id = preg_replace('#\D#', '', $confirm->order_id);
            $order_id = intval($order_id);

            // Reload order object.
            $order = commerce_order_load($order_id);

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

            // Check previous payment pending if any.
            $query = new EntityFieldQuery();
            $query->entityCondition('entity_type', 'commerce_payment_transaction')
            ->propertyCondition('order_id', $order->order_id, '=')
            ->propertyOrderBy('revision_id', 'DESC')
            ->propertyCondition('remote_id', $confirm->transaction_id)
            ->propertyCondition('amount', intval($confirm->amount), '=')
            ->range(0, 1);
            $result = $query->execute();
            if (isset($result['commerce_payment_transaction']) && !empty($result['commerce_payment_transaction'])) {
              // Load previous payment transaction.
              $transaction = commerce_payment_transaction_load(key($result['commerce_payment_transaction']));
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
                  '@oid' => print_r($order->order_id, TRUE),
                )), 'admin/commerce/orders/' . $order->order_id . '/revisions'));
              }
            }
            else {
              // Create new payment transaction.
              $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
              $transaction->instance_id = $payment_method['instance_id'];
              $transaction->remote_id = $confirm->transaction_id;
              $transaction->amount = intval($confirm->amount);
              $transaction->currency_code = 'IDR';
            }
            $transaction->remote_status = $status;
            $transaction->status = $status;
            $transaction->message = 'Payment Data: <br />!data';
            $payment_data = '';
            foreach ((array) $confirm as $key => $data) {
              $payment_data[] = str_replace('_', ' ', $key) . ' = ' . $data;
            }
            $transaction->message_variables = array(
              '!data' => implode('<br />', $payment_data),
            );
            // Save payment transaction.
            commerce_payment_transaction_save($transaction);

            // Log the result.
            watchdog('commerce_kredivo', 'Kredivo update payment transaction for order @oid. <pre>Result: @result</pre><pre>Order: @order</pre><pre>Payment: @pm</pre>', array(
              '@oid' => print_r($order->order_id, TRUE),
              '@transaction' => print_r($transaction, TRUE),
              '@order' => print_r($order, TRUE),
              '@pm' => print_r($payment_method, TRUE),
            ), WATCHDOG_INFO, l(t('View Order @oid Revisions', array(
              '@oid' => print_r($order->order_id, TRUE),
            )), 'admin/commerce/orders/' . $order->order_id . '/revisions'));
          }
        }
      }
    }
  }

  // For every request, return just "OK".
  exit('OK');
}
