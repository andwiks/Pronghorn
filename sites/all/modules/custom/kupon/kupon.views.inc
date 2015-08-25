<?php

/**
 * Implements hook_views_default_views().
 */
function kupon_views_default_views() {
  // Begin copy and paste of output from the Export tab of a view.
  $view = new view();
  $view->name = 'coupon_report';
  $view->description = '';
  $view->tag = 'coupon, sepulsa, commerce_reports';
  $view->base_table = 'node';
  $view->human_name = 'Coupon Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_coupon_type' => 'field_coupon_type',
    'field_simple_coupon' => 'field_simple_coupon',
    'field_order_id' => 'field_order_id',
    'expression' => 'expression',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_coupon_type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_simple_coupon' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_order_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'expression' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_simple_coupon_node']['id'] = 'reverse_field_simple_coupon_node';
  $handler->display->display_options['relationships']['reverse_field_simple_coupon_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_simple_coupon_node']['field'] = 'reverse_field_simple_coupon_node';
  $handler->display->display_options['relationships']['reverse_field_simple_coupon_node']['label'] = 'Owned Coupon';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Coupon Type */
  $handler->display->display_options['fields']['field_coupon_type']['id'] = 'field_coupon_type';
  $handler->display->display_options['fields']['field_coupon_type']['table'] = 'field_data_field_coupon_type';
  $handler->display->display_options['fields']['field_coupon_type']['field'] = 'field_coupon_type';
  /* Field: COUNT(Content: Simple Coupon) */
  $handler->display->display_options['fields']['field_simple_coupon']['id'] = 'field_simple_coupon';
  $handler->display->display_options['fields']['field_simple_coupon']['table'] = 'field_data_field_simple_coupon';
  $handler->display->display_options['fields']['field_simple_coupon']['field'] = 'field_simple_coupon';
  $handler->display->display_options['fields']['field_simple_coupon']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_simple_coupon']['label'] = 'Stock';
  $handler->display->display_options['fields']['field_simple_coupon']['separator'] = '';
  /* Field: COUNT(Content: Order ID) */
  $handler->display->display_options['fields']['field_order_id']['id'] = 'field_order_id';
  $handler->display->display_options['fields']['field_order_id']['table'] = 'field_data_field_order_id';
  $handler->display->display_options['fields']['field_order_id']['field'] = 'field_order_id';
  $handler->display->display_options['fields']['field_order_id']['relationship'] = 'reverse_field_simple_coupon_node';
  $handler->display->display_options['fields']['field_order_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_order_id']['label'] = 'Owned';
  $handler->display->display_options['fields']['field_order_id']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Left';
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[field_simple_coupon] - [field_order_id]';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Rubah';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'coupon' => 'coupon',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Active Time -  start date (field_active_time) */
  $handler->display->display_options['filters']['field_active_time_value']['id'] = 'field_active_time_value';
  $handler->display->display_options['filters']['field_active_time_value']['table'] = 'field_data_field_active_time';
  $handler->display->display_options['filters']['field_active_time_value']['field'] = 'field_active_time_value';
  $handler->display->display_options['filters']['field_active_time_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_active_time_value']['group'] = 1;
  $handler->display->display_options['filters']['field_active_time_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_active_time_value']['expose']['operator_id'] = 'field_active_time_value_op';
  $handler->display->display_options['filters']['field_active_time_value']['expose']['label'] = 'Active start';
  $handler->display->display_options['filters']['field_active_time_value']['expose']['operator'] = 'field_active_time_value_op';
  $handler->display->display_options['filters']['field_active_time_value']['expose']['identifier'] = 'field_active_time_value';
  $handler->display->display_options['filters']['field_active_time_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_active_time_value']['form_type'] = 'date_popup';
  /* Filter criterion: Content: Active Time - end date (field_active_time:value2) */
  $handler->display->display_options['filters']['field_active_time_value2']['id'] = 'field_active_time_value2';
  $handler->display->display_options['filters']['field_active_time_value2']['table'] = 'field_data_field_active_time';
  $handler->display->display_options['filters']['field_active_time_value2']['field'] = 'field_active_time_value2';
  $handler->display->display_options['filters']['field_active_time_value2']['operator'] = '<=';
  $handler->display->display_options['filters']['field_active_time_value2']['group'] = 1;
  $handler->display->display_options['filters']['field_active_time_value2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_active_time_value2']['expose']['operator_id'] = 'field_active_time_value2_op';
  $handler->display->display_options['filters']['field_active_time_value2']['expose']['label'] = 'Active end';
  $handler->display->display_options['filters']['field_active_time_value2']['expose']['operator'] = 'field_active_time_value2_op';
  $handler->display->display_options['filters']['field_active_time_value2']['expose']['identifier'] = 'field_active_time_value2';
  $handler->display->display_options['filters']['field_active_time_value2']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_active_time_value2']['form_type'] = 'date_popup';
  /* Filter criterion: Content: Coupon Type (field_coupon_type) */
  $handler->display->display_options['filters']['field_coupon_type_value']['id'] = 'field_coupon_type_value';
  $handler->display->display_options['filters']['field_coupon_type_value']['table'] = 'field_data_field_coupon_type';
  $handler->display->display_options['filters']['field_coupon_type_value']['field'] = 'field_coupon_type_value';
  $handler->display->display_options['filters']['field_coupon_type_value']['group'] = 1;
  $handler->display->display_options['filters']['field_coupon_type_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_coupon_type_value']['expose']['operator_id'] = 'field_coupon_type_value_op';
  $handler->display->display_options['filters']['field_coupon_type_value']['expose']['label'] = 'Coupon Type';
  $handler->display->display_options['filters']['field_coupon_type_value']['expose']['operator'] = 'field_coupon_type_value_op';
  $handler->display->display_options['filters']['field_coupon_type_value']['expose']['identifier'] = 'field_coupon_type_value';
  $handler->display->display_options['filters']['field_coupon_type_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_coupon_type_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/reports/coupon';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Coupon';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['coupon_report'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Owned Coupon'),
    t('Title'),
    t('Coupon Type'),
    t('Stock'),
    t('Owned'),
    t('Left'),
    t('.'),
    t(','),
    t('Rubah'),
    t('Active start'),
    t('Active end'),
    t('Page'),
  );

  // (Export ends here.)

  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;
}

