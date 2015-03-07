<?php
/**
 * @file
 * topup.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function topup_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'line_item_topup';
  $view->description = 'Display line item topup.';
  $view->tag = 'topup';
  $view->base_table = 'commerce_line_item';
  $view->human_name = 'Line Item Topup';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Line Item Topup';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'line_item_id' => 'line_item_id',
    'order_id' => 'order_id',
    'status' => 'status',
    'field_phone_number' => 'field_phone_number',
    'commerce_product' => 'commerce_product',
    'quantity' => 'quantity',
    'field_topup_data' => 'field_topup_data',
    'field_topup_status' => 'field_topup_status',
    'commerce_total' => 'commerce_total',
    'topup_operation' => 'topup_operation',
  );
  $handler->display->display_options['style_options']['default'] = 'line_item_id';
  $handler->display->display_options['style_options']['info'] = array(
    'line_item_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_phone_number' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_product' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'quantity' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_topup_data' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_topup_status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_total' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'topup_operation' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['caption'] = 'Topup List';
  $handler->display->display_options['style_options']['summary'] = 'Line Item Topup';
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* Relationship: Commerce Line Item: Order ID */
  $handler->display->display_options['relationships']['order_id']['id'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['relationships']['order_id']['field'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['required'] = TRUE;
  /* Field: Commerce Line Item: Line item ID */
  $handler->display->display_options['fields']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['fields']['line_item_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_id']['field'] = 'line_item_id';
  /* Field: Commerce Line Item: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['order_id']['alter']['path'] = 'admin/commerce/orders/[order_id]';
  $handler->display->display_options['fields']['order_id']['alter']['target'] = '_blank';
  /* Field: Commerce Order: Order status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['relationship'] = 'order_id';
  /* Field: Commerce Line item: Phone Number */
  $handler->display->display_options['fields']['field_phone_number']['id'] = 'field_phone_number';
  $handler->display->display_options['fields']['field_phone_number']['table'] = 'field_data_field_phone_number';
  $handler->display->display_options['fields']['field_phone_number']['field'] = 'field_phone_number';
  /* Field: Commerce Line item: Product */
  $handler->display->display_options['fields']['commerce_product']['id'] = 'commerce_product';
  $handler->display->display_options['fields']['commerce_product']['table'] = 'field_data_commerce_product';
  $handler->display->display_options['fields']['commerce_product']['field'] = 'commerce_product';
  $handler->display->display_options['fields']['commerce_product']['label'] = 'Produk';
  $handler->display->display_options['fields']['commerce_product']['type'] = 'commerce_product_reference_rendered_product';
  $handler->display->display_options['fields']['commerce_product']['settings'] = array(
    'view_mode' => 'full',
    'page' => 1,
  );
  /* Field: Commerce Line Item: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  /* Field: Commerce Line item: Topup Data */
  $handler->display->display_options['fields']['field_topup_data']['id'] = 'field_topup_data';
  $handler->display->display_options['fields']['field_topup_data']['table'] = 'field_data_field_topup_data';
  $handler->display->display_options['fields']['field_topup_data']['field'] = 'field_topup_data';
  $handler->display->display_options['fields']['field_topup_data']['type'] = 'topup_data_formatter';
  $handler->display->display_options['fields']['field_topup_data']['delta_offset'] = '0';
  /* Field: Commerce Line item: Topup Status */
  $handler->display->display_options['fields']['field_topup_status']['id'] = 'field_topup_status';
  $handler->display->display_options['fields']['field_topup_status']['table'] = 'field_data_field_topup_status';
  $handler->display->display_options['fields']['field_topup_status']['field'] = 'field_topup_status';
  /* Field: Commerce Line item: Total */
  $handler->display->display_options['fields']['commerce_total']['id'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['table'] = 'field_data_commerce_total';
  $handler->display->display_options['fields']['commerce_total']['field'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_total']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Topup: Topup Operation */
  $handler->display->display_options['fields']['topup_operation']['id'] = 'topup_operation';
  $handler->display->display_options['fields']['topup_operation']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['topup_operation']['field'] = 'topup_operation';
  /* Filter criterion: Commerce Line Item: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'mobile_prepaid' => 'mobile_prepaid',
  );
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'order_id';
  $handler->display->display_options['filters']['status']['value'] = array(
    'pending' => 'pending',
    'processing' => 'processing',
  );
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Order status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Commerce Line item: Topup Status (field_topup_status) */
  $handler->display->display_options['filters']['field_topup_status_value']['id'] = 'field_topup_status_value';
  $handler->display->display_options['filters']['field_topup_status_value']['table'] = 'field_data_field_topup_status';
  $handler->display->display_options['filters']['field_topup_status_value']['field'] = 'field_topup_status_value';
  $handler->display->display_options['filters']['field_topup_status_value']['value'] = array(
    2 => '2',
    0 => '0',
    3 => '3',
    7 => '7',
  );
  $handler->display->display_options['filters']['field_topup_status_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_topup_status_value']['expose']['operator_id'] = 'field_topup_status_value_op';
  $handler->display->display_options['filters']['field_topup_status_value']['expose']['label'] = 'Topup Status';
  $handler->display->display_options['filters']['field_topup_status_value']['expose']['operator'] = 'field_topup_status_value_op';
  $handler->display->display_options['filters']['field_topup_status_value']['expose']['identifier'] = 'field_topup_status_value';
  $handler->display->display_options['filters']['field_topup_status_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_topup_status_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Commerce Line item: Phone Number (field_phone_number) */
  $handler->display->display_options['filters']['field_phone_number_value']['id'] = 'field_phone_number_value';
  $handler->display->display_options['filters']['field_phone_number_value']['table'] = 'field_data_field_phone_number';
  $handler->display->display_options['filters']['field_phone_number_value']['field'] = 'field_phone_number_value';
  $handler->display->display_options['filters']['field_phone_number_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_phone_number_value']['expose']['operator_id'] = 'field_phone_number_value_op';
  $handler->display->display_options['filters']['field_phone_number_value']['expose']['label'] = 'Phone Number';
  $handler->display->display_options['filters']['field_phone_number_value']['expose']['operator'] = 'field_phone_number_value_op';
  $handler->display->display_options['filters']['field_phone_number_value']['expose']['identifier'] = 'field_phone_number_value';
  $handler->display->display_options['filters']['field_phone_number_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/orders/line-item-topup';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Top Up';
  $handler->display->display_options['menu']['weight'] = '15';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  return $views;
}
