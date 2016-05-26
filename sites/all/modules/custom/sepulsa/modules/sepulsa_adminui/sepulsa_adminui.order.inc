<?php
/**
 * @file
 * sepulsa_adminui.order.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function sepulsa_adminui_tools_order_processing().
 */
function sepulsa_adminui_tools_order_processing() {
  $pager = 50;
  // Define order table header.
  $order_header = array(
    'title' => array('data' => t('Order ID'), 'field' => 'co.order_id'),
    'created' => array('data' => t('Created'), 'field' => 'co.created'),
    'user' => array('data' => t('UID'), 'field' => 'co.uid'),
    'mail' => array('data' => t('Mail'), 'field' => 'co.mail'),
    'status' => array('data' => t('Status'), 'field' => 'co.status'),
    'payment' => array('data' => t('Payment')),
    'product' => array('data' => t('Product')),
    'kraken' => array('data' => t('Kraken Status')),
    'op' => array('data' => t('Operations')),
  );
  $order_rows = array();
  // Define search parameter.
  $search = array();
  // Check order search id.
  if (isset($_SESSION['order']['search']['id']) && !empty($_SESSION['order']['search']['id'])) {
    $search['id'] = (int) $_SESSION['order']['search']['id'];
  }
  // Check order search created.
  if (isset($_SESSION['order']['search']['created']) && !empty($_SESSION['order']['search']['created'])) {
    $search['created'] = (int) $_SESSION['order']['search']['created'];
  }
  // Check order search mail.
  if (isset($_SESSION['order']['search']['mail']) && !empty($_SESSION['order']['search']['mail'])) {
    $search['mail'] = trim($_SESSION['order']['search']['mail']);
  }
  // Check if there is search parameter.
  if (!empty($search)) {
    $query = db_select('commerce_order', 'co')->extend('PagerDefault')->extend('TableSort');
    $query->leftJoin('commerce_payment_transaction', 'cpt', 'cpt.order_id = co.order_id');
    $query->leftJoin('commerce_line_item', 'cli', 'cli.order_id = co.order_id');
    $query->leftJoin('field_data_field_phone_number', 'fdfpn', 'fdfpn.entity_id = cli.line_item_id');
    $query->leftJoin('field_data_field_customer_number', 'fdfcn', 'fdfcn.entity_id = cli.line_item_id');
    $query->fields('cpt', array('instance_id'));
    $query->fields('cli', array('line_item_label', 'type'));
    $query->fields('fdfpn', array('field_phone_number_value'));
    $query->fields('fdfcn', array('field_customer_number_value'));
    $query->fields('co', array(
      'order_id', 'uid', 'mail', 'status', 'created',
    ));
    $query->condition('co.status', 'processing');
    // Search by ID.
    if (isset($search['id'])) {
      $query->condition('co.order_id', $search['id']);
    }
    // Search by mail.
    if (isset($search['mail'])) {
      $query->condition('co.mail', $search['mail']);
    }
    // Search by created.
    if (isset($search['created'])) {
      $query->condition('co.created', $search['created'], '>');
      $query->condition('co.created', $search['created'] + (3600 * 24), '<=');
    }
    $query->condition('cli.type', array(
      'mobile_prepaid', 'pln_prepaid', 'multifinance', 'bpjs_kesehatan',
    ), 'IN');
    $query->condition('cpt.status', 'success');
    $total_query = clone $query;
    $total = $total_query->countQuery()->execute()->fetchField();
    $orders = $query->limit($pager)
      ->orderByHeader($order_header)->execute()->fetchAll();
    // Check if query can find any order.
    if (!empty($orders)) {
      // Define payment methods container.
      $payment_methods = array();
      // Looping for each orders.
      foreach ($orders as $order) {
        $links = array();
        // Get definition of payment method.
        if (!isset($payment_methods[$order->instance_id])) {
          $payment_methods[$order->instance_id] = commerce_payment_method_instance_load($order->instance_id);
        }
        // Define product name.
        $product = $order->line_item_label . '<br />';
        $product .= ($order->type == 'mobile_prepaid') ? $order->field_phone_number_value : $order->field_customer_number_value;
        // Define operations.
        $links = array(
          array(
            'title' => t('Continue Process'),
            'href' => 'admin/commerce/tools/order/processing/continue/' . $order->order_id,
          ),
          array(
            'title' => t('View Biller'),
            'href' => 'admin/commerce/tools/kraken/biller/' . $order->order_id,
          ),
        );
        // Check ctools module.
        if (module_exists('ctools')) {
          // Include ctools dropdown.theme.
          ctools_include('dropbutton.theme');
          // Generate ctools dropdown.
          $operations = theme_links__ctools_dropbutton(array(
            'class' => FALSE,
            'image' => FALSE,
            'title' => FALSE,
            'attributes' => array(),
            'links' => $links,
          ));
        }
        else {
          $operations = theme_links(array(
            'links' => $links,
            'attributes' => array(),
            'heading' => array(),
          ));
        }
        // Store in order rows.
        $order_rows[$order->order_id] = array(
          'title' => l($order->order_id, 'admin/commerce/orders/' . $order->order_id, array(
            'attributes' => array('target' => '_blank'),
          )),
          'created' => format_date($order->created),
          'user' => l($order->uid, 'user/' . $order->uid, array(
            'attributes' => array('target' => '_blank'),
          )),
          'mail' => l($order->mail, 'admin/people/kredit/' . $order->uid . '/detail', array(
            'attributes' => array('target' => '_blank'),
          )),
          'status' => $order->status,
          'payment' => l($payment_methods[$order->instance_id]['title'], 'admin/commerce/orders/' . $order->order_id . '/payment', array(
            'attributes' => array('target' => '_blank'),
          )),
          'product' => $product,
          'kraken' => t('Unknown'),
          'op' => $operations,
        );
      }
      // Get kraken transaction status.
      $kraken_status = _sepulsa_get_kraken_status(array_keys($order_rows));
      // Update kraken transaction.
      foreach ($kraken_status as $order_id => $kraken_data) {
        switch ($kraken_data['status']) {
          case 'pending':
            $order_rows[$order_id]['kraken'] = '<strong  style="color: yellow;">' . strtoupper($kraken_data['status']) . '</strong>';
            break;

          case 'success':
            $order_rows[$order_id]['kraken'] = '<strong  style="color: green;">' . strtoupper($kraken_data['status']) . '</strong>';
            break;

          default:
            $order_rows[$order_id]['kraken'] = '<strong style="color: red;">' . strtoupper($kraken_data['status']) . '</strong>';
            break;
        }
      }
    }
  }

  // Get search form.
  $build['search'] = drupal_get_form('sepulsa_adminui_tools_order_processing_search_form');
  $build['search']['#attributes']['class'][] = 'container-inline';

  if (!empty($order_rows)) {
    // Define start and end information.
    $start_info = (empty($total)) ? 0 : (($total > $pager) ? ((isset($_GET['page']) && !empty($_GET['page'])) ? (intval($_GET['page']) * $pager) + 1 : 1) : 1);
    $end_info = ($total > $pager) ? ((isset($_GET['page']) && !empty($_GET['page'])) ? (intval($_GET['page']) * $pager) + $pager : $pager) : $total;
    $end_info = ($end_info > $total) ? $total : $end_info;
    // Display page information.
    $info['order_info'] = array(
      '#markup' => t('Displaying @start - @end of @total (total page @page).<br />', array(
        '@start' => $start_info,
        '@end' => $end_info,
        '@total' => $total,
        '@page' => ceil($total / $pager),
      )),
    );
    $info['order'] = array(
      '#type' => 'tableselect',
      '#header' => $order_header,
      '#options' => $order_rows,
      '#empty' => t('There is no order still on processing based on search parameter.'),
    );

    // Build the tableselect.
    $build['order'] = drupal_get_form('sepulsa_adminui_tools_order_processing_execute_form', $info);

    // Attach the pager theme.
    $build['pager_pager'] = array('#theme' => 'pager');
  }

  return $build;
}

