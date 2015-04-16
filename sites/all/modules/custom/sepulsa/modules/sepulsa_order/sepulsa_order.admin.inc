<?php

function sepulsa_order_query_order_form($form, &$form_state) {
  $form['start'] = array(
    '#type' => 'date_popup',
    '#title' => t('Start time'),
    '#date_format' => 'm/d/Y',
    '#date_label_position' => 'invisible',
  );

  $form['end'] = array(
    '#type' => 'date_popup',
    '#title' => t('End time'),
    '#date_format' => 'm/d/Y',
    '#date_label_position' => 'invisible',
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Query'),
  );

  return $form;
}

function sepulsa_order_query_order_form_submit($form, &$form_state) {
  $arguments = array(
    'start' => $form_state['values']['start'],
    'end' => $form_state['values']['end'],
  );
  $queried = sepulsa_order_query_order(NULL, $arguments);

  $message = format_plural($queried,
      '1 order queried and will be processed on cron run.',
      '@count orders queried and will be processed on cron run.');
  drupal_set_message($message);
}
