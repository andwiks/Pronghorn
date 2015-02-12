<?php
/**
 * @file
 * commerce_veritrans.pages.inc
 *
 * @author andre4s_y
 */

/**
 * Callback Functions.
 */

/**
 * Function commerce_veritrans_user_token().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $account
 *   User object.
 *
 * @return array
 *   Form array.
 */
function commerce_veritrans_user_token($form, &$form_state, $account) {
  drupal_set_title(format_username($account));

  return $form;
}