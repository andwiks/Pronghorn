<?php
/**
 * Implements hook_views_default_views().
 */
function merchant_dashboard_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'merchant_reporting';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Merchant Reporting';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Voucher';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    12 => '12',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_unique_coupon_code' => 'field_unique_coupon_code',
    'status' => 'status',
    'changed' => 'changed',
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
    'field_unique_coupon_code' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<style>
  .views-field {
      padding: 10px;
  }
  table {
      width: 98%;
  }
  </style>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Voucher - Redeemed Code';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'div';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Unique Coupon Code */
  $handler->display->display_options['fields']['field_unique_coupon_code']['id'] = 'field_unique_coupon_code';
  $handler->display->display_options['fields']['field_unique_coupon_code']['table'] = 'field_data_field_unique_coupon_code';
  $handler->display->display_options['fields']['field_unique_coupon_code']['field'] = 'field_unique_coupon_code';
  $handler->display->display_options['fields']['field_unique_coupon_code']['label'] = 'Unique Code';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Redeemed';
  $handler->display->display_options['fields']['status']['type'] = 'custom';
  $handler->display->display_options['fields']['status']['type_custom_true'] = 'No';
  $handler->display->display_options['fields']['status']['type_custom_false'] = 'Yes';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'long';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  $handler->display->display_options['fields']['changed']['format_date_sql'] = 0;
  /* Sort criterion: Content: Published */
  $handler->display->display_options['sorts']['status']['id'] = 'status';
  $handler->display->display_options['sorts']['status']['table'] = 'node';
  $handler->display->display_options['sorts']['status']['field'] = 'status';
  /* Sort criterion: Content: Updated date */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'node';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  /* Contextual filter: Content: Simple Coupon (field_simple_coupon) */
  $handler->display->display_options['arguments']['field_simple_coupon_target_id']['id'] = 'field_simple_coupon_target_id';
  $handler->display->display_options['arguments']['field_simple_coupon_target_id']['table'] = 'field_data_field_simple_coupon';
  $handler->display->display_options['arguments']['field_simple_coupon_target_id']['field'] = 'field_simple_coupon_target_id';
  $handler->display->display_options['arguments']['field_simple_coupon_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_simple_coupon_target_id']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['field_simple_coupon_target_id']['breadcrumb'] = 'Merchant Dashboard';
  $handler->display->display_options['arguments']['field_simple_coupon_target_id']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['field_simple_coupon_target_id']['default_argument_options']['index'] = '2';
  $handler->display->display_options['arguments']['field_simple_coupon_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_simple_coupon_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_simple_coupon_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'offline_owned_simple' => 'offline_owned_simple',
    'offline_owned_unique' => 'offline_owned_unique',
    'owned_simple_coupon' => 'owned_simple_coupon',
    'owned_unique_coupon' => 'owned_unique_coupon',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Author uid */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'node';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['operator'] = 'not in';
  $handler->display->display_options['filters']['uid']['value'] = array(
    0 => '1',
  );
  $handler->display->display_options['filters']['uid']['group'] = 1;
  /* Filter criterion: Content: Unique Coupon Code (field_unique_coupon_code) */
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['id'] = 'field_unique_coupon_code_value';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['table'] = 'field_data_field_unique_coupon_code';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['field'] = 'field_unique_coupon_code_value';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['operator_id'] = 'field_unique_coupon_code_value_op';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['label'] = 'Unique Code';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['operator'] = 'field_unique_coupon_code_value_op';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['identifier'] = 'field_unique_coupon_code_value';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
    17 => 0,
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Voucher */
  $handler = $view->new_display('page', 'Voucher', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'offline_owned_unique' => 'offline_owned_unique',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Author uid */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'node';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['operator'] = 'not in';
  $handler->display->display_options['filters']['uid']['value'] = array(
    0 => '1',
  );
  $handler->display->display_options['filters']['uid']['group'] = 1;
  /* Filter criterion: Content: Unique Coupon Code (field_unique_coupon_code) */
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['id'] = 'field_unique_coupon_code_value';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['table'] = 'field_data_field_unique_coupon_code';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['field'] = 'field_unique_coupon_code_value';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['operator_id'] = 'field_unique_coupon_code_value_op';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['label'] = 'Unique Code';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['operator'] = 'field_unique_coupon_code_value_op';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['identifier'] = 'field_unique_coupon_code_value';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
    17 => 0,
  );
  $handler->display->display_options['path'] = 'merchant_dashboard/voucher';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Platinum Voucher';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-champ-report';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Data Voucher */
  $handler = $view->new_display('views_data_export', 'Data Voucher', 'views_data_export_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xls';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = 'voucher%.xls';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'offline_owned_unique' => 'offline_owned_unique',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Author uid */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'node';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['operator'] = 'not in';
  $handler->display->display_options['filters']['uid']['value'] = array(
    0 => '1',
  );
  $handler->display->display_options['filters']['uid']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Redeem Code';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
  );
  $handler->display->display_options['path'] = 'merchant_dashboard/voucher/%/export';
  $handler->display->display_options['displays'] = array(
    'page_2' => 'page_2',
    'default' => 0,
    'page' => 0,
    'page_1' => 0,
    'page_3' => 0,
    'page_4' => 0,
  );
  $handler->display->display_options['sitename_title'] = 0;

  /* Display: Page All */
  $handler = $view->new_display('page', 'Page All', 'page_1');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Voucher - Redeemed Code';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'div';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Unique Coupon Code */
  $handler->display->display_options['fields']['field_unique_coupon_code']['id'] = 'field_unique_coupon_code';
  $handler->display->display_options['fields']['field_unique_coupon_code']['table'] = 'field_data_field_unique_coupon_code';
  $handler->display->display_options['fields']['field_unique_coupon_code']['field'] = 'field_unique_coupon_code';
  $handler->display->display_options['fields']['field_unique_coupon_code']['label'] = 'Kode Unik';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Telah di redeem';
  $handler->display->display_options['fields']['status']['type'] = 'custom';
  $handler->display->display_options['fields']['status']['type_custom_true'] = 'No';
  $handler->display->display_options['fields']['status']['type_custom_false'] = 'Yes';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Tanggal';
  $handler->display->display_options['fields']['changed']['date_format'] = 'custom';
  $handler->display->display_options['fields']['changed']['custom_date_format'] = 'D, d M Y ';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  $handler->display->display_options['fields']['changed']['timezone'] = 'Asia/Jakarta';
  $handler->display->display_options['fields']['changed']['format_date_sql'] = 0;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'offline_owned_simple' => 'offline_owned_simple',
    'offline_owned_unique' => 'offline_owned_unique',
    'owned_simple_coupon' => 'owned_simple_coupon',
    'owned_unique_coupon' => 'owned_unique_coupon',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Author uid */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'node';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['operator'] = 'not in';
  $handler->display->display_options['filters']['uid']['value'] = array(
    0 => '1',
  );
  $handler->display->display_options['filters']['uid']['group'] = 1;
  /* Filter criterion: Content: Unique Coupon Code (field_unique_coupon_code) */
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['id'] = 'field_unique_coupon_code_value';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['table'] = 'field_data_field_unique_coupon_code';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['field'] = 'field_unique_coupon_code_value';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['operator_id'] = 'field_unique_coupon_code_value_op';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['label'] = 'Unique Code';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['operator'] = 'field_unique_coupon_code_value_op';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['identifier'] = 'field_unique_coupon_code_value';
  $handler->display->display_options['filters']['field_unique_coupon_code_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
    17 => 0,
  );
  $handler->display->display_options['path'] = 'merchant_dashboard/taken/%';

  /* Display: Page redeemed */
  $handler = $view->new_display('page', 'Page redeemed', 'page_3');
  $handler->display->display_options['path'] = 'merchant_dashboard/redeem/%';

  /* Display: Page notredeem */
  $handler = $view->new_display('page', 'Page notredeem', 'page_4');
  $handler->display->display_options['path'] = 'merchant_dashboard/notredeem/%';
  $translatables['merchant_reporting'] = array(
    t('Master'),
    t('Voucher'),
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
    t('<style>
  .views-field {
      padding: 10px;
  }
  table {
      width: 98%;
  }
  </style>'),
    t('Displaying @start - @end of @total'),
    t('Voucher - Redeemed Code'),
    t('Unique Code'),
    t('Redeemed'),
    t('No'),
    t('Yes'),
    t('Updated date'),
    t('All'),
    t('Merchant Dashboard'),
    t('Data Voucher'),
    t('Redeem Code'),
    t('Page All'),
    t('Kode Unik'),
    t('Telah di redeem'),
    t('Tanggal'),
    t('Page redeemed'),
    t('Page notredeem'),
  );

  $views[$view->name] = $view;

  return $views;
}
