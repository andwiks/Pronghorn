<?php
/**
 * @file
 * views_bank_transfer.info.inc
 *
 * @author andreas@sepulsa.com
 *
 * @desc Place to alter commerce payment entity property info.
 */

/**
 * Implements hook_entity_property_info_alter().
 */
function views_bank_transfer_entity_property_info_alter(&$info) {
  // Get commerce_payment_transaction properties.
  $properties = &$info['commerce_payment_transaction']['properties'];
  // Add property for instance ID.
  $properties['instance_id'] = array(
    'label' => t('Payment instance'),
    'description' => t('The payment instance of the transaction.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer payment transactions',
    'options list' => 'views_bank_transfer_options_list',
    'required' => TRUE,
    'schema field' => 'instance_id',
  );
}
