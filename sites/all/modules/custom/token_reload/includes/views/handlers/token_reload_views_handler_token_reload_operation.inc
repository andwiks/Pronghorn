<?php
/**
 * @file
 * topup_handler_field_line_item_operation.inc
 */

class token_reload_views_handler_token_reload_operation extends views_handler_field {

  function render($values) {
    $trid = $this->view->result[$this->view->row_index]->trid;
    return drupal_get_form('token_reload_operation_form_' . $trid, $trid);
  }
}

function token_reload_operation_form($form, &$form_state, $trid) {
  $form_state['token_reload'] = entity_load_single('token_reload', $trid);

  $form['operation'] = array(
    '#type' => 'select',
    '#options' => array(
      // 0 => '-Operation-',
      // 'inquiry' => t('Inquiry'),
      'process' => t('Process'),
      // 'query' => t('Query'),
      // 'requeue_process' => t('Requeue: Process'),
      // 'requeue_query' => t('Requeue: Query'),
    ),
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
  );

  return $form;
}

function token_reload_operation_form_submit($form, &$form_state) {
  $token_reload = $form_state['token_reload'];

  switch ($form_state['values']['operation']) {
    case 'process':
      $transaction = token_reload_queue_process($token_reload->line_item_id);
      drupal_set_message(t('Processing transaction.'));
      break;

    // case 'query':
    //   $params = &drupal_static('token_reload_request_call');
    //   token_reload_queue_query(array('line_item_id' => $token_reload->line_item_id));
    //   drupal_set_message('<pre>' . print_r($params, TRUE) . '</pre>');
    //   break;
  }
}
