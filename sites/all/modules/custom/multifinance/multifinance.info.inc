<?php

/**
 * @file
 */

/**
 * hook_entity_property_info_alter().
 */
function multifinance_entity_property_info_alter(&$info) {
  $info['commerce_line_item']['bundles']['multifinance']['properties']['inquiry'] = array(
    'label' => t('Inquiry'),
    'type' => 'struct',
    'getter callback' => 'multifinance_inquiry_property_verbatim_get',
    'property info' => array(
      'admin_fee' => array(
        'label' => t('Admin Fee'),
        'type' => 'decimal',
      ),
      'misc_fee' => array(
        'label' => t('Miscellaneous Fee'),
        'type' => 'decimal',
      ),
    ),
  );
}
