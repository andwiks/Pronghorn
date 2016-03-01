<?php
/**
 * @file
 * facebook_pixel.rules.inc
 */

/**
 * Impelements hook_rules_action_info.
 */
function facebook_pixel_rules_action_info() {
  return array(
    'facebook_pixel_viewcontent' => array(
      'label' => t('Facebook Pixel'),
      'group' => t('Facebook Pixel'),
      'parameter' => array(
        'type' => array(
          'type' => 'token',
          'label' => t('Facebook Pixel type'),
          'options list' => 'facebook_pixel_type',
          'default value' => 'ViewContent',
          'optional' => TRUE,
        ),
      ),
      'base' => 'facebook_pixel_viewcontent',
      'access callback' => 'rules_system_integration_access',
    ),
  );
}

/**
 * Fucntion facebook_pixel_viewcontent send data type ViewContent to facebook.
 */
function facebook_pixel_viewcontent($type) {
  if ($type != 'Purchase') {
    drupal_add_js("jQuery(document).ready(function () { jQuery(document).ready(function () { !function(f,b,e,v,n,t,s){if(f.fbq)return;n=f.fbq=function(){n.callMethod?
                        n.callMethod.apply(n,arguments):n.queue.push(arguments)};if(!f._fbq)f._fbq=n;
                        n.push=n;n.loaded=!0;n.version='2.0';n.queue=[];t=b.createElement(e);t.async=!0;
                        t.src=v;s=b.getElementsByTagName(e)[0];s.parentNode.insertBefore(t,s)}(window,
                        document,'script','//connect.facebook.net/en_US/fbevents.js');

                        fbq('init', '" . variable_get('facebook_pixel_id', '1198972743465542') . "');
                        fbq('track', '" . $type . "'); }); });", "inline");
  }
  else {
    $_SESSION['facebook_pixel_order_id'] = 'ada';
  }
}

/**
 * Options list facebook pixel.
 */
function facebook_pixel_type() {
  return array(
    'ViewContent' => t('View Content'),
    'Search' => t('Search'),
    'AddToCart' => t('Add To Cart'),
    'AddToWishlist' => t('Add To Wish List'),
    'InitiateCheckout' => t('Initiate Checkout'),
    'Purchase' => t('Purchase'),
    'AddPaymentInfo' => t('Add Payment Info'),
    'Lead' => t('Lead'),
    'CompleteRegistration' => t('Complete Registration'),
  );
}
