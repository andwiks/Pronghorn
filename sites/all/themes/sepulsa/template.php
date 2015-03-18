<?php
/**
 * @file
 * template.php
 *
 * @author ananto@sepulsa.com
 * @since February 2nd 2015
 */

/**
 * Implements hook_block_view_alter().
 */
function sepulsa_block_view_alter(&$data, $block) {
  if ($block->region == 'sidebar_first' && !empty($data['content']['#theme_wrappers'])) {
    foreach ($data['content']['#theme_wrappers'] as $key => $theme_wrapper) {
      if (strpos($theme_wrapper, 'menu_tree') === 0) {
        $data['content']['#theme_wrappers'][] = 'menu_tree__sidebar';
        break;
      }
    }
  }
}

function sepulsa_form_alter(&$form, &$form_state, $form_id) {
  //drupal_set_message("<pre>".print_r($form_id, true)."</pre>");
  $commer_form_id = substr($form_id, 0, 25);
  if ($form_id == "sepulsa_phone_form") {
    //drupal_set_message("<pre>".print_r($form['add'], true)."</pre>");
    $form['phone']['#title'] = NULL;
    $form['phone']['#attributes']['class'] = array('input-text', 'full-width');
    $form['phone']['#attributes']['placeholder'] = t('Masukkan Nomor Handphone (mis. 081234567890)');
    $form['phone']['#suffix'] = '<p></p>';

    $form['operator']['#title'] = NULL;
    $form['operator']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Operator');
    $form['operator']['#suffix'] = '<p></p>';

    $form['card_type']['#title'] = NULL;
    $form['card_type']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Pilihan Kartu');
    //$form['card_type']['#suffix'] = '<p></p>';

    $form['packet']['#title'] = NULL;
    $form['packet']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Pilihan Paket');

    $form['add']['#prefix'] = '<p></p>';
    $form['add']['cart']['#attributes']['style'] = 'float:right';
    $form['add']['charge']['#attributes']['style'] = 'float:right;display:none;';

  } else if ($form_id == "user_login_block") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['name']['#title'] = NULL;
    $form['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    $form['name']['#suffix'] = '<p></p>';

    $form['pass']['#title'] = NULL;
    $form['pass']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Password');
    $form['pass']['#suffix'] = '<p></p>';

    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));
    $form['links']['#markup'] = l(t('Request New Password'), 'user/password');

  } else if ($commer_form_id == 'commerce_cart_add_to_cart' && arg(0) == 'coupon') {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['submit']['#attributes'] = array('class' => array('btn', 'btn-sm', 'style3', 'post-read-more'));

  } else if ($form_id == "user_login") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['name']['#title'] = NULL;
    $form['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    $form['name']['#suffix'] = '<p></p>';

    $form['pass']['#title'] = NULL;
    $form['pass']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Password');
    $form['pass']['#suffix'] = '<p></p>';

    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));
    $form['links']['#markup'] = l(t('Request New Password'), 'user/password');

  } else if ($form_id == "user_register_form") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['account']['name']['#title'] = NULL;
    $form['account']['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Username');

    $form['account']['mail']['#title'] = NULL;
    $form['account']['mail']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    $form['account']['mail']['#suffix'] = '<p></p>';

    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));

  } else if ($form_id == "user_pass") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['name']['#title'] = NULL;
    $form['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    $form['name']['#suffix'] = '<p><br /></p>';

    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));

  } else if ($form_id == "commerce_checkout_form_checkout") {
    global $user;

    //drupal_set_message("<pre>".print_r($form['commerce_payment'], true)."</pre>");
    $form['cart_contents']['#title'] = NULL;

    $form['account']['#title'] = NULL;
    if ($user->uid == 0) {
      $form['account']['login']['mail']['#title'] = NULL;
      $form['account']['login']['mail']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => t('Email Address'));
      $form['account']['login']['#prefix'] = '<div class="cart-collaterals row col-sm-6 col-md-6 box"> <h4><strong>'.t('Put Email Address').'</strong></h4>';
      $form['account']['login']['#suffix'] = '</div>';
    }

    $form['commerce_payment']['#title'] = NULL;
    $form['commerce_payment']['#prefix'] = '<p></p><h4><strong>'.t('Payment Options').'</strong></h4>';

    if (isset($form['commerce_payment']['payment_details']['veritrans'])) {
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['number']['#attributes']['class'] = array('input-text');
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['number']['#title'] = NULL;
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['number']['#prefix'] = "<p></p><h6><strong>".t('Card Number')."</strong></h6>";
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['number']['#suffix'] = "<p></p><h6><strong>".t('Expiration')."</strong></h6>";

      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_month']['#attributes']['style'] = 'width:75px !important; margin-right:10px; display:inline';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_month']['#attributes']['class'] = array('selector');
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_month']['#title'] = NULL;
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_month']['#suffix'] = NULL;
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_year']['#attributes']['style'] = 'width:85px; display:inline';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_year']['#attributes']['class'] = array('selector');

      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['code']['#title'] = NULL;
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['code']['#prefix'] = "<p></p><h6><strong>".t('Card CVV')."</strong></h6>";
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['code']['#attributes']['class'] = array('input-text');
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['code']['#attributes']['style'] = "width:75px !important";

      if (isset($form['commerce_payment']['payment_details']['veritrans']['code2'])) {
        $form['commerce_payment']['payment_details']['veritrans']['code2']['#title'] = NULL;
        $form['commerce_payment']['payment_details']['veritrans']['code2']['#prefix'] = "<p></p> <label for='edit-commerce-payment-payment-details-veritrans-code2' style='display: block;'> <h6><strong>".t('Card CVV')."</strong></h6></label>";
        $form['commerce_payment']['payment_details']['veritrans']['code2']['#attributes']['class'] = array('input-text');
        $form['commerce_payment']['payment_details']['veritrans']['code2']['#attributes']['style'] = "width:75px !important";
      }

      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone']['#title'] = NULL;
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone']['#prefix'] = "<p></p><h6><strong>".t('Phone Number')."</strong></h6>";
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone']['#attributes']['style'] = 'width:190px !important; margin-right:10px';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone']['#attributes']['class'] = array('selector');

      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone_other']['#title'] = NULL;
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone_other']['#prefix'] = "<p></p>";
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone_other']['#attributes']['style'] = 'width:150px !important; margin-right:10px';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone_other']['#attributes']['class'] = array('input-text');

      if (isset($form['commerce_payment']['payment_details']['veritrans']['credit_card']['save'])) {
        $form['commerce_payment']['payment_details']['veritrans']['credit_card']['save']['#title'] = NULL;
        $form['commerce_payment']['payment_details']['veritrans']['credit_card']['save']['#prefix'] = '<p></p><label><div class="checkbox">';
        $form['commerce_payment']['payment_details']['veritrans']['credit_card']['save']['#suffix'] = t('Save Credit Card').'</div></label>';
      }

      if (isset($form['commerce_payment']['payment_details']['veritrans']['tokens'])) {
        $form['commerce_payment']['payment_details']['veritrans']['tokens']['#title'] = NULL;
        $form['commerce_payment']['payment_details']['veritrans']['tokens']['#prefix'] = "<p></p><h6><strong>".t('Saved Credit Card')."</strong></h6>";
        $form['commerce_payment']['payment_details']['veritrans']['tokens']['#attributes']['style'] = 'width:250px !important; margin-right:10px';
        $form['commerce_payment']['payment_details']['veritrans']['tokens']['#attributes']['class'] = array('selector');
      }
    }
    elseif (isset($form['commerce_payment']['payment_details']['bank_details'])) {
      $settings = $form['commerce_payment']['payment_methods']['#value']['bank_transfer|commerce_payment_bank_transfer']['settings'];

      $form['commerce_payment']['payment_details']['bank_details'] = array();
      $form['commerce_payment']['payment_details']['bank_details']['#prefix'] = '<p></p><p><strong>' . t('Please make payment to:') . '</strong>';
      $form['commerce_payment']['payment_details']['bank_details']['#suffix'] = '</p>';
      $form['commerce_payment']['payment_details']['bank_details']['account_bank'] = array(
        '#markup' => '<br>' . t('Banking institution') . ' : <strong>' . $settings['details']['account_bank'] . '</strong>',
      );
      $form['commerce_payment']['payment_details']['bank_details']['account_number'] = array(
        '#markup' => '<br>' . t('Account number') . ' : <strong>' . $settings['details']['account_number'] . ' </strong>',
      );
      $form['commerce_payment']['payment_details']['bank_details']['account_owner'] = array(
        '#markup' => '<br>' . t('Account owner') . ' : <strong>' . $settings['details']['account_owner'] . ' </strong>',
      );
      $form['commerce_payment']['payment_details']['bank_details']['account_branch'] = array(
        '#markup' => '<br>' . t('Branch office') . ' : ' . $settings['details']['account_branch'],
      );

      if (!empty($settings['policy'])) {
        $form['commerce_payment']['payment_details']['policy'] = array(
          '#markup' => '<p>' . $settings['policy'] . '</p>',
        );
      }
    }

    $form['buttons']['continue']['#attributes']['class'] = array('btn', 'style1');
    $form['buttons']['continue']['#prefix'] = '<br />';

  } else if ($form_id == "views_form_commerce_cart_block_default") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    global $base_url;

    foreach ($form['edit_delete'] as &$value) {
      if (isset($value['#type'])) {
        //drupal_set_message("<pre>".print_r($value, true)."</pre>");
        //$value['#type'] = 'image_button';
        //$value['#attributes']['src'] = $base_url.'/'.drupal_get_path('theme','sepulsa').'/images/ico-close.png';
        //$value['#attributes']['width'] = '40px';
        $value['#attributes']['class'][] = 'hapusButton';
        $value['#value'] = '';
      }
    }

  } else if ($form_id == "user_profile_form") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['contact']['#attributes']['style'] = "display:none;";
    $form['mimemail']['#attributes']['style'] = "display:none;";

    $form['account']['pass']['#process'] = array('form_process_password_confirm', 'sepulsa_form_process_password_confirm', 'user_form_process_password_confirm');

    $form['account']['mail']['#attributes']['class'] = array('input-text');
    $form['account']['mail']['#suffix'] = '<p></p>';

    $form['account']['current_pass']['#attributes']['class'] = array('input-text');
    $form['account']['current_pass']['#description'] = t('Enter your current password to change the E-mail address or Password.');
    $form['account']['current_pass']['#suffix'] = '<p></p>';

    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));

  } else if ($form_id == "commerce_veritrans_user_token_form") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['delete']['#attributes'] = array('class' => array('btn', 'style1'));

  }
  elseif ($form_id == 'commerce_checkout_form_complete' && !empty($form_state['build_info']['args'][0]) && is_object($form_state['build_info']['args'][0])) {
    $order = $form_state['build_info']['args'][0];
    if (!empty($order->data['payment_method'])) {
      $payment_method = explode('|', $order->data['payment_method']);
      $payment_method = reset($payment_method);
      $form['checkout_completion_message']['message'] = array(
        '#theme' => 'sepulsa_checkout_completion_message',
        '#order' => $order,
        '#payment_method' => $payment_method,
      );

      if ($payment_method == 'bank_transfer') {
        $method_instance = commerce_payment_method_instance_load($order->data['payment_method']);
        $form['checkout_completion_message']['message']['#payment_settings'] = $method_instance['settings'];
      }
    }
  }
}

