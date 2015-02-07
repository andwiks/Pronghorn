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
  // Add sepulsa autocomplete form js.
  drupal_add_js(drupal_get_path('module', 'sepulsa') . '/sepulsa.form.js', 'file');

  // Get all operator phone prefixes.
  $prefixes = sepulsa_get_all_operator_prefix();
  // Get all operator card types.
  $card_types = sepulsa_get_all_operator_card_type();
  // Store these prefixes inside drupal settings.
  drupal_add_js(array(
    'sepulsa' => array(
      'prefix' => $prefixes,
      'cardtype' => $card_types,
    ),
  ), 'setting');

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
    '#attributes' => array('autocomplete' => 'off'),
    '#autocomplete_path' => 'sepulsa/autocomplete/phone',
  );



  // Define default operator options.
  $operator_options = array(
    0 => t('Select Operator'),
  );
  // Add operator options with operator data from phone prefix.
  $operator_options += drupal_map_assoc(array_keys($prefixes));

  // Check whether phone number is available.
  if (!empty($phone)) {
    // Define default operator value.
    $operator = sepulsa_get_operator($phone);
    // Check whether we can get only one operator.
    if (count($operator) == 1) {
      $operator = current($operator);
    }
    else {
      // Trash unusable operator data.
      unset($operator);
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

  // Define default operator options.
  $card_type_options = array(
    0 => t('Select Card Type'),
  );

  $card_type_options += sepulsa_get_all_operator_card_type();

  // Check whether phone number is available.
  if (!empty($phone) && isset($operator)) {
    // Define default value for card type.
    $card_type = (isset($form_state['values']['card_type']) && !empty($form_state['values']['card_type'])) ? $form_state['values']['card_type'] : 0;
    $card_type = intval($card_type);
  }
  // Define operator dropdown select.
  $form['card_type'] = array(
    '#type' => 'select',
    '#title' => t('Card Type'),
    '#required' => TRUE,
    '#default_value' => (isset($card_type)) ? $card_type : 0,
    '#options' => $card_type_options,
    '#prefix' => '<div id="sepulsa-autocomplete-card-type">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'sepulsa_autocomplete_packets',
      'wrapper' => 'sepulsa-autocomplete-packets',
      'event' => 'change blur',
    ),
    '#validated' => TRUE,
  );

  // Define default packet options.
  $options = array(
    0 => t('Select Packet'),
  );
  // Check whether operator has been selected.
  if (isset($card_type) && !empty($card_type)) {
    // Add packet options based on operator input.
    $options += sepulsa_get_packet($card_type);
    // Define default value for operator.
    $packet = (isset($form_state['values']['packet']) && !empty($form_state['values']['packet'])) ? $form_state['values']['packet'] : 0;
    $packet = intval($packet);
  }
  // Define packet dropdown select.
  $form['packet'] = array(
    '#type' => 'select',
    '#title' => t('Packet'),
    '#required' => TRUE,
    '#default_value' => (isset($packet)) ? $packet : 0,
    '#options' => $options,
    '#ajax' => array(
      'callback' => 'sepulsa_autocomplete_cart',
      'wrapper' => 'sepulsa-autocomplete-cart',
      'event' => 'ready change blur',
    ),
    '#prefix' => '<div id="sepulsa-autocomplete-packets">',
    '#suffix' => '</div>',
    '#validated' => TRUE,
  );

  // Define add to cart: currently as markup.
  $form['add'] = array(
    '#markup' => '<div id="sepulsa-autocomplete-cart"></div>',
  );
  // Check autocomplete for cart.
  $form['add'] = sepulsa_autocomplete_cart($form, $form_state);

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
    // Get operators based on phone number.
    $operator = sepulsa_get_operator($phone);
    // 2. Validate operator.
    // 2a. Available and not empty.
    // 2b. Numeric value.
    // 2c. Operator input has correct term id.
    if (isset($form_state['values']['operator']) && !empty($form_state['values']['operator'])
      && is_numeric($form_state['values']['operator'])
      && isset($operator[$form_state['values']['operator']])
    ) {
      // Get card types from operator.
      $card_types = sepulsa_get_card_type($operator);
      // 3. Validate card type.
      // 3a. Available and not empty.
      // 3b. Numeric value.
      // 3c. Card type input has correct node id.
      if (isset($form_state['values']['card_type']) && !empty($form_state['values']['card_type'])
        && is_numeric($form_state['values']['card_type'])
        && isset($card_types[$form_state['values']['card_type']])
      ) {
        // Define operator node id.
        $card_type = intval($form_state['values']['card_type']);
        // Get packets based on operator input.
        $packets = sepulsa_get_packet($card_type);
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
        // Invalid card type input.
        form_set_error('card_type', t('Illegal card_type input.'));
      }
    }
    else {
      // Invalid operator input.
      form_set_error('operator', t('Illegal operator input.'));
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

  // Load node pulsa.
  $pulsa = node_load($form_state['values']['card_type']);
  // Show message to user.
  // drupal_set_message(t('@pulsa (@product) has been added to cart.', array(
  //  '@pulsa' => $pulsa->title,
  //  '@product' => $product->title,
  // )));
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
 * Function sepulsa_autocomplete_operator().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return form
 *   Form operator array.
 */
function sepulsa_autocomplete_operator($form, &$form_state) {
  // Check for input phone number.
  if (!empty($form_state['values']['phone'])
    && strlen($form_state['values']['phone']) >= 4
    && preg_match('#^\+?\d+$#', $form_state['values']['phone'])
  ) {
    // Sanitize phone number.
    $phone = sepulsa_autocorrect_mobile_prefix($form_state['values']['phone']);
    // Get list of operator based on phone number.
    $operator = sepulsa_get_operator($phone);
    // Check for non empty options.
    if (!empty($operator)) {
      // Add operator options.
      $form['operator']['#options'] = $operator;
      $form['operator']['#validated'] = TRUE;
    }
    // Get list of card type based on operator.
    $card_types = sepulsa_get_card_type($operator);
    // Check for non empty options.
    if (!empty($card_types)) {
      // Add operator options.
      $form['card_type']['#options'] = $form['card_type']['#options'] + $card_types;
      $form['card_type']['#validated'] = TRUE;
    }
  }

  // Return ajax command so multiple elements can be replaced.
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace('#sepulsa-autocomplete-operator', render($form['operator'])),
      ajax_command_replace('#sepulsa-autocomplete-card-type', render($form['card_type'])),
      ajax_command_invoke('#edit-phone', 'focus'),
    ),
  );
}

