<?php

/**
 * hook_entity_property_info_alter().
 */
function commerce_veritrans_entity_property_info_alter(&$info) {
  $properties = &$info ['commerce_order']['properties'];

  $properties ['veritrans_reponse'] = array(
    'label' => t('Veritrans Reponse'),
    'description' => t('Response from veritrans payment.'),
    'type' => 'struct',
    'getter callback' => 'commerce_veritrans_response_property_get',
    'property info' => array(
      'biller_code' => array(
        'label' => t('Biller code'),
        'description' => t('Biller code information if any.'),
        'type' => 'text',
        'getter callback' => 'entity_property_verbatim_get',
      ),
      'bill_key' => array(
        'label' => t('Bill key'),
        'description' => t('Bill key information if any.'),
        'type' => 'text',
        'getter callback' => 'entity_property_verbatim_get',
      ),
    ),
  );
}
