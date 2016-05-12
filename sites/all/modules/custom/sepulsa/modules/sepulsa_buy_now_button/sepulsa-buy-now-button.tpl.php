<?php
/**
 * @file
 * Sepulsa Buy Now Button TPL.
 */
$utm_link = '';
if (isset($_COOKIE['utm_source_cookie']) && !empty($_COOKIE['utm_source_cookie'])) {
  $utm_link .= 'utm_source=' . $_COOKIE['utm_source_cookie'];
}
else {
  if (isset($data['sepulsa_buy_now_button_utm_source']) && !empty($data['sepulsa_buy_now_button_utm_source'])) {
    $utm_link .= 'utm_source=' . $data['sepulsa_buy_now_button_utm_source'];
  }
}

if (isset($_COOKIE['utm_medium_cookie']) && !empty($_COOKIE['utm_medium_cookie'])) {
  if (!empty($utm_link)) {
    $utm_link .= '&';
  }
  $utm_link .= 'utm_medium=' . $_COOKIE['utm_medium_cookie'];
}
else {
  if (isset($data['sepulsa_buy_now_button_utm_medium']) && !empty($data['sepulsa_buy_now_button_utm_medium'])) {
    if (!empty($utm_link)) {
      $utm_link .= '&';
    }
    $utm_link .= 'utm_medium=' . $data['sepulsa_buy_now_button_utm_medium'];
  }
}

if (isset($_COOKIE['utm_campaign_cookie']) && !empty($_COOKIE['utm_campaign_cookie'])) {
  if (!empty($utm_link)) {
    $utm_link .= '&';
  }
  $utm_link .= 'utm_campaign=' . $_COOKIE['utm_campaign_cookie'];
}
else {
  if (isset($data['sepulsa_buy_now_button_utm_campaign']) && !empty($data['sepulsa_buy_now_button_utm_campaign'])) {
    if (!empty($utm_link)) {
      $utm_link .= '&';
    }
    $utm_link .= 'utm_campaign=' . $data['sepulsa_buy_now_button_utm_campaign'];
  }
}

if (empty($utm_link)) {
  $utm_link = '/';
}
else {
  $utm_link .= '/?' . $utm_link;
}
?>
<a href="<?php echo $utm_link ?>" class="bt_std bt_beli">
  <?php echo t('BELI PULSA SEKARANG')?>
</a>
