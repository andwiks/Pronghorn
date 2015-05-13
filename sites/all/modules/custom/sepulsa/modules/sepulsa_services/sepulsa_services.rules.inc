<?php

/**
 * @file
 * sepulsa_services.rules.inc
 */

/**
 * Implements hook_rules_condition_info.
 */
function sepulsa_services_rules_condition_info() {
  return array(
    'drupal_is_services' => array(
      'label' => t('Drupal is services endpoint'),
      'group' => t('Services'),
      'callbacks' => array(
        'execute' => 'sepulsa_services_drupal_is_services',
      ),
    ),
  );
}

function sepulsa_services_drupal_is_services() {
  $menu_item = menu_get_item();
  return !empty($menu_item['page_callback']) && $menu_item['page_callback'] == 'services_endpoint_callback';
}
