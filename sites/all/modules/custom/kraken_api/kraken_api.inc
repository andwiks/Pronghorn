<?php

/**
 * @file
 */

function kraken_api_call($entity, $method = 'GET', $data = array()) {
  $settings = variable_get('kraken_api_settings', array());

  $uri = @parse_url($settings['endpoint']);
  $hostaddr = empty($uri['port']) ? $uri['host'] : $uri['host'] . ':' . $uri['port'];

  $url = $uri['scheme'] . '://' . $settings['username'] . ':' . $settings['secret'] . '@' . $hostaddr . $uri['path'] . '/' . $entity;

  $options = array(
    'headers' => array(
      'Accept' => 'application/json',
      'Content-Type' => 'application/json',
    ),
    'method' => $method,
    'data' => drupal_json_encode($data),
  );

  $result = drupal_http_request($url, $options);
  if (!empty($result->data)) {
    $data = drupal_json_decode($result->data);
    return empty($data) ? $result->data : $data;
  }
  else {
    return;
  }
}
