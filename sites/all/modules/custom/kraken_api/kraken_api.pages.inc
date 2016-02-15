<?php

/**
 * @file
 */

function kraken_api_notification_callback() {
  global $user;

  if ($_SERVER['REQUEST_METHOD'] != 'POST') {
    drupal_add_http_header('status', '405 Method Not Allowed');
  }

  $headers = getallheaders();

  // if ($headers['User-Agent'] != 'Kraken') {
  //   drupal_add_http_header('status', '417 Expectation Failed');
  // }

  $body = file_get_contents('php://input');

  if (!empty($body)) {
    $payload = drupal_json_decode($body);
    watchdog('kraken', '<pre>' . print_r($payload, TRUE) . '</pre>', array(), WATCHDOG_DEBUG);

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'commerce_line_item');
    $query->fieldCondition('field_transaction_id', 'value', $payload['transaction_id']);

    $result = $query->execute();

    if (empty($result['commerce_line_item'])) {
      // @deprecated Backward compatibility for old product type.
      switch ($payload['type']) {
        case 'electricity':
          $token_reload = entity_load('token_reload', FALSE, array('no_ref_2' => $payload['transaction_id']));
          if (!empty($token_reload)) {
            $token_reload = reset($token_reload);

            $info = entity_get_property_info('token_reload');
            foreach ($payload['pln'] as $key => $value) {
              if (isset($info['properties'][$key])) {
                $token_reload->$key = $value;
              }
            }
            $token_reload->no_ref_2 = $payload['transaction_id'];

            if (isset($token_reload->token_pln)) {
              $token_reload->token_pln = implode(' ', str_split($token_reload->token_pln, 4));
            }

            $token_reload->data = $payload;
            $token_reload->revision = TRUE;
            entity_save('token_reload', $token_reload);

            $line_item = commerce_line_item_load($token_reload->line_item_id);
            $line_item->field_line_item_status[LANGUAGE_NONE][0]['value'] = $payload['status'];
            commerce_line_item_save($line_item);
          }
          break;

        case 'mobile':
          $topup = entity_load('topup', FALSE, array('ref' => $payload['transaction_id']));
          if (!empty($topup)) {
            $topup = reset($topup);

            switch ($payload['status']) {
              case 'success':
                $status = TOPUP_SUCCESS;
                break;

              case 'failed':
                $status = TOPUP_FAILED;
                break;
            }

            $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $topup->line_item_id);
            $line_item_wrapper->field_line_item_status->set($payload['status']);
            $line_item_wrapper->save();

            $user = user_load($line_item_wrapper->order->uid->value());
            $topup->status = $status;
            entity_save('topup', $topup);
            topup_update_status($topup->line_item_id, $status);
          }
          break;
      }
    }
    else {
      $line_item = commerce_line_item_load(key($result['commerce_line_item']));
      $line_item->field_line_item_status[LANGUAGE_NONE][0]['value'] = $payload['status'];
      $line_item->data = $payload;
      commerce_line_item_save($line_item);
      entity_get_controller('commerce_line_item')->resetCache(array($line_item->line_item_id));
    }
  }

  drupal_json_output();
}
