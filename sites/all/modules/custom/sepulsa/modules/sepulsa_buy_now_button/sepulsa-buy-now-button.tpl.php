<?php
/**
 * @file
 * Sepulsa Buy Now Button TPL.
 */

$array_query = array();
if (isset($_COOKIE['utm_source_cookie']) &&
  !empty($_COOKIE['utm_source_cookie'])) {
  $array_query['utm_source'] = $_COOKIE['utm_source_cookie'];
}
else {
  if (isset($data['sepulsa_buy_now_button_utm_source']) &&
    !empty($data['sepulsa_buy_now_button_utm_source'])) {
    $array_query['utm_source'] = $data['sepulsa_buy_now_button_utm_source'];
  }
}

if (isset($_COOKIE['utm_medium_cookie']) &&
  !empty($_COOKIE['utm_medium_cookie'])) {
  $array_query['utm_medium'] = $_COOKIE['utm_medium_cookie'];
}
else {
  if (isset($data['sepulsa_buy_now_button_utm_medium']) &&
    !empty($data['sepulsa_buy_now_button_utm_medium'])) {
    $array_query['utm_medium'] = $data['sepulsa_buy_now_button_utm_medium'];
  }
}

if (isset($_COOKIE['utm_campaign_cookie']) &&
  !empty($_COOKIE['utm_campaign_cookie'])) {
  $array_query['utm_campaign'] = $_COOKIE['utm_campaign_cookie'];
}
else {
  if (isset($data['sepulsa_buy_now_button_utm_campaign']) &&
    !empty($data['sepulsa_buy_now_button_utm_campaign'])) {
    $array_query['utm_campaign'] = $data['sepulsa_buy_now_button_utm_campaign'];
  }
}

?>
<?php echo l(t('BELI PULSA SEKARANG'),
  '',
  array(
    'query' => $array_query,
    'attributes' => array('class' => 'bt_std bt_beli'),
  ))?>
