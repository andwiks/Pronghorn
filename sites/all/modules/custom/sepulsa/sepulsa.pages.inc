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
function sepulsa_autocomplete($type, $string = '') {
  // Get autocomplete callback by type.
  switch ($type) {
    case 'phone':
      return sepulsa_autocomplete_phone_number($string);

    case 'data':
      return sepulsa_autocomplete_phone_number($string, TRUE);

    default:
      drupal_access_denied();
  }
}

/**
 * Function sepulsa_front_page().
 */
function sepulsa_front_page() {
  global $user;
  // Get cart order.
  $order = commerce_cart_order_load($user->uid);
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
function sepulsa_phone_form($form, &$form_state, $data_packet = FALSE) {
  global $user;
  $form_state['data_packet'] = $data_packet;

  $existing_phone_options = array();
  // Get sepulsa operator data.
  $operator_data = sepulsa_get_all_operator_data(FALSE, $data_packet);

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
  $packet_options = array(
    0 => t('Select Packet'),
  );

  // Define default packet description
  $desc = "";
  // Check previous form state.
  $phone = (isset($form_state['input']['phone']) && !empty($form_state['input']['phone'])) ? $form_state['input']['phone'] : '';
  // Autocorrect phone prefix.
  $phone = sepulsa_autocorrect_mobile_prefix($phone);

  if (!$data_packet) {
    $query = "SELECT distinct(field_phone_number_value)
        FROM commerce_order a, commerce_line_item b, field_data_field_phone_number c
        WHERE
        a.order_id = b.order_id
        AND b.line_item_id = c.entity_id
        AND (a.status = 'completed' OR a.status = 'invoiced')
        AND a.uid = :uid
        ORDER BY a.created DESC";
    $result = db_query($query, array(':uid' => $user->uid));
    foreach ($result as $row) {
    $existing_phone_options[$row->field_phone_number_value] = $row->field_phone_number_value;
    }
    $existing_phone = sepulsa_get_existing_number();
    foreach ($existing_phone as $value) {
    $existing_phone_options[$value] = $value;
    }
    if (!(is_array($existing_phone_options) && empty($existing_phone_options))) {
    $existing_phone_options['0'] = t('Topup another number');
    $form['existing_phone'] = array(
      '#type' => 'select',
      '#title' => '',
      '#options' => $existing_phone_options,
      //'#suffix' => '<div>&nbsp;</div>',
      '#validated' => TRUE,
      '#attributes' => array(
      'class' => array('full-width'),
      ),
    );
    }
  }

  // Show input phone number.
  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#required' => TRUE,
    '#default_value' => (!empty($phone)) ? $phone : '',
    '#autocomplete_path' => 'sepulsa/autocomplete/phone',
    '#attributes' => array(
      'pattern' => '[0-9]*',
    'style'=>'margin-top: 15px;'
    ),
    // '#prefix' => '<div id="phone-sepulsa" style="display: inline;">',
    // '#suffix' => 'test</div>',
  );
  if ($data_packet) {
    $form['phone']['#autocomplete_path'] = 'sepulsa/autocomplete/data';
  }

  // Add operator options with operator data from phone prefix.
  foreach ($operator_data as $data) {
    // Add operator options.
    $operator_options[$data['operator']] = $data['operator'];
    // Check whether phone number is available.
    if (!empty($phone)) {
      // Define default operator value.
      if (in_array(substr($phone, 0, 4), $data['prefix'])) {
        // Define default value for operator.
        $operator = $data['operator'];
        // Define option for card type.
        $card_type_options = $data['cardtype'];
        if (isset($form_state['input']['card_type']) && isset($card_type_options[$form_state['input']['card_type']])) {
          // Define default value for card type.
          $card_type = $form_state['input']['card_type'];
          // Define price to compare packet price.
          $price = 0;
          // Define default packet.
          $packet = 0;
          // Looping to get default options for packet.
          foreach ($data['packet'][$card_type] as $packets) {
            $packet_options[$packets['id']] = $packets['title'];
            // If price is bigger, than use this as default packet.
            if ($packets['price'] > $price) {
              $price = $packets['price'];
              $packet = $packets['id'];
            }
          }
          // Redefine default value for packet.
          $packet = (isset($form_state['input']['packet']) && isset($packet_options[$form_state['input']['packet']])) ? $form_state['input']['packet'] : $packet;
        }
      }
    }
    foreach ($data['packet'] as $packets) {
      foreach ($packets as $packet) {
        $descs[$packet['id']] = (isset($packet['desc']) ? $packet['desc'] : "");
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
    // '#prefix' => '<div id="operator-sepulsa" style="display: none;">',
    // '#suffix' => '</div>',
  );

  // Check whether operator input already available.
  $form['card_type'] = array(
    '#type' => 'select',
    '#title' => t('Card Type'),
    '#required' => TRUE,
    '#default_value' => isset($card_type) ? $card_type : 0,
    '#options' => $card_type_options,
    '#prefix' => '<div id="sepulsa-autocomplete-card-type" style="display: ' . ((count($card_type_options) > 1) ? 'inline' : 'none') . ';">',
    '#suffix' => '</div>',
    '#validated' => TRUE,
  );

  // Define packet dropdown select.
  $form['packet'] = array(
    '#type' => 'select',
    '#title' => t('Packet'),
    '#required' => TRUE,
    '#default_value' => (isset($packet)) ? $packet : 0,
    '#options' => $packet_options,
    '#prefix' => '<div id="sepulsa-autocomplete-packets" style="display: ' . ((!empty($packet_options)) ? 'inline' : 'none') . ';">',
    '#suffix' => '</div>',
    '#validated' => TRUE,
  );

  // Data package information
  $data_info = "";
  foreach ($descs as $key => $desc) {
    if ($desc != '') {
      $data_info .= '<div id="packet-description' . $key . '" class="hidden_div" style="border: 1px solid; padding: 1em; margin: 15px 0; font-size:1.2em; clear:both;">' . $desc . '</div>';
    }
  }
  $form['data_info'] = array(
    '#markup' => $data_info
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
    && preg_match('#^\+?\d{8,14}$#', $form_state['values']['phone'])
  ) {
    // Define phone variable.
    $phone = sepulsa_autocorrect_mobile_prefix($form_state['values']['phone']);
    // Get operator data.
    $operator_data = sepulsa_get_all_operator_data(FALSE, $form_state['data_packet']);
    // Define default packets.
    $packets = array();
    // Looping for each operator data.
    foreach ($operator_data as $operator) {
      // Chech wether prefix for this operator is matched.
      if (in_array(substr($phone, 0, 4), $operator['prefix'])) {
        // Looping for each operator packet.
        foreach ($operator['packet'] as $packet) {
          // Check whether packet is available.
          if (!empty($packet)) {
            // Looping foreach packet.
            foreach ($packet as $weight => $packet_data) {
              // Check whether packet data has id and title.
              if (isset($packet_data['id']) && isset($packet_data['title'])) {
                $packets[$packet_data['id']] = $packet_data['title'];
              }
            }
          }
        }
        // Break: the other value has no meaning anymore.
        break;
      }
    }
    // No need to validate card type: because sometimes the option is hidden.
    // 2. Validate packet.
    // 2a. Available and not empty.
    // 2b. Numeric value.
    // 2c. Packet input has correct product id.
    if (isset($form_state['values']['packet']) && !empty($form_state['values']['packet'])
      && is_numeric($form_state['values']['packet'])
      && isset($packets[$form_state['values']['packet']])
    ) {
      // 3. Validate submit button: human or bot.
      // 3a. Check form operation.
      // 3b. Form operation must the same as form add/charge input.
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

  $order = commerce_cart_order_load($GLOBALS['user']->uid);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    if ($line_item_wrapper->getBundle() == 'mobile_prepaid' && $line_item_wrapper->field_phone_number->value() == $phone) {
      form_set_error('phone', t('The number you enter already on cart'));
    }
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

  // Load product: based on input packet id (validated before).
  $product = commerce_product_load($form_state['values']['packet']);
  // Create new line item: mobile_prepaid.
  $line_item = commerce_product_line_item_new($product, 1, 0, array(), 'mobile_prepaid');
  // Get line item wrapper.
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  // Set field_phone_number based on user input.
  $line_item_wrapper->field_phone_number->set($form_state['values']['phone']);

  // Add the line item to cart.
  commerce_cart_product_add($user->uid, $line_item);
}

/**
 * Private Functions.
 */

/**
 * Function sepulsa_autocorrect_mobile_prefix().
 *
 * +62 / 062 / 62 become 08.
 *
 * @param string $phone
 *   Phone number.
 *
 * @return string
 *   Corrected phone number.
 */
function sepulsa_autocorrect_mobile_prefix($phone) {
  // Auto correct for pattern: +/0 62 to 08.
  return preg_replace('#^[\+|0]?62#', '08', $phone);
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
function sepulsa_autocomplete_prefix($string, $count = 4, $data_packet = FALSE) {
  // Define default count.
  $count = intval($count);
  // Get List of operator and prefix code.
  $operators = sepulsa_get_all_operator_data(FALSE, $data_packet);
  // Define default matches.
  $matches = array();
  // Only for non empty operators.
  if (!empty($operators)) {
    // Looping for each operator.
    foreach ($operators as $operator) {
      // Check whether prefix is available.
      $packet = reset($operator['packet']);
      if (isset($operator['prefix']) && !empty($operator['prefix']) && !empty($packet)) {
        // Looping for each prefix.
        foreach ($operator['prefix'] as $prefix) {
          // Autocomplete based on count parameter: create sub prefix.
          $sub_prefix = substr($prefix, 0, $count);
          // Check whether prefix is available and not exists before.
          if (strstr($sub_prefix, $string) !== FALSE && !array_key_exists($sub_prefix, $matches)) {
            // Add space before prefix to improve readability.
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
function sepulsa_autocomplete_phone_number($string = '', $data_packet = FALSE) {
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
        $matches = sepulsa_autocomplete_prefix($string, 3, $data_packet);
      }
      // Check for 3 digits string.
      if (strlen($string) == 3) {
        $matches = sepulsa_autocomplete_prefix($string, 4, $data_packet);
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
