<?php

/**
 * Implements hook_views_default_views().
 */
function sepulsa_invoice_views_default_views() {
  // Begin copy and paste of output from the Export tab of a view.
  $view = new view();
  $view->name = 'sepulsa_invoice_line_items';
  $view->tag = 'sepulsa';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Invoice line items';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Invoice list items';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_tags'] = array(
    0 => 'sepulsa_invoice_line_items',
  );
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'field_phone_number' => 'field_phone_number',
    'title' => 'title',
    'field_credit_amount' => 'field_credit_amount',
    'line_item_title' => 'line_item_title',
    'commerce_unit_price' => 'commerce_unit_price',
    'quantity' => 'quantity',
    'commerce_total' => 'commerce_total',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_phone_number' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_credit_amount' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'line_item_title' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_unit_price' => array(
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
    'commerce_total' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-right',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Commerce Order: Referenced line items */
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['label'] = 'Line items';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
  /* Relationship: Commerce Line item: Referenced products */
  $handler->display->display_options['relationships']['commerce_product_product_id']['id'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['table'] = 'field_data_commerce_product';
  $handler->display->display_options['relationships']['commerce_product_product_id']['field'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['label'] = 'Products';
  /* Relationship: Commerce Product: Referencing Node */
  $handler->display->display_options['relationships']['field_product']['id'] = 'field_product';
  $handler->display->display_options['relationships']['field_product']['table'] = 'commerce_product';
  $handler->display->display_options['relationships']['field_product']['field'] = 'field_product';
  $handler->display->display_options['relationships']['field_product']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['field_product']['label'] = 'Node';
  /* Field: Commerce Line item: Phone Number */
  $handler->display->display_options['fields']['field_phone_number']['id'] = 'field_phone_number';
  $handler->display->display_options['fields']['field_phone_number']['table'] = 'field_data_field_phone_number';
  $handler->display->display_options['fields']['field_phone_number']['field'] = 'field_phone_number';
  $handler->display->display_options['fields']['field_phone_number']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['field_phone_number']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_phone_number']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_phone_number']['alter']['text'] = '([field_phone_number])';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_product';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Commerce Product: Credit Amount */
  $handler->display->display_options['fields']['field_credit_amount']['id'] = 'field_credit_amount';
  $handler->display->display_options['fields']['field_credit_amount']['table'] = 'field_data_field_credit_amount';
  $handler->display->display_options['fields']['field_credit_amount']['field'] = 'field_credit_amount';
  $handler->display->display_options['fields']['field_credit_amount']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['field_credit_amount']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_credit_amount']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_credit_amount']['alter']['text'] = 'Rp [field_credit_amount]';
  $handler->display->display_options['fields']['field_credit_amount']['settings'] = array(
    'thousand_separator' => '.',
    'prefix_suffix' => 1,
  );
  /* Field: Commerce Line Item: Title */
  $handler->display->display_options['fields']['line_item_title']['id'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_title']['field'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['line_item_title']['label'] = 'Product';
  $handler->display->display_options['fields']['line_item_title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['line_item_title']['alter']['text'] = '[title] [field_credit_amount] [field_phone_number]';
  $handler->display->display_options['fields']['line_item_title']['element_class'] = 'product-name';
  $handler->display->display_options['fields']['line_item_title']['element_default_classes'] = FALSE;
  /* Field: Commerce Line item: Unit price */
  $handler->display->display_options['fields']['commerce_unit_price']['id'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['table'] = 'field_data_commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['field'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_unit_price']['label'] = 'Price';
  $handler->display->display_options['fields']['commerce_unit_price']['element_class'] = 'product-price';
  $handler->display->display_options['fields']['commerce_unit_price']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_unit_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_unit_price']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Commerce Line Item: Jumlah */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['quantity']['label'] = 'Quantity';
  $handler->display->display_options['fields']['quantity']['element_class'] = 'product-price';
  $handler->display->display_options['fields']['quantity']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  /* Field: Commerce Line item: Total */
  $handler->display->display_options['fields']['commerce_total']['id'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['table'] = 'field_data_commerce_total';
  $handler->display->display_options['fields']['commerce_total']['field'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_total']['element_class'] = 'product-subtotal text-right';
  $handler->display->display_options['fields']['commerce_total']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_total']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['commerce_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_total']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Sort criterion: Commerce Line Item: Line item ID */
  $handler->display->display_options['sorts']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['sorts']['line_item_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['sorts']['line_item_id']['field'] = 'line_item_id';
  $handler->display->display_options['sorts']['line_item_id']['relationship'] = 'commerce_line_items_line_item_id';
  /* Contextual filter: Commerce Order: Order ID */
  $handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Commerce Line Item: Line item is of a product line item type */
  $handler->display->display_options['filters']['product_line_item_type']['id'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['product_line_item_type']['field'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['filters']['product_line_item_type']['value'] = '1';

  /* Display: Rules */
  $handler = $view->new_display('views_rules', 'Rules', 'views_rules');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['rules_parameter'] = array(
    'order_id' => array(
      'enabled' => 1,
      'type' => 'integer',
      'label' => 'Commerce Order: Order ID',
      'name' => 'order_id',
    ),
  );
  $handler->display->display_options['rules_variables'] = array(
    'field_phone_number' => array(
      'enabled' => 0,
      'rendered' => 0,
      'type' => 'text',
      'label' => '(Line items referenced by commerce_line_items) Phone Number',
      'name' => 'field_phone_number',
    ),
    'title' => array(
      'enabled' => 0,
      'rendered' => 0,
      'type' => 'text',
      'label' => '(Node referencing products from field_product) Title',
      'name' => 'title',
    ),
    'field_credit_amount' => array(
      'enabled' => 0,
      'rendered' => 0,
      'type' => 'text',
      'label' => '(Products referenced by commerce_product) Credit Amount',
      'name' => 'field_credit_amount',
    ),
    'line_item_title' => array(
      'enabled' => 1,
      'rendered' => 1,
      'type' => 'text',
      'label' => 'Product',
      'name' => 'line_item_title',
    ),
    'commerce_unit_price' => array(
      'enabled' => 1,
      'rendered' => 1,
      'type' => 'text',
      'label' => 'Price',
      'name' => 'commerce_unit_price',
    ),
    'quantity' => array(
      'enabled' => 1,
      'rendered' => 1,
      'type' => 'integer',
      'label' => 'Quantity',
      'name' => 'quantity',
    ),
    'commerce_total' => array(
      'enabled' => 1,
      'rendered' => 1,
      'type' => 'text',
      'label' => 'Total',
      'name' => 'commerce_total',
    ),
    'commerce_order_total' => array(
      'enabled' => 1,
      'rendered' => 1,
      'type' => 'text',
      'label' => 'Order total',
      'name' => 'commerce_order_total',
    ),
  );
  $translatables['invoice_list_items'] = array(
    t('Defaults'),
    t('Invoice list items'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Line items'),
    t('Products'),
    t('Node'),
    t('Phone Number'),
    t('([field_phone_number])'),
    t('Title'),
    t('Credit Amount'),
    t('Rp [field_credit_amount]'),
    t('Product'),
    t('[title] [field_credit_amount] [field_phone_number]'),
    t('Price'),
    t('Quantity'),
    t('.'),
    t(','),
    t('Total'),
    t('All'),
    t('Rules'),
  );

  // (Export ends here.)

  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;
}
