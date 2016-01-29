<?php
/**
 * @file
 * template.php
 *
 * @author dwi@sepulsa.com
 * @since July 20nd 2015
 */

/**
 * Implements hook_block_view_alter().
 */
function sepulsav2_block_view_alter(&$data, $block) {
  if ($block->region == 'sidebar_first' && !empty($data['content']['#theme_wrappers'])) {
    foreach ($data['content']['#theme_wrappers'] as $key => $theme_wrapper) {
      if (strpos($theme_wrapper, 'menu_tree') === 0) {
        $data['content']['#theme_wrappers'][$key] = 'menu_tree__sidebar';

        foreach (element_children($data['content']) as $children) {
          $data['content'][$children]['#theme'] = 'menu_link__sidebar';
        }
        break;
      }
    }
  }
}

/**
 * Implements hook_form_alter().
 */
function sepulsav2_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == "sepulsa_phone_form") {
    $form['phone']['#title'] = NULL;
    $form['phone']['#attributes']['class'] = array('input-text', 'full-width');
    $form['phone']['#attributes']['placeholder'] = t('Masukkan Nomor Handphone (mis. 081234567890)');
    $form['phone']['#suffix'] = '<p></p>';

    $form['operator']['#title'] = NULL;
    $form['operator']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Operator');
    $form['operator']['#suffix'] = '<p></p>';

    $form['card_type']['#title'] = NULL;
    $form['card_type']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Pilihan Kartu');

    $form['packet']['#title'] = NULL;
    $form['packet']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Pilihan Paket');

    $form['add']['#prefix'] = '<div class="topup-action">';
    $form['add']['cart']['#value'] = t('Add to cart', array(), array('context' => 'multipaid_product'));
    $form['add']['cart']['#attributes']['style'] = 'float:right';
    $form['add']['charge']['#attributes']['style'] = 'float:right;';
    $form['add']['#suffix'] = '</div>';

  } else if ($form_id == "user_login_block") {
    $form['name']['#title'] = NULL;
    $form['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    $form['name']['#suffix'] = '<p></p>';

    $form['pass']['#title'] = NULL;
    $form['pass']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Password');
    $form['pass']['#suffix'] = '<p></p>';

    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));
    $form['links']['#markup'] = l(t('Request New Password'), 'user/password');

  } else if ($form_id == "user_login") {
    $form['name']['#title'] = NULL;
    $form['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    $form['name']['#suffix'] = '<p></p>';

    $form['pass']['#title'] = NULL;
    $form['pass']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Password');
    $form['pass']['#suffix'] = '<p></p>';

    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));
    $form['links']['#markup'] = l(t('Request New Password'), 'user/password');

  } else if ($form_id == "user_register_form") {
    $form['account']['name']['#title'] = NULL;
    $form['account']['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Username');

    $form['account']['mail']['#title'] = NULL;
    $form['account']['mail']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    $form['account']['mail']['#suffix'] = '<p></p>';

    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));

  } else if ($form_id == "user_pass") {
    $form['name']['#title'] = NULL;
    $form['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    $form['name']['#suffix'] = '<p><br /></p>';

    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));

  } else if ($form_id == "commerce_checkout_form_checkout") {
    $form['#attributes']['class'][] = 'row';

    $form['cart_contents']['#title'] = NULL;

    if (!empty($form['commerce_coupon'])) {
      $form['commerce_coupon']['coupon_code']['#attributes']['class'][] = 'input-text';
      $form['commerce_coupon']['coupon_code']['#attributes']['class'][] = 'full-width';
      $form['commerce_coupon']['coupon_add']['#attributes']['class'][] = 'btn';
      $form['commerce_coupon']['coupon_add']['#attributes']['class'][] = 'btn-sm';
      $form['commerce_coupon']['coupon_add']['#attributes']['class'][] = 'style1';

      if (!empty($form_state['order']->commerce_coupons)) {
        $form['commerce_coupon']['coupon_code']['#access'] = FALSE;
        $form['commerce_coupon']['coupon_add']['#access'] = FALSE;
        $form['commerce_coupon']['redeemed_coupons']['#prefix'] = '<label style="margin-bottom:-10px">' . t('Coupon Code') . '</label>';
      }
    }

    if (!empty($form['account'])) {
      $form['account']['#title'] = NULL;
      $form['account']['login']['mail']['#title_display'] = 'invisible';
      $form['account']['login']['mail']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => t('Email Address'));
      $form['account']['login']['#prefix'] = '<div class="cart-collaterals box"> <h4><strong>'.t('Put Email Address').'</strong></h4>';
      $form['account']['login']['#suffix'] = '</div>';
    }

    $form['commerce_payment']['#title'] = NULL;
    $form['commerce_payment']['#prefix'] = '<div id="commerce-payment-ajax-wrapper">';
    $form['commerce_payment']['#prefix'] .= '<h4><strong>'.t('Payment Options').'</strong></h4>';
    $form['commerce_payment']['#suffix'] = '</div>';

    $form['commerce_payment']['payment_method']['#process'] = array('form_process_radios', 'sepulsav2_process_radios_payment_method');

    if (isset($form['commerce_payment']['payment_details']['veritrans'])) {
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['#prefix'] = '<div class="clearfix"></div>';

      # This Form attribute for Card Number
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['number']['#prefix'] = "<div class='form-item'><label>".t('Card Number')."</label>";
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['number']['#title_display'] = 'invisible';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['number']['#attributes']['class'] = array('input-text');
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['number']['#suffix'] = "</div>";

      # This Form attribute for Card Expire
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_month']['#prefix'] = '<div class="form-item"><label>'.t("Expiration").'</label>';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_month']['#attributes']['style'] = 'width:75px !important; margin-right:10px; display:inline';
      // $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_month']['#attributes']['class'] = array('selector');
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_month']['#title_display'] = 'invisible';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_year']['#attributes']['style'] = 'width:85px; display:inline';
      // $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_year']['#attributes']['class'] = array('selector');
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['exp_year']['#suffix'] = '</div>';

      # This Form attribute for Card CVV
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['code']['#title_display'] = 'invisible';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['code']['#prefix'] = "<div class='form-item'><label>".t('Card CVV')."</label>";
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['code']['#suffix'] = "</div>";
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['code']['#attributes']['class'] = array('input-text');
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['code']['#attributes']['style'] = "width:75px !important";

      if (isset($form['commerce_payment']['payment_details']['veritrans']['code2'])) {
        $form['commerce_payment']['payment_details']['veritrans']['code2']['#title_display'] = 'invisible';
        $form['commerce_payment']['payment_details']['veritrans']['code2']['#prefix'] = '<div class="form-item"><p></p> <label for="edit-commerce-payment-payment-details-veritrans-code2" style="display: block;"> <h6><strong>' . t('Card CVV') . '</strong></h6></label>';
        $form['commerce_payment']['payment_details']['veritrans']['code2']['#suffix'] = '</div>';
        $form['commerce_payment']['payment_details']['veritrans']['code2']['#attributes']['class'] = array('input-text');
        $form['commerce_payment']['payment_details']['veritrans']['code2']['#attributes']['style'] = "width:75px !important";
      }

      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone_other']['#title_display'] = 'invisible';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone_other']['#prefix'] = '<div class="form-item"><p></p>';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone_other']['#suffix'] = '</div>';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone_other']['#attributes']['style'] = 'width:150px !important; margin-left: 30%; margin-right:10px';
      $form['commerce_payment']['payment_details']['veritrans']['credit_card']['phone_other']['#attributes']['class'] = array('input-text');

      if (isset($form['commerce_payment']['payment_details']['veritrans']['credit_card']['save'])) {
        $form['commerce_payment']['payment_details']['veritrans']['credit_card']['save']['#title_display'] = 'invisible';
        $form['commerce_payment']['payment_details']['veritrans']['credit_card']['save']['#prefix'] = '<p></p><label><div class="checkbox">';
        $form['commerce_payment']['payment_details']['veritrans']['credit_card']['save']['#suffix'] = t('Save Credit Card').'</div></label>';
      }

      if (isset($form['commerce_payment']['payment_details']['veritrans']['tokens'])) {
        $form['commerce_payment']['payment_details']['veritrans']['tokens']['#title_display'] = 'invisible';
        $form['commerce_payment']['payment_details']['veritrans']['tokens']['#prefix'] = "<p></p><h6><strong>".t('Saved Credit Card')."</strong></h6>";
        // $form['commerce_payment']['payment_details']['veritrans']['tokens']['#attributes']['class'] = array('selector');
      }
    }
    elseif (isset($form['commerce_payment']['payment_details']['bank_details'])) {
      $settings = $form['commerce_payment']['payment_methods']['#value'][$form['commerce_payment']['payment_method']['#default_value']]['settings'];

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
    unset($form['#prefix']);
    unset($form['#suffix']);

    $views = $form_state['build_info']['args'][0];

    foreach (element_children($form['edit_delete']) as $children) {
      $form['edit_delete'][$children]['#value'] = '';

      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $views->result[$children]->_field_data['commerce_line_item_field_data_commerce_line_items_line_item_']['entity']);
      if ($line_item_wrapper->getBundle() == 'coupon') {
         $form['edit_delete'][$children]['#product_id'] = $line_item_wrapper->commerce_product->getIdentifier();
         $form['edit_delete'][$children]['#ajax'] = array(
           'callback' => 'sepulsav2_views_form_commerce_cart_block_default_ajax_submit',
           'progress' => array('type' => 'none'),
         );
      }
    }
  } else if ($form_id == "user_profile_form") {
    $form['#attributes']['class'][] = 'form';
    $form['#attributes']['class'][] = 'akun';

    foreach (element_children($form['account']) as $children) {
      if (isset($form['account'][$children]['#type']) && $form['account'][$children]['#type'] == 'textfield') {
        $form['account'][$children]['#prefix'] = '<div class="row">';
        $form['account'][$children]['#suffix'] = '</div>';
      }
    }

    $form['account']['pass']['#process'] = array('form_process_password_confirm', 'user_form_process_password_confirm', 'sepulsav2_form_process_password_confirm');

    foreach (element_children($form) as $children) {
      if (isset($form[$children]['#language'])) {
        $form[$children]['#access'] = FALSE;
      }
    }

    $form['contact']['#access'] = FALSE;
    $form['mimemail']['#access'] = FALSE;

    $form['actions']['#prefix'] = '<hr>';

    foreach (element_children($form['actions']) as $children) {
      $form['actions'][$children]['#attributes']['class'][] = 'bt_std';
      $form['actions'][$children]['#prefix'] = '<div class="row">';
      $form['actions'][$children]['#suffix'] = '</div>';
    }
  } else if ($form_id == "commerce_veritrans_user_token_form") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['delete']['#attributes'] = array('class' => array('btn', 'style1'));

  }
  elseif ($form_id == 'commerce_checkout_form_complete' && !empty($form_state['build_info']['args'][0]) && is_object($form_state['build_info']['args'][0])) {
    $form['checkout_completion_message']['message']['#theme'] = 'sepulsa_checkout_completion_message';
    $form['checkout_completion_message']['message']['#message'] = $form['checkout_completion_message']['message']['#markup'];
  }
}

