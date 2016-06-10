<?php
/**
 * @file
 * sepulsa_reports.voucher.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function sepulsa_reports_voucher_check().
 */
function sepulsa_reports_voucher_check() {
  // Define order cart statuses.
  $cart_statuses = array(
    'cart', 'checkout_checkout', 'checkout_payment',
  );
  // Define order table header.
  $order_header = array(
    array('data' => t('Product ID')),
    array('data' => t('Order ID'), 'field' => 'co.order_id'),
    array('data' => t('Created'), 'field' => 'co.created'),
    array('data' => t('Changed'), 'field' => 'co.changed'),
    array('data' => t('UID'), 'field' => 'co.uid'),
    array('data' => t('Mail'), 'field' => 'co.mail'),
    array('data' => t('Status'), 'field' => 'co.status'),
    array('data' => t('Transaction Total')),
    array('data' => t('Transaction Success')),
    array('data' => t('Transaction Pending')),
    array('data' => t('Transaction Failed')),
  );
  // Define order table row.
  $order_rows = array();

  // Define product order header.
  $product_header = array(
    array('data' => t('Voucher ID'), 'field' => 'cp.product_id'),
    array('data' => t('SKU'), 'field' => 'cp.sku'),
    array('data' => t('Title'), 'field' => 'cp.title'),
    array('data' => t('Status'), 'field' => 'cp.status'),
    array('data' => t('Published')),
    array('data' => t('Type')),
    array('data' => t('Product Stock')),
    array('data' => t('Total Stock')),
    array('data' => t('Total Available')),
    array('data' => t('Total Taken')),
    array('data' => t('Total Used')),
    array('data' => t('Total Unused')),
    array('data' => t('Total in Cart')),
  );
  // Define product rows.
  $product_rows = array();

  // Define search parameter.
  $search = array();
  // Check voucher search sku.
  if (isset($_SESSION['voucher']['search']['sku']) && !empty($_SESSION['voucher']['search']['sku'])) {
    $search['sku'] = $_SESSION['voucher']['search']['sku'];
  }
  // Check voucher search title.
  if (isset($_SESSION['voucher']['search']['title']) && !empty($_SESSION['voucher']['search']['title'])) {
    $search['title'] = $_SESSION['voucher']['search']['title'];
  }

  // Get search form.
  $build['search'] = drupal_get_form('sepulsa_reports_voucher_search_form');
  $build['search']['#attributes']['class'][] = 'container-inline';

  // Only start query if there is search data.
  if (!empty($search)) {
    // Query to get product voucher.
    $query = db_select('commerce_product', 'cp')->extend('TableSort');
    $query->fields('cp', array(
      'product_id', 'sku', 'title', 'status',
    ));
    // Only for product type: coupon.
    $query->condition('type', 'coupon');
    // Search by SKU.
    if (isset($search['sku'])) {
      $query->condition('sku', $search['sku']);
    }
    // Search by title.
    if (isset($search['title'])) {
      $query->condition('title', '%' . db_like($search['title']) . '%', 'LIKE');
    }
    // Get product data.
    $products = $query->orderByHeader($product_header)->execute()
      ->fetchAll();

    // Check if products is exists.
    if (!empty($products)) {
      foreach ($products as $product) {
        // Define total container.
        $status = array(
          'nid' => 0,
          'type' => t('unknown'),
          'status' => NULL,
          'stock' => 0,
          'all' => 0,
          'available' => 0,
          'taken' => 0,
          'used' => 0,
          'unused' => 0,
          'cart' => 0,
        );
        // Get node id based on: field_data_field_product.
        $query_node = db_select('field_data_field_product', 'fdfp');
        $query_node->addField('fdfp', 'entity_id');
        $query_node->condition('field_product_product_id', $product->product_id);
        $result = $query_node->execute()->fetchAssoc();

        // Check if there is result.
        if (!empty($result)) {
          // Get product wrapper.
          $product_wrapper = entity_metadata_wrapper('commerce_product', $product->product_id);
          // Get product stock.
          $status['stock'] = intval($product_wrapper->commerce_stock->value());
          // Get node wrapper.
          $node_wrapper = entity_metadata_wrapper('node', $result['entity_id']);
          // Set node id.
          $status['nid'] = $result['entity_id'];
          // Get published status.
          $status['status'] = $node_wrapper->status;
          // Get coupon type.
          $status['type'] = $node_wrapper->field_coupon_type->value();
          // Get total all coupon.
          $status['all'] = db_select('field_data_field_simple_coupon', 'fdfsc')
            ->condition('field_simple_coupon_target_id', $status['nid'])
            ->countQuery()
            ->execute()
            ->fetchField();
          // Get total query.
          $total_query = db_select('field_data_field_simple_coupon', 'fdfsc');
          $total_query->join('node', 'n', 'fdfsc.entity_id = n.nid');
          $total_query->condition('fdfsc.field_simple_coupon_target_id', $status['nid']);
          $total_query->addExpression('count(*)', 'taken');
          // Get total taken.
          $status['available'] = clone $total_query;
          $status['available'] = $status['available']
            ->condition('n.uid', 1)->execute()->fetchField();
          // Get total taken.
          $status['taken'] = clone $total_query;
          $status['taken'] = $status['taken']
            ->condition('n.uid', 1, '>')->execute()->fetchField();
          // Get total used.
          $status['used'] = clone $total_query;
          $status['used'] = $status['used']
            ->condition('n.status', 0)->execute()->fetchField();
          // Get total taken.
          $status['unused'] = clone $total_query;
          $status['unused'] = $status['unused']
            ->condition('n.uid', 1, '>')
            ->condition('n.status', 1)
            ->execute()->fetchField();
          // Get cart query.
          $cart_query = db_select('commerce_line_item', 'cli');
          $cart_query->join('commerce_order', 'co', 'cli.order_id = co.order_id');
          $cart_query->condition('cli.type', 'coupon');
          $cart_query->condition('cli.line_item_label', $product->sku);
          $cart_query->condition('co.status', $cart_statuses, 'IN');
          $cart_query->addExpression('count(*)', 'cart');
          $status['cart'] = $cart_query->execute()->fetchField();

          // Check order in cart only if status cart is not empty.
          // Only query order if there is 1 product only.
          if (!empty($status['cart']) && count($products) == 1) {
            $order_query = db_select('commerce_order', 'co')->extend('TableSort');
            $order_query->join('commerce_line_item', 'cli', 'cli.order_id = co.order_id');
            $order_query->fields('co', array(
              'order_id', 'uid', 'mail', 'status', 'created', 'changed',
            ));
            $order_query->condition('cli.type', 'coupon');
            $order_query->condition('cli.line_item_label', $product->sku);
            $order_query->condition('co.status', $cart_statuses, 'IN');
            $orders = $order_query
              ->orderByHeader($order_header)->execute()->fetchAll();
            if (!empty($orders)) {
              foreach ($orders as $order) {
                $payment = array(
                  'total' => 0,
                  'success' => 0,
                  'pending' => 0,
                  'failed' => 0,
                );
                // Define payment transaction query.
                $payment_query = db_select('commerce_payment_transaction', 'cpt');
                $payment_query->condition('cpt.order_id', $order->order_id);
                $payment_query->addExpression('count(*)', 'counter');
                // Get payment total.
                $payment['total'] = clone $payment_query;
                $payment['total'] = $payment['total']->execute()->fetchField();
                // Get payment success.
                $payment['success'] = clone $payment_query;
                $payment['success'] = $payment['success']
                  ->condition('cpt.status', 'success')->execute()->fetchField();
                // Get payment pending.
                $payment['pending'] = clone $payment_query;
                $payment['pending'] = $payment['pending']
                  ->condition('cpt.status', 'pending')->execute()->fetchField();
                // Get payment failed.
                $payment['failed'] = clone $payment_query;
                $payment['failed'] = $payment['failed']
                  ->condition('cpt.status', 'failure')->execute()->fetchField();

                // Store order data into table row.
                $order_rows[] = array(
                  'data' => array(
                    $product->product_id,
                    l($order->order_id, 'admin/commerce/orders/' . $order->order_id, array(
                      'attributes' => array('target' => '_blank'),
                    )),
                    format_date($order->created),
                    format_date($order->changed),
                    $order->uid, $order->mail, $order->status,
                    $payment['total'],
                    $payment['success'],
                    $payment['pending'],
                    $payment['failed'],
                  ),
                );
              }
            }
          }
        }

        // Store product into row data.
        $product_rows[] = array(
          'data' => array(
            $product->product_id, (!empty($status['nid'])) ? l($product->sku, 'node/' . $status['nid'] . '/edit', array('attributes' => array('target' => '_blank'))) : $product->sku,
            $product->title,
            ($product->status ? t('Active') : t('Inactive')),
            (is_null($status['status']) ? t('Unavailable') : (!empty($status['status']) ? t('Published') : t('Unpublished'))),
            (strpos($status['type'], '_') !== FALSE) ? $status['type'] : 'online_' . $status['type'],
            $status['stock'],
            $status['all'],
            $status['available'],
            $status['taken'],
            $status['used'],
            $status['unused'],
            $status['cart'],
          ),
        );
      }
    }
  }

  // Show product table.
  $build['product_table'] = array(
    '#theme' => 'table',
    '#header' => $product_header,
    '#rows' => $product_rows,
    '#empty' => t('There is no product match with your search criteria.'),
  );

  // Show order information.
  if (!empty($order_rows)) {
    $build['order_info'] = array(
      '#markup' => t('Displaying @total orders.<br />', array(
        '@total' => count($order_rows),
      )),
    );
    // Attach the pager table.
    $build['order_table'] = array(
      '#theme' => 'table',
      '#header' => $order_header,
      '#rows' => $order_rows,
      '#empty' => t('There is no product attach to order based on product search parameter.'),
    );
  }

  // Return build variables.
  return $build;
}