/**
 * Function sepulsa_adminui_tools_order_processing_search_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function sepulsa_adminui_tools_order_processing_search_form($form = array(), $form_state = array()) {
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
  );
  $form['search']['created'] = array(
    '#type' => 'date_popup',
    '#title' => t('Created Date'),
    '#title_display' => 'invisible',
    '#date_format' => 'Y-m-d',
    '#date_label_position' => 'none',
    '#date_timezone' => 'Asia/Jakarta',
    '#date_year_range' => '-0:+1',
    '#description' => '',
    '#attributes' => array(
      'placeholder' => t('Created Date: @date', array('@date' => date('Y-m-d'))),
    ),
    '#default_value' => isset($_SESSION['order']['search']['created']) && !empty($_SESSION['order']['search']['created']) ? date('Y-m-d', $_SESSION['order']['search']['created']) : '',
  );
  $form['search']['id'] = array(
    '#prefix' => '<br />',
    '#type' => 'textfield',
    '#title' => t('Order ID'),
    '#title_display' => 'invisible',
    '#default_value' => isset($_SESSION['order']['search']['id']) && !empty($_SESSION['order']['search']['id']) ? $_SESSION['order']['search']['id'] : '',
    '#size' => 10,
    '#maxlength' => 20,
    '#attributes' => array(
      'placeholder' => t('Order ID'),
    ),
  );
  $form['search']['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail'),
    '#title_display' => 'invisible',
    '#default_value' => isset($_SESSION['order']['search']['mail']) && !empty($_SESSION['order']['search']['mail']) ? $_SESSION['order']['search']['mail'] : '',
    '#size' => 20,
    '#maxlength' => 255,
    '#attributes' => array(
      'placeholder' => t('Customer Email'),
    ),
  );
  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  $form['search']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('sepulsa_adminui_tools_order_processing_reset'),
  );
  return $form;
}

/**
 * Function sepulsa_adminui_tools_order_processing_search_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_adminui_tools_order_processing_search_form_validate($form, &$form_state) {
  // Only for operation search.
  if (isset($form_state['values']['op']) && $form_state['values']['op'] == t('Search')) {
    // Validate order id.
    if (isset($form_state['values']['id']) && !empty($form_state['values']['id'])) {
      if (!is_numeric($form_state['values']['id'])) {
        form_set_error('id', t('Order ID need positif numeric input.'));
      }
    }
    // Validate order mail.
    if (isset($form_state['values']['mail'])
      && !empty($form_state['values']['mail'])
      && !valid_email_address($form_state['values']['mail'])
    ) {
      form_set_error('mail', t('Please input valid email address.'));
    }
  }
}

/**
 * Function sepulsa_adminui_tools_order_processing_search_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_adminui_tools_order_processing_search_form_submit($form, &$form_state) {
  $_SESSION['order']['search'] = array();
  if (isset($form_state['values']['id']) && !empty($form_state['values']['id'])) {
    $form_state['values']['id'] = trim($form_state['values']['id']);
    $_SESSION['order']['search']['id'] = (int) $form_state['values']['id'];
  }
  if (isset($form_state['values']['mail']) && !empty($form_state['values']['mail'])) {
    $_SESSION['order']['search']['mail'] = trim($form_state['values']['mail']);
  }
  if (isset($form_state['values']['created']) && !empty($form_state['values']['created'])) {
    $form_state['values']['created'] = trim($form_state['values']['created']);
    $_SESSION['order']['search']['created'] = strtotime($form_state['values']['created']);
  }
}

/**
 * Function sepulsa_adminui_tools_order_processing_reset().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_adminui_tools_order_processing_reset($form, &$form_state) {
  if (isset($_SESSION['order']['search'])) {
    unset($_SESSION['order']['search']);
  }
}

/**
 * Function sepulsa_adminui_tools_order_processing_execute_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param array $info
 *   Order info and table as array.
 *
 * @return array
 *   Form array.
 */
