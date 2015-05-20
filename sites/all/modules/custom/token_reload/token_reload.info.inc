<?php

/**
 * @file
 * token_reload.info.inc
 */

/**
 * Implements hook_entity_property_info_alter().
 */
function token_reload_entity_property_info_alter(&$info) {
  $properties = &$info['commerce_line_item']['bundles']['electricity_prepaid']['properties'];

  $properties['ref_2'] = array(
    'label' => t('Ref 2'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $properties['ref_3'] = array(
    'label' => t('Ref 3'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $properties['meter_number'] = array(
    'label' => t('Meter Number'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $properties['customer_number'] = array(
    'label' => t('Customer Number'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $properties['customer_name'] = array(
    'label' => t('Customer Name'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $properties['subscriber_segmentation'] = array(
    'label' => t('Subscriber Segmentation'),
    'type' => 'text',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $properties['power_consuming_category'] = array(
    'label' => t('Power Consuming Category'),
    'type' => 'integer',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
  $properties['purchased_kwh_unit'] = array(
    'label' => t('Purchased KWh Unit'),
    'type' => 'decimal',
    'getter callback' => 'token_reload_line_item_inquiry_data',
  );
}
