<?php
/**
 * @file
 * sepulsa_adminui.kraken.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Function sepulsa_adminui_kraken_data_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   System settings form.
 */
function sepulsa_adminui_kraken_data_form($form = array(), $form_state = array()) {
  $data = variable_get('sepulsa_adminui_kraken_data', array(
    'host' => '',
    'base' => '',
    'user' => '',
    'pass' => '',
  ));
  $form['sepulsa_adminui_kraken_data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Kraken Data'),
    '#tree' => TRUE,
  );
  $form['sepulsa_adminui_kraken_data']['host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#default_value' => $data['host'],
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['sepulsa_adminui_kraken_data']['base'] = array(
    '#type' => 'textfield',
    '#title' => t('Base'),
    '#default_value' => $data['base'],
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['sepulsa_adminui_kraken_data']['user'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#default_value' => $data['user'],
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['sepulsa_adminui_kraken_data']['pass'] = array(
    '#type' => 'password',
    '#title' => t('Pass'),
    '#default_value' => $data['pass'],
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['#validate'][] = 'sepulsa_adminui_kraken_data_form_validate';
  return system_settings_form($form);
}

/**
 * Function sepulsa_adminui_kraken_data_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_adminui_kraken_data_form_validate($form, &$form_state) {
  // Create database info array..
  $database = array(
    'database' => $form_state['values']['sepulsa_adminui_kraken_data']['base'],
    'username' => $form_state['values']['sepulsa_adminui_kraken_data']['user'],
    'password' => $form_state['values']['sepulsa_adminui_kraken_data']['pass'],
    'host' => $form_state['values']['sepulsa_adminui_kraken_data']['host'],
    'port' => 3306,
    'driver' => 'mysql',
  );
  // Encapsulate with try and catch.
  try {
    // Add database info.
    Database::addConnectionInfo('CheckConnect', 'default', $database);
    // Connect using added database.
    Database::getConnection('default', 'CheckConnect');
    // Set back to default database.
    db_set_active();
  }
  catch (Exception $e) {
    // Return failed check: error message.
    form_set_error('sepulsa_adminui_kraken_data', t('Unable to connect to Kraken with message: @message', array(
      '@message' => $e->getMessage(),
    )));
  }
}

/**
 * Function sepulsa_adminui_kraken_biller_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function sepulsa_adminui_kraken_biller_form($form = array(), $form_state = array()) {
  // Define biller table header.
  $biller_header = array(
    'id' => array('data' => t('Biller ID')),
    'type' => array('data' => t('Type')),
    'product_id' => array('data' => t('Product ID')),
    'status' => array('data' => t('Status')),
    'customer_number' => array('data' => t('Customer Number')),
    'price' => array('data' => t('Price')),
    'created' => array('data' => t('Created')),
    'response' => array('data' => t('Response Time')),
    'data' => array('data' => t('Data')),
  );
  $biller_rows = array();

  // Get order id from 5th arguments.
  $order_id = arg(5);

  // Search form.
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
    '#attributes' => array(
      'class' => array('container-inline'),
    ),
  );
  $form['search']['id'] = array(
    '#prefix' => '<br />',
    '#type' => 'textfield',
    '#title' => t('Order ID'),
    '#title_display' => 'invisible',
    '#default_value' => $order_id,
    '#size' => 10,
    '#maxlength' => 20,
    '#required' => TRUE,
    '#attributes' => array(
      'placeholder' => t('Order ID'),
    ),
  );
  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  // Check if there is order id input.
  if (!empty($order_id)) {
    $biller = _sepulsa_get_kraken_biller($order_id);
    krsort($biller);
    $transaction = array();
    // Looping foreach biller data.
    foreach ($biller as $biller_data) {
      $data = isset($biller_data->data) && !empty($biller_data->data) ? unserialize($biller_data->data) : array();
      $formatted_data = '';
      $token = '-';
      foreach ($data as $key => $value) {
        if (stripos($key, 'token') !== FALSE) {
          $token = preg_replace('#\d{4}#', '$0 ', $value);
        }
        elseif (stripos($key, 'bit') === 0) {
          // Bit data is longer: do nothing here.
        }
        else {
          $formatted_data .= t('!key: !value<br />', array(
            '!key' => $key,
            '!value' => $value,
          ));
        }
      }
      $biller_rows[$biller_data->biller_id] = array(
        'data' => array(
          'id' => $biller_data->biller_id,
          'type' => $biller_data->type,
          'product_id' => $biller_data->product_id,
          'status' => $biller_data->status,
          'customer_number' => $biller_data->customer_number,
          'price' => $biller_data->price,
          'created' => format_date($biller_data->created),
          'response' => isset($biller_data->response_time) ? $biller_data->response_time : t('unknown'),
          'data' => $formatted_data,
        ),
      );
      if (strpos($biller_data->type, 'pln') !== FALSE) {
        $biller_row_data = array_pop($biller_rows[$biller_data->biller_id]['data']);
        $biller_rows[$biller_data->biller_id]['data']['token'] = array(
          'data' => $token,
          'style' => 'width: 160px; font-weight: bold; color: red;',
        );
        $biller_rows[$biller_data->biller_id]['data']['data'] = $biller_row_data;
        if (!isset($biller_header['token'])) {
          $biller_header_data = array_pop($biller_header);
          $biller_header['token'] = array('data' => t('Token PLN'));
          $biller_header['data'] = $biller_header_data;
        }
      }
      $transaction = array(
        'id' => $biller_data->transaction_id,
        'status' => $biller_data->transaction_status,
      );
    }
    $form['order'] = array(
      '#type' => 'item',
      '#title' => t('Order ID'),
      '#markup' => $order_id,
    );
    if (!empty($transaction)) {
      $form['transaction'] = array(
        '#type' => 'item',
        '#title' => t('Transaction ID'),
        '#markup' => t('@id (@status)', array(
          '@id' => isset($transaction['id']) ? $transaction['id'] : 0,
          '@status' => isset($transaction['status']) ? $transaction['status'] : t('unknown'),
        )),
      );
    }
    $form['biller'] = array(
      '#markup' => theme('table', array(
        '#theme' => 'table',
        '#header' => $biller_header,
        '#rows' => $biller_rows,
        '#empty' => t('There are no biller data.'),
      )),
    );
  }
  return $form;
}

/**
 * Function sepulsa_adminui_kraken_biller_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_adminui_kraken_biller_form_validate($form, &$form_state) {
  // Validate order id.
  if (isset($form_state['values']['id']) && !empty($form_state['values']['id'])) {
    if (!is_numeric($form_state['values']['id'])) {
      form_set_error('id', t('Order ID need positif numeric input.'));
    }
  }
}

/**
 * Function sepulsa_adminui_kraken_biller_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_adminui_kraken_biller_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/tools/kraken/biller/' . $form_state['values']['id'];
}