function sepulsa_adminui_tools_order_processing_execute_form($form = array(), $form_state = array(), $info = NULL) {
  if (!empty($info)) {
    $form['operation'] = array(
      '#type' => 'fieldset',
      '#title' => t('Bulk Operation'),
      '#attributes' => array(
        'class' => array('container-inline'),
      ),
    );
    $form['operation']['select'] = array(
      '#type' => 'select',
      '#title' => t('Operation'),
      '#title_display' => 'invisible',
      '#options' => array(
        'continue' => t('Continue Process'),
      ),
      '#required' => TRUE,
    );
    $form['operation']['go'] = array(
      '#type' => 'submit',
      '#value' => t('Go'),
    );
    $form['execute'] = $info;
  }
  // Remove unused session.
  if (isset($_SESSION['order']['execute'])) {
    unset($_SESSION['order']['execute']);
  }
  return $form;
}

/**
 * Function sepulsa_adminui_tools_order_processing_execute_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_adminui_tools_order_processing_execute_form_validate($form, &$form_state) {
  // At least there is one order.
  if (isset($form_state['values']['order'])) {
    $orders = array_filter($form_state['values']['order']);
    if (empty($orders)) {
      form_set_error('order', t('Need at least one order id.'));
    }
  }
}

/**
 * Function sepulsa_adminui_tools_order_processing_execute_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_adminui_tools_order_processing_execute_form_submit($form, &$form_state) {
  $_SESSION['order']['execute'][$form_state['values']['select']] = array_values($form_state['values']['order']);
  $form_state['redirect'] = 'admin/commerce/tools/order/processing/' . $form_state['values']['select'];
}

/**
 * Function sepulsa_adminui_tools_order_processing_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param string $operation
 *   Process operation.
 *
 * @return array
 *   Form array.
 */
