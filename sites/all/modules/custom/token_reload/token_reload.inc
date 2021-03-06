<?php

function token_reload_request_data($params = array(), $method = 'inq') {
  $settings = variable_get('token_reload_settings', array()) + token_reload_default_settings();
  $params += array(
    'product_code' => variable_get('token_reload_product_code', 'PLNPRA'),
    'meter_number' => NULL,
    'customer_number' => NULL,
    'phone_number' => NULL,
    'nominal' => NULL,
    'ref_1' => NULL,
    'ref_2' => NULL,
    'ref_3' => NULL,
  );

  // Set XML.
  $xml = new SimpleXMLElement('<methodCall/>');
  $xml->addChild('methodName', 'fastpay.' . $method);
  $xml_params = $xml->addChild('params');
  // Add value: Kode Produk.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $params['product_code']);
  // Add value: Id Pelanggan 1.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $params['meter_number']);
  // Add value: Id Pelanggan 2.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $params['customer_number']);
  // Add value: Id Pelanggan 3.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $params['phone_number']);

  // Add value: Nominal.
  if ($method == 'pay') {
    $xml_params->addChild('param')->addChild('value')->addChild('string', $params['nominal']);
  }

  // Add value: UID.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $settings['uid']);
  // Add value: PIN.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $settings['pin']);
  // Add value: REF1.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $params['ref_1']);

  if ($method == 'pay') {
    // Add value: REF2.
    $xml_params->addChild('param')->addChild('value')->addChild('string', $params['ref_2']);
    // Add value: REF3.
    $xml_params->addChild('param')->addChild('value')->addChild('string', $params['ref_3']);
  }

  return $xml->asXML();
}

function token_reload_request_query_data($params = array()) {
  $settings = variable_get('token_reload_settings', array()) + token_reload_default_settings();
  $params += array(
    'start' => NULL,
    'end' => NULL,
    'ref_2' => NULL,
    'product_code' => variable_get('token_reload_product_code', 'PLNPRA'),
    'meter_number' => NULL,
  );

  // Set XML.
  $xml = new SimpleXMLElement('<methodCall/>');
  $xml->addChild('methodName', 'fastpay.datatransaksi');
  $xml_params = $xml->addChild('params');
  // Add value: Tanggal 1.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $params['start']);
  // Add value: Tanggal 2.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $params['end']);
  // Add value: REF2.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $params['ref_2']);
  // Add value: Kode produk.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $params['product_code']);
  // Add value: Id pelanggan.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $params['meter_number']);
  // Add value: Limit.
  $xml_params->addChild('param')->addChild('value')->addChild('string');
  // Add value: UID.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $settings['uid']);
  // Add value: PIN.
  $xml_params->addChild('param')->addChild('value')->addChild('string', $settings['pin']);

  return $xml->asXML();
}

function token_reload_request_call($data, $method = 'inquiry') {
  drupal_static_reset(__FUNCTION__);
  $params = &drupal_static(__FUNCTION__);
  $settings = variable_get('token_reload_settings', array()) + token_reload_default_settings();

  $uri = @parse_url($settings['endpoint']);
  $url = $uri['scheme'] . '://' . $settings['username'] . ':' . $settings['secret'] . '@' . $uri['host'] . $uri['path'] . '/' . $method;
  $options = array(
    'headers' => array(
      'Content-Type' => 'application/json',
    ),
    'method' => 'POST',
    'data' => drupal_json_encode($data),
  );

  if ($method != 'inquiry') {
    $options['timeout'] = $settings['timeout'];
  }

  $result = drupal_http_request($url, $options);

  // Check the result:
  // 1. Must have 201 http code.
  // 2. Must have data.
  if ($result->code == 201 && !empty($result->data)) {
    if (variable_get('token_reload_log_request')) {
      watchdog('token_reload', '<pre>!request</pre>', array('!request' => htmlentities($result->request)), WATCHDOG_DEBUG);
    }
    if (variable_get('token_reload_log_response')) {
      watchdog('token_reload', '<pre>!response</pre>', array('!response' => htmlentities($result->data)), WATCHDOG_DEBUG);
    }
    return drupal_json_decode($result->data);
  }
  else {
    return FALSE;
  }
}