/**
 * Implements hook_form_BASE_FORM_ID_alter() for webform_client_form().
 */
function sepulsa_form_webform_client_form_alter(&$form, &$form_state, $form_id) {
  if ($form_state['build_info']['args'][0]->title == 'Konfirmasi Pembayaran') {
    $form['#attributes']['class'][] = 'text-center';
    $form['actions']['submit']['#attributes']['class'][] = 'btn';
    $form['actions']['submit']['#attributes']['class'][] = 'style1';
  }
}

/**
 * Implementation of expand_password_confirm.
 */
function sepulsa_form_process_password_confirm($element) {
  //drupal_set_message("<pre>".print_r($element, true)."</pre>");
  $element['pass1']['#attributes']['class'] = array('input-text');
  $element['pass1']['#suffix'] = '<p></p>';

  $element['pass2']['#attributes']['class'] = array('input-text');
  //$element['pass2']['#suffix'] = '<p></p>';

  return $element;
}

function sepulsa_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

function sepulsa_menu_tree__sidebar($variables) {
  return '<ul class="arrow-circle hover-effect filter-options">' . $variables['tree'] . '</ul>';
}

function sepulsa_preprocess_block(&$vars, $hook) {
  //drupal_set_message("<pre>".print_r($vars, true)."</pre>");
  foreach ($vars['classes_array'] as $key => $value) {
    if ($value == 'block') {
      $vars['classes_array'][$key] = NULL;
    }
  }
}

