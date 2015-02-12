<?php
/**
 * @file
 * sepulsa.module
 *
 * @author andre4s_y
 *
 * @todo: add sepulsa.install: add taxonomy mobile operator.
 */

/**
 * Callback Functions.
 */

/**
 * Function sepulsa_autocomplete().
 */
function sepulsa_autocomplete() {
  // Get autocomplete parameter.
  $type = arg(2);
  $string = arg(3);
  // Get autocomplete callback by type.
  switch ($type) {
    case 'phone':
      return sepulsa_autocomplete_phone_number($string);

    default:
      drupal_access_denied();
  }
}

/**
 * Function sepulsa_front_page().
 */
function sepulsa_front_page() {
  // Get sepulsa phone form.
  $form = drupal_get_form('sepulsa_phone_form');
  // Return sepulsa phone form.
  return $form;
}

/**
 * Function sepulsa_phone_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function sepulsa_phone_form($form, &$form_state) {
  // Get sepulsa operator data.
  $operator_data = sepulsa_get_all_operator_data();
  // Add sepulsa autocomplete form js.
  $form['#attached']['js'][] = array(
    'data' => drupal_get_path('module', 'sepulsa') . '/sepulsa.form.js',
    'type' => 'file',
  );
  $form['#attached']['js'][] = array(
    'data' => array('sepulsa' => $operator_data),
    'type' => 'setting',
  );

  // Define default operator options.
  $operator_options = array(
    0 => t('Select Operator'),
  );
  // Define default card type: empty value.
  $card_type_options = array(
    0 => t('Select Card Type'),
  );
  // Define default packet options.
  $packet_options = array();

  // Check previous form state.
  $phone = (isset($form_state['values']['phone']) && !empty($form_state['values']['phone'])) ? $form_state['values']['phone'] : '';
  // Autocorrect phone prefix.
  $phone = sepulsa_autocorrect_mobile_prefix($phone);

  // Show input phone number.
  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#required' => TRUE,
    '#default_value' => (!empty($phone)) ? $phone : '',
    '#autocomplete_path' => 'sepulsa/autocomplete/phone',
    '#attributes' => array(
      'pattern' => '[0-9]*',
    ),
  );

  // Add operator options with operator data from phone prefix.
  foreach ($operator_data as $data) {
    $operator_options[$data['operator']] = $data['operator'];
    // Check whether phone number is available.
    if (!empty($phone)) {
      // Define default operator value.
      $prefix_key = array_search(substr($phone, 0, 4), $data['prefix']);
      if ($prefix_key !== FALSE) {
        $operator = $data['prefix'][$prefix_key];
        $card_type_options = $data['cardtype'];
        if (isset($form_state['values']['card_type']) && isset($card_type_options[$form_state['values']['card_type']])) {
          $packet_options = $data['packet'][$form_state['values']['card_type']];
          sort($packet_options);
          $packet = key($packet_options);
        }
      }
    }
  }

  // Define operator dropdown select.
  $form['operator'] = array(
    '#type' => 'select',
    '#title' => t('Operator'),
    '#required' => TRUE,
    '#default_value' => (isset($operator)) ? $operator : 0,
    '#options' => $operator_options,
    '#disabled' => TRUE,
  );

  $card_type_display = (count($card_type_options) > 1) ? 'inline' : 'none';
  // Check whether operator input already available.
  $form['card_type'] = array(
    '#type' => 'select',
    '#title' => t('Card Type'),
    '#required' => TRUE,
    '#default_value' => isset($card_type) ? $card_type : key($card_type_options),
    '#options' => $card_type_options,
    '#prefix' => '<div id="sepulsa-autocomplete-card-type" style="display: ' . $card_type_display . ';">',
    '#suffix' => '</div>',
    '#validated' => TRUE,
  );

  $packet_display = (!empty($packet_options)) ? 'inline' : 'none';
  // Define packet dropdown select.
  $form['packet'] = array(
    '#type' => 'select',
    '#title' => t('Packet'),
    '#required' => TRUE,
    '#default_value' => (isset($packet)) ? $packet : key($packet_options),
    '#options' => $packet_options,
    '#prefix' => '<div id="sepulsa-autocomplete-packets" style="display: ' . $packet_display . ';">',
    '#suffix' => '</div>',
    '#validated' => TRUE,
  );

  // Define add to cart: currently as markup.
  // Check autocomplete for cart.
  $form['add'] = array(
    '#tree' => TRUE,
  );
  // Submit button: add to cart.
  $form['add']['cart'] = array(
    '#type' => 'submit',
    '#value' => t('Add to Cart'),
    '#disabled' => TRUE,
    '#attributes' => array(
      'class' => array('btn', 'style1', 'inactive'),
    ),
  );
  // Submit button: fast charge.
  $form['add']['charge'] = array(
    '#type' => 'submit',
    '#value' => t('Fast Charge'),
    '#disabled' => TRUE,
    '#attributes' => array(
      'class' => array('btn', 'style1', 'inactive'),
    ),
  );

  // Return form array.
  return $form;
}

/**
 * Function sepulsa_phone_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_phone_form_validate($form, &$form_state) {
  // 1. Validate phone number.
  // 1a. Available and not empty.
  // 1b. Has pattern +digits.
  // 1c. At least has 8 chars (magic number?).
  // 1d. Maximum has 14 chars (magic number?).
  if (isset($form_state['values']['phone']) && !empty($form_state['values']['phone'])
    && preg_match('#^\+?\d{8,14}#', $form_state['values']['phone'])
  ) {
    // Define phone variable.
    $phone = sepulsa_autocorrect_mobile_prefix($form_state['values']['phone']);
    // Get operator data.
    $operator_data = sepulsa_get_all_operator_data();
    // Define default packets.
    $packets = array();
    foreach ($operator_data as $operator) {
      if (in_array(substr($phone, 0, 4), $operator['prefix'])) {
        foreach ($operator['packet'] as $packet) {
          $packets += $packet;
        }
      }
    }

    // 4. Validate packet.
    // 4a. Available and not empty.
    // 4b. Numeric value.
    // 4c. Packet input has correct product id.
    if (isset($form_state['values']['packet']) && !empty($form_state['values']['packet'])
      && is_numeric($form_state['values']['packet'])
      && isset($packets[$form_state['values']['packet']])
    ) {
      // 5. Validate submit button: human or bot.
      // 5a. Check form operation.
      // 5b. Form operation must the same as form add/charge input.
      if (!(isset($form_state['values']['op']) && !empty($form_state['values']['op'])
        && ($form_state['values']['op'] == $form_state['values']['add']['cart']
        || $form_state['values']['op'] == $form_state['values']['add']['charge'])
      )) {
        // Invalid form input.
        form_set_error('packet', t('Illegal form operation.'));
      }
    }
    else {
      // Invalid packet input.
      form_set_error('packet', t('Illegal packet input.'));
    }

  }
  else {
    // This section always return error.
    form_set_error('phone', t('We apologize, your phone number is not supported by our system.'));
  }
}

/**
 * Function sepulsa_phone_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_phone_form_submit($form, &$form_state) {
  // Get global user object.
  global $user;

  // Fix phone number using autocorrect mobile prefix.
  $form_state['values']['phone'] = sepulsa_autocorrect_mobile_prefix($form_state['values']['phone']);

  // Load product.
  $product = commerce_product_load($form_state['values']['packet']);
  // Create new line item: mobile_prepaid.
  $line_item = commerce_product_line_item_new($product, 1, 0, array(), 'mobile_prepaid');
  // Get line item wrapper.
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  // Set field_phone_number based on user input.
  $line_item_wrapper->field_phone_number->set($form_state['values']['phone']);

  // Check fast charge operation: empty previous cart.
  if ($form_state['values']['op'] == $form_state['values']['add']['charge']) {
    // Get cart order.
    $order = commerce_cart_order_load($user->uid);
    // Check whether order is not empty.
    if (!empty($order)) {
      // Empty the cart.
      commerce_cart_order_empty($order);
    }
  }

  // Add the line item to cart.
  commerce_cart_product_add($user->uid, $line_item);

  // Redirect to coupon page.
  // By default redirection is handled by rules.
  // Check fast charge operation: redirect to checkout page.
  if ($form_state['values']['op'] == $form_state['values']['add']['charge']) {
    // Check rules action go to.
    if (isset($GLOBALS['_rules_action_drupal_goto_do'])) {
      unset($GLOBALS['_rules_action_drupal_goto_do']);
    }
    // Redirect to checkout page.
    $form_state['redirect'] = 'checkout';
  }
}

/**
 * Private Functions.
 */