function token_reload_response_dummy($data, $method = 'inq') {
  foreach (simplexml_load_string($data)->params->param as $param) {
    $params[] = (string) $param->value->string;
  }

  // Set XML.
  $xml = new SimpleXMLElement('<methodResponse/>');
  $data = $xml->addChild('params')->addChild('param')->addChild('value')->addChild('array')->addChild('data');

  // $statuses = array('', '00', '15', '33', '99', 'XX');
  $statuses = array('', '00');
  $notes = array('SUCCESSFUL', 'KETERANGAN');
  if ($method == 'datatransaksi') {
    $status = $statuses[array_rand($statuses)];
    $result = "$params[2]#TRANSAKSIDATETIME#$params[3]#NAMAPRODUK#$params[4]#$status#KETERANGAN#NOMINAL#SN";

    $data->addChild('value')->addChild('string', $params[0]); // TANGGAL1
    $data->addChild('value')->addChild('string', $params[1]); // TANGGAL2
    $data->addChild('value')->addChild('string', $params[3]); // KODEPRODUK
    $data->addChild('value')->addChild('string', $params[4]); // IDPELANGGAN
    $data->addChild('value')->addChild('string');             // LIMIT
    $data->addChild('value')->addChild('string', $params[6]); // UID
    $data->addChild('value')->addChild('string', $params[7]); // PIN
    $data->addChild('value')->addChild('string', $status);    // STATUS
    $data->addChild('value')->addChild('string', $notes[array_rand($notes)]); // KETERANGAN
    $data->addChild('value')->addChild('string', $result);      // KETERANGAN
  }
  else {
    $data->addChild('value')->addChild('string', $params[0]); // KODEPRODUK
    $data->addChild('value')->addChild('string', format_date(REQUEST_TIME, 'custom', 'YmdHis'));
    $data->addChild('value')->addChild('string', $params[1]); // IDPELANGGAN1
    $data->addChild('value')->addChild('string', $params[2]); // IDPELANGGAN2
    $data->addChild('value')->addChild('string', $params[3]); // IDPELANGGAN3

    switch ($method) {
      case 'inq':
        $data->addChild('value')->addChild('string', 'NOMINAL');
        $data->addChild('value')->addChild('string', 'BIAYAADMIN');
        $data->addChild('value')->addChild('string', $params[4]); // UID
        $data->addChild('value')->addChild('string', $params[5]); // PIN
        $data->addChild('value')->addChild('string', $params[6]); // REF1
        $data->addChild('value')->addChild('string', 'REF2');
        $data->addChild('value')->addChild('string', 'REF3');
        break;
      case 'pay':
        $data->addChild('value')->addChild('string', $params[4]); // NOMINAL
        $data->addChild('value')->addChild('string', 'BIAYAADMIN');
        $data->addChild('value')->addChild('string', $params[5]); // UID
        $data->addChild('value')->addChild('string', $params[6]); // PIN
        $data->addChild('value')->addChild('string', $params[7]); // REF1
        $data->addChild('value')->addChild('string', $params[8]); // REF2
        $data->addChild('value')->addChild('string', $params[9]); // REF3
        break;
    }

    $data->addChild('value')->addChild('string', $statuses[array_rand($statuses)]);
    $data->addChild('value')->addChild('string', $notes[array_rand($notes)]);
    $data->addChild('value')->addChild('string', 'SWITCHERID');
    $data->addChild('value')->addChild('string', 'NOMOREMETER');
    $data->addChild('value')->addChild('string', 'IDPELANGGAN');
    $data->addChild('value')->addChild('string', 'FLAG');
    $data->addChild('value')->addChild('string', 'NOREF1');
    $data->addChild('value')->addChild('string', 'NOREF2');
    $data->addChild('value')->addChild('string', 'VENDINGRECEIPTNUMBER');
    $data->addChild('value')->addChild('string', 'NAMAPELANGGAN');
    $data->addChild('value')->addChild('string', 'SUBSCRIBERSEGMENTATION');
    $data->addChild('value')->addChild('string', 'POWERCONSUMINGCATEGORY');
    $data->addChild('value')->addChild('string', 'MINORUNITOFADMINCHARGE');
    $data->addChild('value')->addChild('string', 'ADMINCHARGE');
    $data->addChild('value')->addChild('string', 'BUYINGOPTION');
    $data->addChild('value')->addChild('string', 'DISTRIBUTIONCODE');
    $data->addChild('value')->addChild('string', 'SERVICEUNIT');
    $data->addChild('value')->addChild('string', 'SERVICEUNITPHONE');
    $data->addChild('value')->addChild('string', 'MAXKWHLIMIT');
    $data->addChild('value')->addChild('string', 'TOTALREPEATUNSOLDTOKEN');
    $data->addChild('value')->addChild('string', 'UNSOLD1');
    $data->addChild('value')->addChild('string', 'UNSOLD2');
    $data->addChild('value')->addChild('string', 'TOKENPLN');
    $data->addChild('value')->addChild('string', 'MINORUNITSTAMPDUTY');
    $data->addChild('value')->addChild('string', 'STAMPDUTY');
    $data->addChild('value')->addChild('string', 'MINORUNITPPN');
    $data->addChild('value')->addChild('string', 'PPN');
    $data->addChild('value')->addChild('string', 'MINORUNITPPJ');
    $data->addChild('value')->addChild('string', 'PPJ');
    $data->addChild('value')->addChild('string', 'MINORUNITCUSTOMERPAYABLESINSTALLMENT');
    $data->addChild('value')->addChild('string', 'CUSTOMERPAYABLESINSTALLMENT');
    $data->addChild('value')->addChild('string', 'MINORUNITOFPOWERPURCHASE');
    $data->addChild('value')->addChild('string', 'POWERPURCHASE');
    $data->addChild('value')->addChild('string', 'MINORUNITOFPURCHASEDKWHUNIT');
    $data->addChild('value')->addChild('string', 'PURCHASEDKWHUNIT');
    $data->addChild('value')->addChild('string', 'INFOTEXT');
  }

  return $xml->asXML();
}

