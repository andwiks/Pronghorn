<?php
/**
 * @file
 * Author aizat@sepulsa.com.
 */

/**
 * Function deposit_sepulsa_wizard_form().
 */
function deposit_sepulsa_wizard_form($form, &$form_state) {
  if (empty($form_state['step'])) {
    $form_state['step'] = 1;
    $form_state['step_information'] = _form_deposit_sepulsa_steps();
  }
  $step = &$form_state['step'];
  $form = $form_state['step_information'][$step]['form']($form, $form_state);
  return $form;
}

/**
 * Function _form_deposit_sepulsa_steps().
 */
function _form_deposit_sepulsa_steps() {
  return array(
    1 => array(
      'form' => 'deposit_sepulsa_get_order_id_form',
    ),
    2 => array(
      'form' => 'deposit_sepulsa_confirm_process_form',
    ),
  );
}

/**
 * Function deposit_sepulsa_get_order_id_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   System form for get order ID.
 */
function deposit_sepulsa_get_order_id_form($form = array(), $form_state = array()) {
  $form['deposit_sepulsa'] = array(
    '#type' => 'fieldset',
    '#title' => t('Failed Order ID'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['deposit_sepulsa']['order_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Order ID :'),
    '#default_value' => '',
    '#description' => t('Fail Deposit Sepulsa Order from IOS.'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proses'),
    '#submit' => array('deposit_sepulsa_get_order_id_submit'),
  );
  return $form;
}

/**
 * Function deposit_sepulsa_get_order_id_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function deposit_sepulsa_get_order_id_submit($form = array(), &$form_state = array()) {
  // Store value form.
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];

  $order_id = intval($form_state['values']['order_id']);
  $info = FALSE;
  $status = '';
  try {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order_id);
    $status = $order_wrapper->status->value();
    $order_line_items = $order_wrapper->commerce_line_items->value();
  }
  catch (EntityMetadataWrapperException $exc) {
    $info = 2;
  }
  if ($status == 'processing') {
    $info = 1;
    foreach ($order_line_items as $line_item) {
      if (($line_item->type == 'product' || $line_item->type == 'deposit_sepulsa') && strpos($line_item->line_item_label, 'DEPO') !== FALSE) {
        $form_state['step_information'][$current_step]['order_id'] = $order_id;
        $form_state['step_information'][$current_step]['line_item'] = $line_item;
        $form_state['step'] = 2;
      }
    }
  }

  if ($form_state['step'] != 2) {
    switch ($info) {
      case 1:
          drupal_set_message(t('There is no product or deposit sepulsa in type of line item'), 'error');
        break;

      case 2:
          drupal_set_message(t('Invalid Order ID'), 'error');
        break;

      default:
          drupal_set_message(t('This facility is only for orders with processing status'), 'error');
        break;
    }
  }

  if ($form_state['step'] == 2) {
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Function deposit_sepulsa_confirm_process_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   System form confirm process.
 */
function deposit_sepulsa_confirm_process_form($form = array(), &$form_state = array()) {
  $msg = '<div style = "font-size:15px;"> ';
  $msg .= '<b>' . t('Line Item To Process for Order !oid', array(
    '!oid' => l($form_state['step_information'][1]['order_id'], 'admin/commerce/orders/' . $form_state['step_information'][1]['order_id'], array(
      'attributes' => array('target'=>'_blank'),
    )),
  )) . '</b><br>
            ' . t('Line Item :') . $form_state['step_information'][1]['line_item']->line_item_label . '<br>
            ' . t('Type :') . $form_state['step_information'][1]['line_item']->type . '<br>
            ' . t('Amount :') . $form_state['step_information'][1]['line_item']->commerce_unit_price[LANGUAGE_NONE][0]['currency_code'] . ' ' . $form_state['step_information'][1]['line_item']->commerce_unit_price[LANGUAGE_NONE][0]['amount'] . '<br><br>
          <span style="color:red;">' .
          t('If you agree to this confirmation, This order will be processed.') . '</span>';
  $msg .= ' </div>';
  $form = confirm_form($form,
                      t('Are you sure want to process this order ?'),
                      'admin/commerce/tools/deposit/process',
                      $msg);
  $form['actions']['submit']['#submit'] = array('deposit_sepulsa_confirm_process_submit');
  return $form;
}

/**
 * Function deposit_sepulsa_confirm_process_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function deposit_sepulsa_confirm_process_submit($form = array(), &$form_state = array()) {
  // Set correct line item: deposit_sepulsa.
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $form_state['step_information'][1]['line_item']);
  $line_item_wrapper->type->set('deposit_sepulsa');
  $line_item_wrapper->save();
  // Load order data.
  $order = commerce_order_load($form_state['step_information'][1]['stored_values']['order_id']);
  // Load user data.
  $user = user_load($order->uid);
  // Queue topup deposit.
  topup_credit_deposit_action($order, $user, 28);
  // Show message to admin.
  drupal_set_message('Deposit order: currently in queue, please wait for process.');
  // Force queue topup deposit to run.
  if (module_exists('ultimate_cron') === TRUE) {
    $job = _ultimate_cron_job_load('queue_topup_credit_deposit');
    if (!empty($job)) {
      $job->launch();
    }
  }
}
