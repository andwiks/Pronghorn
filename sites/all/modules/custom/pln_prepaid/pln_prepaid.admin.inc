<?php
/**
 * @file
 * pln_prepaid.admin.inc
 *
 * @author aizat@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function pln_prepaid_settings_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   System settings form.
 */
function pln_prepaid_settings_form($form = array(), $form_state = array()) {
  // Sepulsa Referral Campaign fieldset.
  $form['pln_prepaid'] = array(
    '#type' => 'fieldset',
    '#title' => t('PLN Prepaid Admin Fee'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Sepulsa Referral Campaign timeout.
  $form['pln_prepaid']['pln_admin_fee'] = array(
    '#type' => 'textfield',
    '#title' => t('PLN Admin Fee (IDR)'),
    '#default_value' => variable_get('pln_admin_fee', 0),
    '#description' => t('PLN Admin Fee'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