function token_reload_response_keys() {
  $keys = array(
    'product_code',
    'time',
    'customer_number_1',
    'customer_number_2',
    'customer_number_3',
    'nominal',
    'admin_fee',
    'uid',
    'pin',
    'ref_1',
    'ref_2',
    'ref_3',
    'status',
    'note',
    'switcher_id',
    'meter_number',
    'customer_number',
    'flag',
    'no_ref_1',
    'no_ref_2',
    'vending_receipt_number',
    'customer_name',
    'subscriber_segmentation',
    'power_consuming_category',
    'minor_admin_charge',
    'admin_charge',
    'buying_option',
    'distribution_code',
    'service_unit',
    'service_unit_phone',
    'max_kwh_limit',
    'total_repeat_unsold_token',
    'unsold_1',
    'unsold_2',
    'token_pln',
    'minor_stamp_duty',
    'stamp_duty',
    'minor_ppn',
    'ppn',
    'minor_ppj',
    'ppj',
    'minor_customer_payables_installment',
    'customer_payables_installment',
    'minor_power_purchase',
    'power_purchase',
    'minor_purchased_kwh_unit',
    'purchased_kwh_unit',
    'info',
  );

  return $keys;
}

function token_reload_response_query_keys() {
  $keys = array(
    'ref_2',
    'time',
    'product_code',
    'product_name',
    'meter_number',
    'status',
    'note',
    'nominal',
    'no_ref_2',
  );

  return $keys;
}
