<?php

/**
 * @file
 */

function sepulsa_settings_form($form, &$form_state) {
  $settings = variable_get('sepulsa_settings', array());
  $form['sepulsa_settings'] = array(
    '#tree' => TRUE,
  );

  $form['sepulsa_settings']['multipaid_product'] = array(
    '#title' => t('Multipaid product'),
    '#type' => 'checkbox',
    '#default_value' => empty($settings['multipaid_product']) ? FALSE : TRUE,
  );

  return system_settings_form($form);
}