/**
 * Implements hook_preprocess_menu_link ().
 */
function sepulsa_preprocess_menu_link (&$variables) {
  if ($variables['element']['#original_link']['in_active_trail']) {
    $variables['element']['#attributes']['class'][] = 'active';
  }
}

function sepulsa_form_element($variables) {
  $element = &$variables['element'];

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
  );

  // Add element #id for #type 'item'.
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  // Add element's #type and #name as class to aid with JS/CSS selectors.
  $attributes['class'] = array('form-item');
  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'], array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
  }
  // Add a class for disabled elements to facilitate cross-browser styling.
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }
  //$output = '<div' . drupal_attributes($attributes) . '>' . "\n";
  $output = '';

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>' : '';

  switch ($element['#title_display']) {
    case 'before':
    case 'invisible':
      $output .= ' ' . theme('form_element_label', $variables);
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;

    case 'after':
      $output .= ' ' . $prefix . $element['#children'] . $suffix;
      $output .= ' ' . theme('form_element_label', $variables) . "\n";
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;
  }

  if (!empty($element['#description'])) {
    $output .= '<div class="description">' . $element['#description'] . "</div>\n";
  }

  //$output .= "</div>\n";

  return $output;
}

/**
 * Implements hook_theme().
 */
function sepulsa_theme($existing, $type, $theme, $path) {
  return array(
    'sepulsa_checkout_completion_message' => array(
      'variables' => array(
        'user' => NULL,
        'order' => NULL,
        'payment_method' => NULL,
        'payment_settings' => NULL,
        'authenticated' => FALSE,
      ),
      'path' => $path . '/templates/checkout',
      'template' => 'sepulsa-checkout-completion-message',
    ),
  );
}

/**
 * Implements hook_preprocess_sepulsa_checkout_completion_message().
 */
function sepulsa_preprocess_sepulsa_checkout_completion_message(&$variables) {
  $variables['user'] = $GLOBALS['user'];
  if ($variables['user']->uid) {
    $variables['authenticated'] = TRUE;
  }
}