/**
 * Implements hook_form_FORM_ID_alter() for views_form_commerce_cart_block_popup().
 */
function sepulsav2_form_views_form_commerce_cart_block_popup_alter(&$form, &$form_state, $form_id) {
  unset($form['#action']);
  $form['select_voucher'] = array(
    '#type' => 'checkbox',
    '#title' => t('I want to select voucher'),
    '#default_value' => TRUE,
    '#prefix' => '<div class="opsi-voucher">',
    '#suffix' => '</div>',
    '#weight' => $form['output']['#weight'] + 1,
  );

  $form['actions']['#weight'] = 100;
  $form['actions']['#attributes']['style'] = 'text-align:center';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Process'),
    '#submit' => array('sepulsav2_form_views_form_commerce_cart_block_popup_submit'),
  );
}

/**
 * Implements hook_form_BASE_FORM_ID_alter() for commerce_cart_add_to_cart_form().
 */
function sepulsav2_form_commerce_cart_add_to_cart_form_alter(&$form, &$form_state, $form_id) {
  // Check for charge button.
  if (isset($form['charge']) && !empty($form['charge'])) {
    $form['charge']['#attributes']['class'][] = 'btn';
    $form['charge']['#attributes']['class'][] = 'style1';
    $form['charge']['#attributes']['class'][] = 'pull-right';
    $form['charge']['#attributes']['style'][] = 'float:right;';
    $form['charge']['#weight'] = 4;
  }
  if (isset($form_state['line_item']) && !empty($form_state['line_item'])) {
    switch ($form_state['line_item']->type) {
      case 'coupon':
        $form['submit']['#ajax'] = array(
          'callback' => 'sepulsav2_commerce_add_to_cart_form_ajax_submit',
          'progress' => array('type' => 'none'),
        );
        break;

      case 'electricity_prepaid':
        $form['product_id']['#attributes']['class'][] = 'input-text';
        $form['product_id']['#attributes']['class'][] = 'full-width';
        $form['product_id']['#suffix'] = '<p></p>';

        $form['line_item_fields']['#weight'] = 0;

        $form['line_item_fields']['electricity_customer_number'][LANGUAGE_NONE][0]['value']['#title_display'] = 'invisible';
        $form['line_item_fields']['electricity_customer_number'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = $form['line_item_fields']['electricity_customer_number'][LANGUAGE_NONE][0]['value']['#title'];
        $form['line_item_fields']['electricity_customer_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'input-text';
        $form['line_item_fields']['electricity_customer_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'full-width';
        $form['line_item_fields']['electricity_customer_number'][LANGUAGE_NONE][0]['value']['#suffix'] = '<p></p>';

        $form['line_item_fields']['electricity_phone_number'][LANGUAGE_NONE][0]['value']['#title_display'] = 'invisible';
        $form['line_item_fields']['electricity_phone_number'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = $form['line_item_fields']['electricity_phone_number'][LANGUAGE_NONE][0]['value']['#title'];
        $form['line_item_fields']['electricity_phone_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'input-text';
        $form['line_item_fields']['electricity_phone_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'full-width';
        $form['line_item_fields']['electricity_phone_number'][LANGUAGE_NONE][0]['value']['#suffix'] = '<p></p>';


        $form['submit']['#value'] = t('Add to cart', array(), array('context' => 'multipaid_product'));
        $form['submit']['#attributes']['class'][] = 'btn';
        $form['submit']['#attributes']['class'][] = 'style1';
        $form['submit']['#attributes']['class'][] = 'pull-right';
        $form['submit']['#attributes']['style'][] = 'float:right;';

        $form['submit']['#weight'] = 3;
        $form['submit']['#states'] = array(
          'enabled' => array(
            ':input[name="line_item_fields[electricity_customer_number][' . LANGUAGE_NONE . '][0][value]"]' => array('empty' => FALSE),
            ':input[name="line_item_fields[electricity_phone_number][' . LANGUAGE_NONE . '][0][value]"]' => array('empty' => FALSE),
          ),
        );
        // Add charge state: if available.
        if (isset($form['charge']) && !empty($form['charge'])) {
          $form['charge']['#states'] = array(
            'enabled' => array(
              ':input[name="line_item_fields[electricity_customer_number][' . LANGUAGE_NONE . '][0][value]"]' => array('empty' => FALSE),
              ':input[name="line_item_fields[electricity_phone_number][' . LANGUAGE_NONE . '][0][value]"]' => array('empty' => FALSE),
            ),
          );
        }
        $form['#action'] .= '#pln';
        break;

      case 'biznet':
      case 'multifinance':
        $form['product_id']['#attributes']['class'][] = 'input-text';
        $form['product_id']['#attributes']['class'][] = 'full-width';
        $form['product_id']['#suffix'] = '<p></p>';

        $form['line_item_fields']['#weight'] = 0;

        $form['line_item_fields']['field_customer_number']['#weight'] = -10;
        $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#title_display'] = 'invisible';
        $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#title'];
        $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'input-text';
        $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'full-width';
        $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#suffix'] = '<p></p>';

        if (!empty($form['description'])) {
          $form['description']['#prefix'] = '<div style="border: 1px solid; padding: 1em; margin: 15px 0px; font-size: 1.2em; clear: both;">';
          $form['description']['#suffix'] = '</div>';
          $form['description']['#weight'] = 1;
        }

        $form['submit']['#value'] = t('Add to cart', array(), array('context' => 'multipaid_product'));
        $form['submit']['#attributes']['class'][] = 'btn';
        $form['submit']['#attributes']['class'][] = 'style1';
        $form['submit']['#attributes']['class'][] = 'pull-right';
        $form['submit']['#attributes']['style'][] = 'float:right;';
        $form['submit']['#weight'] = 3;

        $form['submit']['#states'] = array(
          'enabled' => array(
            'form#commerce-cart-add-to-cart-form-' . $form_state['line_item']->type . ' input[name="line_item_fields[field_customer_number][' . LANGUAGE_NONE . '][0][value]"]' => array('empty' => FALSE),
          ),
        );
        // Add charge state: if available.
        if (isset($form['charge']) && !empty($form['charge'])) {
          $form['charge']['#states'] = array(
            'enabled' => array(
              'form#commerce-cart-add-to-cart-form-' . $form_state['line_item']->type . ' input[name="line_item_fields[field_customer_number][' . LANGUAGE_NONE . '][0][value]"]' => array('empty' => FALSE),
            ),
          );
        }
        $form['#action'] .= '#' . $form_state['line_item']->type;
        break;

      case 'pln_prepaid':
        $form['product_id']['#attributes']['class'][] = 'input-text';
        $form['product_id']['#attributes']['class'][] = 'full-width';
        $form['product_id']['#suffix'] = '<p></p>';

        $form['line_item_fields']['#weight'] = 0;

        $form['line_item_fields']['field_customer_number']['#weight'] = -10;
        $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#title_display'] = 'invisible';
        $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#title'];
        $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'input-text';
        $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'full-width';
        $form['line_item_fields']['field_customer_number'][LANGUAGE_NONE][0]['value']['#suffix'] = '<p></p>';

        $form['line_item_fields']['field_phone_number']['#weight'] = -5;
        $form['line_item_fields']['field_phone_number'][LANGUAGE_NONE][0]['value']['#title_display'] = 'invisible';
        $form['line_item_fields']['field_phone_number'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = $form['line_item_fields']['field_phone_number'][LANGUAGE_NONE][0]['value']['#title'];
        $form['line_item_fields']['field_phone_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'input-text';
        $form['line_item_fields']['field_phone_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'full-width';
        $form['line_item_fields']['field_phone_number'][LANGUAGE_NONE][0]['value']['#suffix'] = '<p></p>';

        if (!empty($form['description'])) {
          $form['description']['#prefix'] = '<div style="border: 1px solid; padding: 1em; margin: 15px 0px; font-size: 1.2em; clear: both;">';
          $form['description']['#suffix'] = '</div>';
        }

        $form['add']['#prefix'] = '<div class="topup-action-2">';
        $form['submit']['#value'] = t('Add to cart', array(), array('context' => 'multipaid_product'));
        $form['submit']['#attributes']['class'][] = 'btn';
        $form['submit']['#attributes']['class'][] = 'style1';
        $form['submit']['#attributes']['class'][] = 'pull-right';
        $form['add']['#suffix'] = '</div>';

        $form['submit']['#states'] = array(
          'enabled' => array(
            '#' . $form['#id'] . ' :input[name="line_item_fields[field_customer_number][' . LANGUAGE_NONE . '][0][value]"]' => array('empty' => FALSE),
            '#' . $form['#id'] . ' :input[name="line_item_fields[field_phone_number][' . LANGUAGE_NONE . '][0][value]"]' => array('empty' => FALSE),
          ),
        );

        // Add charge state: if available.
        if (isset($form['charge']) && !empty($form['charge'])) {
          $form['charge']['#states'] = array(
            'enabled' => array(
              '#' . $form['#id'] . ' :input[name="line_item_fields[field_customer_number][' . LANGUAGE_NONE . '][0][value]"]' => array('empty' => FALSE),
              '#' . $form['#id'] . ' :input[name="line_item_fields[field_phone_number][' . LANGUAGE_NONE . '][0][value]"]' => array('empty' => FALSE),
            ),
          );
        }
        $form['#action'] .= '#pln';
        break;
    }
  }
}

/**
 * Implements hook_form_BASE_FORM_ID_alter() for webform_client_form().
 */
function sepulsav2_form_webform_client_form_alter(&$form, &$form_state, $form_id) {
  if ($form_state['build_info']['args'][0]->title == 'Konfirmasi Pembayaran') {
    $form['#attributes']['class'][] = 'text-center';
    $form['actions']['submit']['#attributes']['class'][] = 'btn';
    $form['actions']['submit']['#attributes']['class'][] = 'style1';
  }
}

/**
 * Implements hook_commerce_coupon_add_coupon_ajax_alter().
 */
function sepulsav2_commerce_coupon_add_coupon_ajax_alter(&$commands, $form, &$form_state) {
  commerce_cart_order_refresh($form_state['order']);
  $rebuild = drupal_rebuild_form($form_state['build_info']['form_id'], $form_state, $form);
  $commerce_payment = $rebuild['commerce_payment'];

  if (!in_array($form_state['input']['commerce_payment']['payment_method'], array_keys($commerce_payment['payment_method']['#options']))) {
    $payment_method = reset(array_keys($commerce_payment['payment_method']['#options']));
    $form_state['input']['commerce_payment']['payment_method'] = $payment_method;
    $form_state['values']['commerce_payment']['payment_method'] = $payment_method;
  }
  $commands[] = ajax_command_replace('#commerce-payment-ajax-wrapper', drupal_render($commerce_payment));
}

/**
 * Implementation of expand_password_confirm.
 */
function sepulsav2_form_process_password_confirm($element) {
  $element['pass1']['#prefix'] = '<div class="row">';
  $element['pass1']['#suffix'] = '</div>';
  $element['pass2']['#prefix'] = '<div class="row">';
  $element['pass2']['#suffix'] = '</div>';
  unset($element['#attached']['js']);

  return $element;
}

function sepulsav2_menu_link__sidebar(array $variables) {
  $element = $variables ['element'];
  $sub_menu = '';

  if ($element ['#below']) {
    $sub_menu = drupal_render($element ['#below']);
  }
  return l($element ['#title'], $element ['#href'], $element ['#localized_options']);
}

function sepulsav2_menu_local_tasks(&$variables) {
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

function sepulsav2_menu_tree__sidebar($variables) {
  return '<nav>' . $variables['tree'] . '</nav>';
}

/**
 * Implements hook_preprocess_menu_link().
 */
function sepulsav2_preprocess_menu_link(&$variables) {
  if ($variables['element']['#original_link']['in_active_trail']) {
    $variables['element']['#attributes']['class'][] = 'active';
  }
}

/**
 * Implements hook_preprocess_html().
 */
function sepulsav2_preprocess_html(&$variables) {
  if (drupal_is_front_page()) {
    $variables['classes_array'][] = 'home';
  }

  if (isset($variables['page']['shopping_cart'])) {
    $variables['shopping_cart'] = $variables['page']['shopping_cart'];
    unset($variables['page']['shopping_cart']);
  }

  $variables['footer_sub_left'] = block_get_blocks_by_region('footer_sub_left');
  $variables['footer_sub_right'] = block_get_blocks_by_region('footer_sub_right');
  $variables['footer_second'] = block_get_blocks_by_region('footer_second');
  if ($variables['user']->uid > 0) {
    module_load_include('module', 'userpoints', 'userpoints');
    $variables['userpoints'] = userpoints_get_current_points($variables['user']->uid);
  }
}

/**
 * Implements hook_preprocess_page().
 */
function sepulsav2_preprocess_page(&$variables) {
  if (drupal_is_front_page()) {
    if (isset($_POST['form_id']) && $_POST['form_id'] == 'commerce_cart_add_to_cart_form') {
      $variables['active_tab'] = 'token_reload';
    }
    else {
      $variables['active_tab'] = 'topup';
    }
  }
}

/**
 * Implements hook_preprocess_region().
 */
function sepulsav2_preprocess_region(&$variables) {
  switch ($variables['region']) {
    case 'shopping_cart':
      $variables['classes_array'][] = 'box';
      $variables['classes_array'][] = 'lang';
      $variables['classes_array'][] = 'cart';
      break;
  }
}

/**
 * Implements hook_preprocess_block().
 */
function sepulsav2_preprocess_block(&$variables) {
  foreach ($variables['classes_array'] as $key => $value) {
    if ($value == 'block') {
      $variables['classes_array'][$key] = NULL;
    }
  }
}

/**
 * Implements hook_preprocess_views_view_unformatted().
 */
function sepulsav2_preprocess_views_view_unformatted(&$variables) {
  $view = $variables['view'];

  switch ($view->name) {
    case 'coupon':
      if ($view->current_display == 'page') {
        $variables['row_classes'] = array();
        $products = array();
        $cart = commerce_cart_order_load($variables['user']->uid);

        if (!empty($cart)) {
          $cart_wrapper = entity_metadata_wrapper('commerce_order', $cart);

          foreach ($cart_wrapper->commerce_line_items as $line_item) {
            if ($line_item->getBundle() == 'coupon') {
              $products[] = $line_item->commerce_product->getIdentifier();
            }
          }

          foreach ($view->result as $key => $result) {
            $product_id = $result->field_field_product[0]['raw']['product_id'];
            if (in_array($product_id, $products)) {
              $variables['row_classes'][$key] = 'hidden coupon-' . $product_id;
            }
          }
        }
      }
      break;
  }
}

/**
 * Implements hook_preprocess_views_view_list().
 */
function sepulsav2_preprocess_views_view_list(&$variables) {
  $view = $variables['view'];

  switch ($view->name) {
    case 'commerce_cart_block':
      foreach ($view->result as $id => $result) {
        $variables['classes_array'][$id] = 'line-item-' . $result->commerce_line_item_field_data_commerce_line_items_line_item_;
      }
      break;
  }
}

/**
 * Implements hook_preprocess_views_view_table().
 */
function sepulsav2_preprocess_views_view_table(&$variables) {
  $view = $variables['view'];

  switch ($view->name) {
    case 'commerce_cart_block':
      if (!empty($view->args)) {
        $order = commerce_order_load(reset($view->args));
      }
      else {
        global $user;

        $order = commerce_cart_order_load($user->uid);
      }

      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      $line_items = $order_wrapper->commerce_line_items;
      $variables['order_total'] = commerce_line_items_total($line_items);
      break;
  }
}

/**
 * Implements hook_preprocess_sepulsa_checkout_completion_message().
 */
function sepulsav2_preprocess_sepulsa_checkout_completion_message(&$variables) {
  $variables['user'] = $GLOBALS['user'];
  if ($variables['user']->uid) {
    $variables['authenticated'] = TRUE;
  }
}

function sepulsav2_form_element($variables) {
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
 * Overrides theme_status_messages().
 */
function sepulsav2_status_messages($variables) {
  $display = $variables['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
    'info' => t('Informative message'),
  );

  // Map Drupal message types to their corresponding Bootstrap classes.
  // @see http://twitter.github.com/bootstrap/components.html#alerts
  $status_class = array(
    'status' => 'success',
    'error' => 'error',
    'warning' => 'notice',
    // Not supported, but in theory a module could send any type of message.
    // @see drupal_set_message()
    // @see theme_status_messages()
    'info' => 'help',
  );

  foreach (drupal_get_messages($display) as $type => $messages) {
    $class = (isset($status_class[$type])) ? ' alert-' . $status_class[$type] : '';
    $output .= "<div class=\"alert alert-block$class\">\n";
    // $output .= "  <a class=\"close\" data-dismiss=\"alert\" href=\"#\">&times;</a>\n";
    $output .= "  <a class=\"close\" data-dismiss=\"alert\" href=\"#\"></a>\n";

    if (!empty($status_heading[$type])) {
      $output .= '<h4 class="element-invisible">' . $status_heading[$type] . "</h4>\n";
    }

    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= $messages[0];
    }

    $output .= "</div>\n";
  }
  return $output;
}

/**
 * Implements hook_theme().
 */
function sepulsav2_theme($existing, $type, $theme, $path) {
  return array(
    'commerce_checkout_form_checkout' => array(
      'render element' => 'form',
      'path' => $path . '/templates/checkout',
      'template' => 'commerce-checkout-form-checkout',
    ),
    'sepulsa_checkout_completion_message' => array(
      'variables' => array(
        'message' => NULL,
      ),
      'path' => $path . '/templates/checkout',
      'template' => 'sepulsa-checkout-completion-message',
    ),
    'sepulsav2_referral' => array(
      'variables' => array(
        'link' => NULL,
      ),
      'path' => $path . '/templates',
      'template' => 'sepulsav2-referral',
    ),
  );
}

function sepulsav2_form_views_form_commerce_cart_block_popup_submit($form, &$form_state) {
  if ($form_state['values']['select_voucher']) {
    $form_state['redirect'] = 'coupon';
  }
  else {
    $form_state['redirect'] = array('checkout/' . $form_state['order']->order_id, array('query' => array('fast_charge' => 1)));
  }
}

function sepulsav2_commerce_add_to_cart_form_ajax_submit($form, $form_state) {
  if (!function_exists('commerce_order_rules_contains_product')) {
    module_load_include('rules.inc', 'commerce_order');
  }

  $commands = array();
  $order = commerce_cart_order_load($GLOBALS['user']->uid);
  $product_id = $form_state['default_product']->product_id;
  $rebuild = drupal_rebuild_form($form_state['build_info']['form_id'], $form_state, $form);

  if (commerce_order_rules_contains_product($order, $form_state['default_product']->sku, '>', 0)) {
    $node_id = $form_state['context']['entity_id'];

    $variables = array(
      'order' => $order,
      'contents_view' => commerce_embed_view('commerce_cart_block', 'default', array($order->order_id), 'coupon'),
    );

    $commands[] = array(
      'command' => 'select_coupon',
      'selector' => '#node-' . $node_id,
    );
    $commands[] = ajax_command_invoke('#node-' . $node_id, 'addClass', array('hidden coupon-' . $product_id));
    $commands[] = ajax_command_replace('.cart-contents', theme('commerce_cart_block', $variables));
  }

  $commands[] = ajax_command_replace('.commerce-cart-add-to-cart-form-' . $product_id, render($rebuild));
  $commands[] = ajax_command_prepend('.region-content', theme('status_messages'));

  return array('#type' => 'ajax', '#commands' => $commands);
}

function sepulsav2_views_form_commerce_cart_block_default_ajax_submit($form, $form_state) {
  $product_id = $form_state['triggering_element']['#product_id'];

  $commands = array();
  $commands[] = ajax_command_remove('.line-item-' . $form_state['triggering_element']['#line_item_id']);
  $commands[] = ajax_command_invoke('.hidden.coupon-' . $product_id, 'removeClass', array('hidden coupon-' . $product_id));

  drupal_get_messages('status');

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Implements hook_user_view_alter().
 */
function sepulsav2_user_view_alter(&$build) {
  module_load_include('inc', 'user', 'user.pages');

  $build['Referrals']['#access'] = FALSE;
  $build['summary']['#access'] = FALSE;
  $build['simplenews']['#access'] = FALSE;

  $build['referrals'] = array(
    '#theme' => 'sepulsav2_referral',
    '#link' => url($build['#account']->referral_link, array('absolute' => TRUE)),
    '#suffix' => '<hr>',
    '#weight' => 1,
    '#attached' => array(
      'js' => array(
        path_to_theme() . '/js/zclip/jquery.zclip.js' => array(
          'scope' => 'footer',
        ),
        array(
          'type' => 'setting',
          'data' => array(
            'zclip' => array(
              'path' => url(path_to_theme() . '/js/zclip/ZeroClipboard.swf'),
              'selector' => '#copy-link-wrap',
              'target' => '#toclip',
              'message' => 'Your Referral link has been copied',
            ),
          ),
        ),
      ),
    ),
  );

  $build['form'] = drupal_get_form('user_profile_form', $build['#account']);
  $build['form']['#weight'] = 2;
}

function sepulsav2_process_radios_payment_method($element) {
  foreach ($element['#options'] as $key => $choice) {
    $element[$key]['#prefix'] = '<div class="payment">';
    $element[$key]['#suffix'] = '</div>';
  }

  return $element;
}