/**
 * Function sepulsa_autocorrect_mobile_prefix().
 *
 * +62 / 062 become 08.
 *
 * @param string $phone
 *   Phone number.
 *
 * @return string
 *   Corrected phone number.
 */
function sepulsa_autocorrect_mobile_prefix($phone) {
  return preg_replace('#^[\+|0]62#', '08', $phone);
}

/**
 * Function sepulsa_autocomplete_prefix().
 *
 * Generate autocomplete for input prefix based on operator prefix data.
 *
 * @param string $string
 *   Input prefix.
 * @param int $count
 *   How many prefix character need to be autocomplete.
 *
 * @return array
 *   Autocomplete array.
 */
function sepulsa_autocomplete_prefix($string, $count = 4) {
  // Define default count.
  $count = intval($count);
  // Get List of operator and prefix code.
  $operators = sepulsa_get_all_operator_data();
  // Define default matches.
  $matches = array();
  // Only for non empty operators.
  if (!empty($operators)) {
    // Looping for each operator.
    foreach ($operators as $operator) {
      // Check whether prefix is available.
      if (isset($operator['prefix']) && !empty($operator['prefix'])) {
        // Looping for each prefix.
        foreach ($operator['prefix'] as $prefix) {
          // Autocomplete based on count parameter: create sub prefix.
          $sub_prefix = substr($prefix, 0, $count);
          // Check whether prefix is available and not exists before.
          if (strstr($sub_prefix, $string) !== FALSE && !array_key_exists($sub_prefix, $matches)) {
            $matches[$sub_prefix] = ' ' . $sub_prefix;
          }
        }
      }
    }
  }
  return $matches;
}

