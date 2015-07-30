<?php

/**
 * @file
 * Provides metadata for the sepulsa order entity.
 */

/**
 * Implements hook_entity_property_info.
 */
function sepulsa_order_entity_property_info() {
  $info = array();
  $properties = &$info ['sepulsa_order']['properties'];

  $properties['order'] = array(
    'label' => t('Order'),
    'type' => 'commerce_order',
    'description' => t('Original order.'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'order_id',
  );
  $properties['mail'] = array(
    'label' => t('Order e-mail'),
    'description' => t('The e-mail address associated with this order.'),
    'setter callback' => 'entity_property_verbatim_set',
    'validation callback' => 'valid_email_address',
    'schema field' => 'mail',
  );
  $properties['status'] = array(
    'type' => 'text',
    'label' => t('Status'),
    'description' => t('The current status of the order.'),
    'setter callback' => 'entity_property_verbatim_set',
    'options list' => 'sepulsa_order_status_options_list',
    'schema field' => 'status',
  );
  $properties['created'] = array(
    'type' => 'date',
    'label' => t('Date created'),
    'description' => t('The date the order was created.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_order entities',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'type' => 'date',
    'label' => t('Date changed'),
    'description' => t('The date the order was most recently updated.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_order entities',
    'schema field' => 'changed',
  );
  $properties['prepaid'] = array(
    'label' => t('Mobile Prepaid'),
    'type' => 'commerce_product',
    'description' => t('Mobile prepaid.'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'prepaid',
  );
  $properties['phone'] = array(
    'label' => t('Phone Number'),
    'description' => t('Customer phone number.'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'phone',
  );

  for ($i=1; $i <= 5; $i++) {
    $properties ['coupon_' . $i] = array(
      'label' => t('Coupon !number', array('!number' => $i)),
      'type' => 'commerce_product',
      'description' => t('Customer selected coupon number !number.', array('!number' => $i)),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'coupon_' . $i,
    );
  }

  $properties['coupon_number'] = array(
    'label' => t('Coupon number'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'coupon_number',
  );
  $properties['amount'] = array(
    'label' => t('Amount'),
    'type' => 'decimal',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'amount',
  );
  $properties['currency_code'] = array(
    'label' => t('Currency'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'options list' => 'commerce_currency_code_options_list',
    'schema field' => 'currency_code',
  );
  $properties['payment_method'] = array(
    'label' => t('Payment Method'),
    'description' => t('The payment method of the transaction.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer payment transactions',
    'options list' => 'commerce_payment_method_options_list',
    'schema field' => 'payment_method',
  );
  $properties['payment_instance'] = array(
    'label' => t('Payment Instance'),
    'type' => 'rules_config',
    'description' => t('The payment method instance of the transaction'),
    'setter callback' => 'entity_property_verbatim_set',
    'options list' => 'sepulsa_order_payment_instance_options_list',
    'schema field' => 'payment_instance',
  );

  return $info;
}