/**
 * Function sepulsa_autocomplete_packets().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form packet array.
 */
function sepulsa_autocomplete_packets($form, &$form_state) {
  // Get input card_type, which is a node id.
  $nid = (isset($form_state['values']['card_type']) && !empty($form_state['values']['card_type'])) ? intval($form_state['values']['card_type']) : 0;
  // Check whehter we can get node id.
  if (!empty($nid)) {
    // Get packet options.
    $options = sepulsa_get_packet($nid);
    // Check for non empty options.
    if (!empty($options)) {
      // Add packets options.
      $form['packet']['#options'] = $form['packet']['#options'] + $options;
      $form['packet']['#validated'] = TRUE;
    }
  }
  // Return packet form.
  return $form['packet'];
}

/**
 * Function sepulsa_autocomplete_cart().
 *
 * @param array $form
 *   Form array.
 *
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form add to cart button.
 */
function sepulsa_autocomplete_cart($form, &$form_state) {
  // Check for all for nonempty input: validate function will inspect the value.
  if (isset($form_state['values']['phone']) && !empty($form_state['values']['phone'])
    && isset($form_state['values']['operator']) && !empty($form_state['values']['operator'])
    && isset($form_state['values']['card_type']) && !empty($form_state['values']['card_type'])
    && isset($form_state['values']['packet']) && !empty($form_state['values']['packet'])
  ) {
    $form['add'] = array(
      '#tree' => TRUE,
    );
    // Submit button: add to cart.
    $form['add']['cart'] = array(
      '#type' => 'submit',
      '#value' => t('Add to Cart'),
      '#attributes' => array('class' => array('btn', 'style1'), 'style' => 'float:right'),
    );
    // Submit button: fast charge.
    $form['add']['charge'] = array(
      '#type' => 'submit',
      '#value' => t('Fast Charge'),
      '#attributes' => array('class' => array('btn', 'style1'), 'style' => 'float:right'),
    );
  }
  // Return form add: markup or submit button.
  return $form['add'];
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
  $operators = sepulsa_get_all_operator_prefix();
  // Define default matches.
  $matches = array();
  // Only for non empty operators.
  if (!empty($operators)) {
    foreach ($operators as $prefixs) {
      foreach ($prefixs as $prefix) {
        $sub_prefix = substr($prefix, 0, $count);
        if (strstr($sub_prefix, $string) !== FALSE && !array_key_exists($sub_prefix, $matches)) {
          $matches[$sub_prefix] = ' ' . $sub_prefix;
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
  if (!empty($string)) {
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

/**
 * Function sepulsa_get_operator().
 *
 * Output Format:
 * key: taxonomy id.
 * value: taxonomy term name.
 *
 * @param string $phone
 *   Phone number.
 *
 * @return array
 *   Operator content.
 */
function sepulsa_get_operator($phone) {
  // Chop phone number.
  $phone = substr($phone, 0, 4);
  // Get taxonomy id: based on phone prefix.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'taxonomy_term')
    ->entityCondition('bundle', array('mobile_operator'))
    ->fieldCondition('field_operator_prefix', 'value', $phone, '=');
  $result = $query->execute();
  // Get taxonomy term ids.
  $tids = (isset($result['taxonomy_term'])) ? array_keys($result['taxonomy_term']) : array();

  if (!empty($tids)) {
    foreach ($tids as $tid) {
      // Load taxonomy term by tid.
      $term = taxonomy_term_load($tid);
      // This suppose to result only 1 tid, so after finish, break the loop.
      return array($tid => $term->name);
    }
  }
  return array();
}

/**
 * Function sepulsa_get_packet().
 *
 * Format:
 * key: product_id.
 * value: product title.
 *
 * @param int $nid
 *   Node id.
 *
 * @return array
 *   Packet options array.
 */
function sepulsa_get_packet($nid) {
  // Define default packet options.
  $options = array();

  // Load node wrapper.
  $node_wrapper = entity_metadata_wrapper('node', $nid);
  // Check node type and node status.
  if ($node_wrapper->type->value() == 'pulsa'
    && $node_wrapper->status->value() == 1
  ) {
    // Get product object.
    $products = $node_wrapper->field_product->value();
    // Looping for each product.
    foreach ($products as $product) {
      // Check product status.
      if (!empty($product->status)) {
        // Add it inside options array.
        $options[$product->product_id] = $product->title;
      }
    }
  }

  // Return options.
  return $options;
}