/**
 * Function sepulsa_reports_voucher_search_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function sepulsa_reports_voucher_search_form($form = array(), $form_state = array()) {
  $form['sku'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU'),
    '#title_display' => 'invisible',
    '#default_value' => isset($_SESSION['voucher']['search']['sku']) ? $_SESSION['voucher']['search']['sku'] : '',
    '#size' => 40,
    '#maxlength' => 255,
    '#attributes' => array(
      'placeholder' => t('SKU'),
    ),
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#title_display' => 'invisible',
    '#default_value' => isset($_SESSION['voucher']['search']['title']) ? $_SESSION['voucher']['search']['title'] : '',
    '#size' => 40,
    '#maxlength' => 255,
    '#attributes' => array(
      'placeholder' => t('Title'),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  $form['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('sepulsa_reports_voucher_search_form_reset'),
  );
  return $form;
}

/**
 * Function sepulsa_reports_voucher_search_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_reports_voucher_search_form_validate($form, &$form_state) {
  if (isset($form_state['values']['sku']) && empty($form_state['values']['sku'])
    && isset($form_state['values']['title']) && empty($form_state['values']['title'])
  ) {
    form_set_error('sku', t('No voucher search criteria, you need at least input voucher SKU.'));
  }
}

/**
 * Function sepulsa_reports_voucher_search_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_reports_voucher_search_form_submit($form, &$form_state) {
  $_SESSION['voucher']['search'] = array();
  if (isset($form_state['values']['sku']) && !empty($form_state['values']['sku'])) {
    $_SESSION['voucher']['search']['sku'] = $form_state['values']['sku'];
  }
  if (isset($form_state['values']['title']) && !empty($form_state['values']['title'])) {
    $_SESSION['voucher']['search']['title'] = $form_state['values']['title'];
  }
}

/**
 * Function sepulsa_reports_voucher_search_form_reset().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_reports_voucher_search_form_reset($form, &$form_state) {
  if (isset($_SESSION['voucher']['search'])) {
    unset($_SESSION['voucher']['search']);
  }
}

function sepulsa_reports_live_voucher_check() {
  // Define order cart statuses.
  $cart_statuses = array(
    'cart', 'checkout_checkout', 'checkout_payment',
  );
  // Define order table header.
  $order_header = array(
    array('data' => t('Product ID')),
    array('data' => t('Order ID'), 'field' => 'co.order_id'),
    array('data' => t('Created'), 'field' => 'co.created'),
    array('data' => t('Changed'), 'field' => 'co.changed'),
    array('data' => t('UID'), 'field' => 'co.uid'),
    array('data' => t('Mail'), 'field' => 'co.mail'),
    array('data' => t('Status'), 'field' => 'co.status'),
    array('data' => t('Transaction Total')),
    array('data' => t('Transaction Success')),
    array('data' => t('Transaction Pending')),
    array('data' => t('Transaction Failed')),
  );
  // Define order table row.
  $order_rows = array();

  // Define product order header.
  $product_header = array(
    array('data' => t('Voucher ID'), 'field' => 'cp.product_id'),
    array('data' => t('SKU'), 'field' => 'cp.sku'),
    array('data' => t('Title'), 'field' => 'cp.title'),
    array('data' => t('Status'), 'field' => 'cp.status'),
    array('data' => t('Published')),
    array('data' => t('Type')),
    array('data' => t('Product Stock')),
    array('data' => t('Total Stock')),
    array('data' => t('Total Available')),
    array('data' => t('Total Taken')),
    array('data' => t('Total Used')),
    array('data' => t('Total Unused')),
    array('data' => t('Total in Cart')),
    array('data' => t('Expiry')),
  );
  // Define product rows.
  $product_rows = array();

  // Define search parameter.
  $search = array();
  // Check voucher search sku.
  /*if (isset($_SESSION['voucher']['search']['sku']) && !empty($_SESSION['voucher']['search']['sku'])) {
    $search['sku'] = $_SESSION['voucher']['search']['sku'];
  }*/
  // Check voucher search title.
  /*if (isset($_SESSION['voucher']['search']['title']) && !empty($_SESSION['voucher']['search']['title'])) {
    $search['title'] = $_SESSION['voucher']['search']['title'];
  }*/
  // Get search form.
  //$build['search'] = drupal_get_form('sepulsa_reports_voucher_search_form');
  //$build['search']['#attributes']['class'][] = 'container-inline';

  // Only start query if there is search data.
  //if (!empty($search)) {
    // Query to get product voucher.
    $query = db_select('commerce_product', 'cp')->extend('TableSort');
    $query-> join('field_data_field_expiry_date', 'fed', 'fed.entity_id = cp.product_id');
    $query->fields('cp', array(
      'product_id', 'sku', 'title', 'status',
    ));
    $query->fields('fed', array(
      'field_expiry_date_value',
    ));
    //$query->range(0, 15);
    $query->condition('field_expiry_date_value', time(), '>');
    // Only for product type: coupon.
    $query->condition('type', 'coupon');
    // Search by SKU.
    //if (isset($search['sku'])) {
      //$query->condition('sku', $search['sku']);
    //}
    // Search by title.
    //if (isset($search['title'])) {
      //$query->condition('title', '%' . db_like($search['title']) . '%', 'LIKE');
    //}
    // Get product data.
    $products = $query->orderByHeader($product_header)->execute()
      ->fetchAll();
    // Check if products is exists.
    if (!empty($products)) {
      foreach ($products as $product) {
        // Define total container.
        $status = array(
          'nid' => 0,
          'type' => t('unknown'),
          'status' => NULL,
          'stock' => 0,
          'all' => 0,
          'available' => 0,
          'taken' => 0,
          'used' => 0,
          'unused' => 0,
          'cart' => 0,
        );
        // Get node id based on: field_data_field_product.
        $query_node = db_select('field_data_field_product', 'fdfp');
        $query_node->addField('fdfp', 'entity_id');
        $query_node->condition('field_product_product_id', $product->product_id);
        $result = $query_node->execute()->fetchAssoc();

        // Check if there is result.
        if (!empty($result)) {
          // Get product wrapper.
          $product_wrapper = entity_metadata_wrapper('commerce_product', $product->product_id);
          // Get product stock.
          $status['stock'] = intval($product_wrapper->commerce_stock->value());
          // Get node wrapper.
          $node_wrapper = entity_metadata_wrapper('node', $result['entity_id']);
          // Set node id.
          $status['nid'] = $result['entity_id'];
          // Get published status.
          $status['status'] = $node_wrapper->status;
          // Get coupon type.
          $status['type'] = $node_wrapper->field_coupon_type->value();
          // Get total all coupon.
          $status['all'] = db_select('field_data_field_simple_coupon', 'fdfsc')
            ->condition('field_simple_coupon_target_id', $status['nid'])
            ->countQuery()
            ->execute()
            ->fetchField();
          // Get total query.
          $total_query = db_select('field_data_field_simple_coupon', 'fdfsc');
          $total_query->join('node', 'n', 'fdfsc.entity_id = n.nid');
          $total_query->condition('fdfsc.field_simple_coupon_target_id', $status['nid']);
          $total_query->addExpression('count(*)', 'taken');
          // Get total taken.
          $status['available'] = clone $total_query;
          $status['available'] = $status['available']
            ->condition('n.uid', 1)->execute()->fetchField();
          // Get total taken.
          $status['taken'] = clone $total_query;
          $status['taken'] = $status['taken']
            ->condition('n.uid', 1, '>')->execute()->fetchField();
          // Get total used.
          $status['used'] = clone $total_query;
          $status['used'] = $status['used']
            ->condition('n.status', 0)->execute()->fetchField();
          // Get total taken.
          $status['unused'] = clone $total_query;
          $status['unused'] = $status['unused']
            ->condition('n.uid', 1, '>')
            ->condition('n.status', 1)
            ->execute()->fetchField();
          // Get cart query.
          $cart_query = db_select('commerce_line_item', 'cli');
          $cart_query->join('commerce_order', 'co', 'cli.order_id = co.order_id');
          $cart_query->condition('cli.type', 'coupon');
          $cart_query->condition('cli.line_item_label', $product->sku);
          $cart_query->condition('co.status', $cart_statuses, 'IN');
          $cart_query->addExpression('count(*)', 'cart');
          $status['cart'] = $cart_query->execute()->fetchField();

          //Get expiry date
          $status['expiry']=gmdate("d-m-Y", $product->field_expiry_date_value);

          // Check order in cart only if status cart is not empty.
          // Only query order if there is 1 product only.
          if (!empty($status['cart']) && count($products) == 1) {
            $order_query = db_select('commerce_order', 'co')->extend('TableSort');
            $order_query->join('commerce_line_item', 'cli', 'cli.order_id = co.order_id');
            $order_query->fields('co', array(
              'order_id', 'uid', 'mail', 'status', 'created', 'changed',
            ));
            $order_query->condition('cli.type', 'coupon');
            $order_query->condition('cli.line_item_label', $product->sku);
            $order_query->condition('co.status', $cart_statuses, 'IN');
            $orders = $order_query
              ->orderByHeader($order_header)->execute()->fetchAll();
            if (!empty($orders)) {
              foreach ($orders as $order) {
                $payment = array(
                  'total' => 0,
                  'success' => 0,
                  'pending' => 0,
                  'failed' => 0,
                );
                // Define payment transaction query.
                $payment_query = db_select('commerce_payment_transaction', 'cpt');
                $payment_query->condition('cpt.order_id', $order->order_id);
                $payment_query->addExpression('count(*)', 'counter');
                // Get payment total.
                $payment['total'] = clone $payment_query;
                $payment['total'] = $payment['total']->execute()->fetchField();
                // Get payment success.
                $payment['success'] = clone $payment_query;
                $payment['success'] = $payment['success']
                  ->condition('cpt.status', 'success')->execute()->fetchField();
                // Get payment pending.
                $payment['pending'] = clone $payment_query;
                $payment['pending'] = $payment['pending']
                  ->condition('cpt.status', 'pending')->execute()->fetchField();
                // Get payment failed.
                $payment['failed'] = clone $payment_query;
                $payment['failed'] = $payment['failed']
                  ->condition('cpt.status', 'failure')->execute()->fetchField();

                // Store order data into table row.
                $order_rows[] = array(
                  'data' => array(
                    $product->product_id,
                    l($order->order_id, 'admin/commerce/orders/' . $order->order_id, array(
                      'attributes' => array('target' => '_blank'),
                    )),
                    format_date($order->created),
                    format_date($order->changed),
                    $order->uid, $order->mail, $order->status,
                    $payment['total'],
                    $payment['success'],
                    $payment['pending'],
                    $payment['failed'],
                  ),
                );
              }
            }
          }
        }

        // Store product into row data.
        $product_rows[] = array(
          'data' => array(
            $product->product_id, (!empty($status['nid'])) ? l($product->sku, 'node/' . $status['nid'] . '/edit', array('attributes' => array('target' => '_blank'))) : $product->sku,
            $product->title,
            ($product->status ? t('Active') : t('Inactive')),
            (is_null($status['status']) ? t('Unavailable') : (!empty($status['status']) ? t('Published') : t('Unpublished'))),
            (strpos($status['type'], '_') !== FALSE) ? $status['type'] : 'online_' . $status['type'],
            $status['stock'],
            $status['all'],
            $status['available'],
            $status['taken'],
            $status['used'],
            $status['unused'],
            $status['cart'],
            $status['expiry'],
          ),
        );
      }
    }
  //}

  // Show product table.
  $build['product_table'] = array(
    '#theme' => 'table',
    '#header' => $product_header,
    '#rows' => $product_rows,
    '#empty' => t('There is no product match with your search criteria.'),
  );

  // Show order information.
  /*if (!empty($order_rows)) {
    $build['order_info'] = array(
      '#markup' => t('Displaying @total orders.<br />', array(
        '@total' => count($order_rows),
      )),
    );
    // Attach the pager table.
    $build['order_table'] = array(
      '#theme' => 'table',
      '#header' => $order_header,
      '#rows' => $order_rows,
      '#empty' => t('There is no product attach to order based on product search parameter.'),
    );
  }*/

  // Return build variables.
  return $build;
}

