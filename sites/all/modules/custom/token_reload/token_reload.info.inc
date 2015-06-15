<?php

/**
 * @file
 * token_reload.info.inc
 */

/**
 * Implements hook_entity_property_info_alter().
 */
function token_reload_entity_property_info_alter(&$info) {
  $electricity_prepaid = &$info['commerce_line_item']['bundles']['electricity_prepaid']['properties'];

  $electricity_prepaid['ref_2'] = array(
    'label' => t('Ref 2'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $electricity_prepaid['ref_3'] = array(
    'label' => t('Ref 3'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $electricity_prepaid['meter_number'] = array(
    'label' => t('Meter Number'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $electricity_prepaid['customer_number'] = array(
    'label' => t('Customer Number'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $electricity_prepaid['customer_name'] = array(
    'label' => t('Customer Name'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $electricity_prepaid['subscriber_segmentation'] = array(
    'label' => t('Subscriber Segmentation'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $electricity_prepaid['power_consuming_category'] = array(
    'label' => t('Power Consuming Category'),
    'type' => 'integer',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $electricity_prepaid['purchased_kwh_unit'] = array(
    'label' => t('Purchased KWh Unit'),
    'type' => 'decimal',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );

  $token_reload = &$info['token_reload']['properties'];

  $token_reload['line_item_id'] = array(
    'label' => t('Line item ID'),
    'type' => 'commerce_line_item',
    'description' => t('The line item this token reload belongs to.'),
    'schema field' => 'line_item_id',
  );

  $token_reload['status']['options list'] = 'token_reload_statuses';

  $token_reload['meter_number']['label'] = t('Meter number');
  $token_reload['customer_number']['label'] = t('Customer number');
  $token_reload['customer_name']['label'] = t('Customer name');

  $token_reload['time']['type'] = 'date';
}