/**
 * Implements hook_views_query_alter().
 */
function kupon_views_query_alter(&$view, &$query) {
  if ($view->name == 'coupon' && ($view->current_display == 'page' || $view->current_display == 'coupon')) {
    //getting total price on cart
    global $user;
    $order = commerce_cart_order_load($user->uid);
    if (empty($order->commerce_line_items)) {
      //don't allow access coupon page if cart empty
      //drupal_goto("<front>");
    }
    $wrapper = entity_metadata_wrapper('commerce_order', $order);
    $line_items = $wrapper->commerce_line_items;
    $total = commerce_line_items_total($line_items);

    //if more then 100k, default it to 100k
    //if ($total['amount'] > 100000) $total['amount'] = 100000;

    //change the views SQL here
    $query->where[1]['conditions'][4]['value'] = $total['amount'];
    $query->where[1]['conditions'][5]['value'] = $total['amount'];
  }
}

/**
 * Implements hook_preprocess_views_view_table().
 */
function kupon_preprocess_views_view_table(&$variables) {
  $view = $variables['view'];

  if ($view->name == 'coupon_report') {
    foreach ($variables['result'] as $key => $result) {
      if ($result->field_data_field_coupon_type_field_coupon_type_value == 'simple') {
        $variables['rows'][$key]['field_simple_coupon'] = 'N/A';
        $variables['rows'][$key]['expression'] = 'N/A';
      }
    }
  }
}
