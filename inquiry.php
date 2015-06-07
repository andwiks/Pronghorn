<?php

$line_items = entity_load('commerce_line_item', array(30852, 31630, 32074, 32500));

foreach ($line_items as $line_item) {
  module_load_include('inc', 'token_reload');
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

  $params = array(
    'meter_number' => $line_item_wrapper->electricity_customer_number->value(),
    'customer_number' => $line_item_wrapper->electricity_customer_number->value(),
    'phone_number' => $line_item_wrapper->electricity_phone_number->value(),
  );

  $inquiry = token_reload_request_call(token_reload_request_data($params));
  if ($inquiry) {
    if ($inquiry['status'] != '00') {
      $params['meter_number'] = NULL;
      $params['customer_number'] = $line_item_wrapper->electricity_customer_number->value();
      $inquiry = token_reload_request_call(token_reload_request_data($params));
    }

    if ($inquiry['status'] == '00') {
      $line_item->data['inquiry'] = $inquiry;
      entity_save('commerce_line_item', $line_item);
    }
    drush_print_r($inquiry);
  }
}