function sepulsa_adminui_tools_order_processing_form($form = array(), $form_state = array(), $operation = NULL) {
  // Check process operation.
  if (empty($operation)) {
    drupal_set_message(t('Undefined order process operation.'), 'error');
    drupal_goto('admin/commerce/tools/order/processing');
  }
  $order_ids = array();
  // Checking sessions.
  if (isset($_SESSION['order']['execute'][$operation]) && !empty($_SESSION['order']['execute'][$operation])) {
    $order_ids = $_SESSION['order']['execute'][$operation];
  }
  // Checking 6th arguments.
  $id = arg(6);
  if (!empty($id)) {
    $order_ids = array($id);
  }
  $order_ids = array_filter($order_ids);
  // Check there is order id to process.
  if (empty($order_ids)) {
    drupal_set_message(t('Can not do %operation process order: order id not found.', array(
      '%operation' => $operation,
    )), 'error');
    drupal_goto('admin/commerce/tools/order/processing');
  }
  // Looping foreach order id.
  foreach ($order_ids as $key => $id) {
    $list[$key] = l($id, 'admin/commerce/orders/' . $id, array(
      'attributes' => array('target' => '_blank'),
    ));
  }
  $form['list'] = array(
    '#markup' => theme_item_list(array(
      'items' => $list,
      'title' => t('List of order:'),
      'type' => 'ul',
      'attributes' => array(),
    )),
  );
  $form['ids'] = array(
    '#type' => 'value',
    '#value' => $order_ids,
  );
  $form['operation'] = array(
    '#type' => 'value',
    '#value' => $operation,
  );

  $message = t('You want to do %operation operation to order.', array(
    '%operation' => strtoupper($operation),
  ));
  $caption = '<p>' . t('Are you sure?') . '</p>';

  return confirm_form($form, filter_xss_admin($message), 'admin/commerce/tools/order/processing', filter_xss_admin($caption), ucfirst($operation));
}

/**
 * Function sepulsa_adminui_tools_order_processing_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_adminui_tools_order_processing_form_validate($form, &$form_state) {
  if (!(isset($form_state['values']['ids']) && !empty($form_state['values']['ids']))) {
    form_set_message('ids', t('Unable to continue, missing order ids.'));
  }
  if (!(isset($form_state['values']['operation']) && !empty($form_state['values']['operation']))) {
    form_set_message('operation', t('Unable to continue, unknown operation.'));
  }
}

/**
 * Function sepulsa_adminui_tools_order_processing_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_adminui_tools_order_processing_form_submit($form, &$form_state) {
  // Checking sessions.
  if (isset($_SESSION['order']['execute']) && !empty($_SESSION['order']['execute'])) {
    unset($_SESSION['order']['execute']);
  }
  // For case operation continue.
  if ($form_state['values']['operation'] == 'continue') {
    $total = count($form_state['values']['ids']);
    // Looping for each order id.
    foreach ($form_state['values']['ids'] as $order_id) {
      $operations[] = array(
        'sepulsa_adminui_tools_order_process_continue',
        array($order_id, $total),
      );
    }
    $batch = array(
      'operations' => $operations,
      'finished' => 'sepulsa_adminui_tools_processing_batch_finished',
      'title' => t('Processing Order'),
      'init_message' => t('Process is starting.'),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Process order has encountered an error.'),
    );
    batch_set($batch);
  }
  // Redirect to main process page.
  $form_state['redirect'] = 'admin/commerce/tools/order/processing';
}
