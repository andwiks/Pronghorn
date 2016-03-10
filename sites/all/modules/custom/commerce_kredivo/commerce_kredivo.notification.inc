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
 *
 * @param object $order
 *   Order object.
 * @param object $transaction
 *   Payment transaction object.
 */
function commerce_kredivo_notification($order = NULL, $transaction = NULL) {
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
  watchdog('commerce_kredivo', 'Kredivo Push Notification Data. <pre>Input: @input</pre><pre>Order: @order</pre><pre>Transaction: @transaction</pre>', array(
    '@input' => print_r($input, TRUE),
    '@order' => print_r($order, TRUE),
    '@transaction' => print_r($transaction, TRUE),
  ), WATCHDOG_INFO, 'Kredivo Push Notification');

  // Check input post from Kredivo.
  if (isset($input['post']) && !empty($input['post'])) {
    // Input post: json string.
    $kredivo = json_decode($input['post']);
    // Check whether kredivo send valid push notification data.
    if (!empty($kredivo) && isset($kredivo->status)
      && isset($kredivo->transaction_id) && isset($kredivo->transaction_status)
      && isset($kredivo->transaction_time) && isset($kredivo->order_id)
      && isset($kredivo->amount) && isset($kredivo->signature_key)
      && isset($order->order_id) && isset($transaction->transaction_id)
    ) {
      // Check if payment is really using Kredivo.
      if (isset($order->data['payment_method']) && strpos($order->data['payment_method'], 'kredivo') !== FALSE) {
        // Save the confirm data.
        $order->data['commerce_kredivo']['notification'][] = $kredivo;
        // Save order data.
        commerce_order_save($order);

        // Load payment method data.
        $payment_method = commerce_payment_method_instance_load($order->data['payment_method']);

        // Check debug mode.
        if (!empty($payment_method['settings']['debug']['debug_mode'])) {
          // Save this push notification data inside order.
          watchdog('commerce_kredivo', 'Kredivo Push Notification Debug <pre>Input: @input</pre><pre>Kredivo: @kredivo</pre><pre>Order: @order</pre>', array(
            '@input' => print_r($input, TRUE),
            '@kredivo' => print_r($kredivo, TRUE),
            '@order' => print_r($order, TRUE),
          ), WATCHDOG_DEBUG, l(t('View Order @oid Revisions', array(
            '@oid' => $order->order_id,
          )), 'admin/commerce/orders/' . $order->order_id . '/revisions'));
        }

        // Define data to update.
        $data = array(
          'tr_id' => $kredivo->transaction_id,
          'signature_key' => $kredivo->signature_key,
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
          }
        }
      }
    }
    else {
      // Log it: json error.
      watchdog('commerce_kredivo', 'Kredivo Push Notification Error with message: @message. <pre>@input</pre>', array(
        '@message' => json_last_error_msg(),
        '@input' => print_r($input, TRUE),
      ), WATCHDOG_ERROR);
    }
  }

  // For every request, return just json "OK".
  exit('{"status":"OK"}');
}
