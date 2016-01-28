<?php
/**
 * @file
 * sepulsa.admin.inc
 */

/**
 * Callback Functions.
 */

/**
 * Function sepulsa_settings_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function sepulsa_settings_form($form, &$form_state) {
  $settings = variable_get('sepulsa_settings', array());
  $form['sepulsa_settings'] = array(
    '#tree' => TRUE,
  );

  $form['sepulsa_settings']['multipaid_product'] = array(
    '#title' => t('Multipaid product'),
    '#type' => 'checkbox',
    '#default_value' => !empty($settings['multipaid_product']),
  );

  // Add submit function.
  $form['#submit'][] = 'sepulsa_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Function sepulsa_settings_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function sepulsa_settings_form_submit($form, &$form_state) {
  // Check for setting multipaid product.
  if (isset($form_state['values']['sepulsa_settings']['multipaid_product'])) {
    // Get block information: views - commerce cart block popup.
    $block = block_load('views', 'commerce_cart_block-popup');
    // Check whether setting is disabled.
    if (isset($block->status)
      && $block->status != $form_state['values']['sepulsa_settings']['multipaid_product']
      && isset($block->theme)
    ) {
      // Update the block status.
      db_update('block')
        ->fields(array(
          'status' => $form_state['values']['sepulsa_settings']['multipaid_product'],
          'region' => ($form_state['values']['sepulsa_settings']['multipaid_product']) ? 'shopping_cart' : '-1',
        ))
        ->condition('module', 'views')
        ->condition('delta', 'commerce_cart_block-popup')
        ->condition('theme', $block->theme)
        ->execute();
      // Clear cache.
      cache_clear_all();
    }
  }
}