/**
 * Function sepulsa_autocomplete_phone_number().
 *
 * Autocomplete user's phone input: suggestion for mobile phone number.
 *
 * @param string $string
 *   Phone number input by user.
 *
 * @return string
 *   JSON output.
 */
function sepulsa_autocomplete_phone_number($string = '') {
  $matches = array();
  // Check whether strin gis not empty or has input 0.
  if (!empty($string) || $string == '0') {
    // Check ID access number.
    if (preg_match('#^[\+|0]?6?2?$#', $string)) {
      $matches['08'] = ' 08';
    }
    elseif (strlen($string) < 5 && preg_match('#^\d+$#', $string)) {
      // Check for 2 digits string.
      if (strlen($string) == 2) {
        $matches = sepulsa_autocomplete_prefix($string, 3);
      }
      // Check for 3 digits string.
      if (strlen($string) == 3) {
        $matches = sepulsa_autocomplete_prefix($string, 4);
      }
    }
    elseif (preg_match('#^\+62(\d+)#', $string, $match)) {
      $autocomplete = '0' . $match[1];
      $matches[$autocomplete] = ' ' . $autocomplete;
    }
    elseif (!is_numeric($string)) {
      $matches[''] = ' ' . t('Invalid Input');
    }
  }

  drupal_json_output($matches);
}
