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
 * Function commerce_veritrans_user_token_form().
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
function commerce_veritrans_user_token_form($form, &$form_state, $account) {
  // Set page title.
  drupal_set_title(t('List of saved credit card'));

  // Get all token.
  $tokens = commerce_veritrans_token_get($account->uid);
  // Define token options.
  $options = array();

  // Check state: deleted token.
  if (!(isset($form_state['deleted']) && !empty($form_state['deleted']))) {
    // Define deleted form state.
    $form_state['deleted'] = array();

    // Show notification.
    $form['notification'] = array(
      '#markup' => t('You can add saved credit card when you pay using credit card and choose to save it.'),
    );
    // Check whether token is available.
    if (!empty($tokens)) {
      foreach ($tokens as $token) {
        $options[$token->token_id] = array(
          'cc' => commerce_veritrans_token_format($token),
          'phone' => $token->phone,
          'expired' => format_date($token->expired),
          'order' => l(t('Order @oid', array('@oid' => $token->order_id)), 'user/' . $account->uid . '/orders/' . $token->order_id),
        );
      }
    }

    // Define table.
    $header = array(
      'cc' => t('Masked Card Number'),
      'phone' => t('Phone Number'),
      'expired' => t('Expired'),
      'order' => t('Use first in order'),
    );

    // Show token table.
    $form['tokens'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#attributes' => array(
        'class' => array('table', 'style1'),
      ),
      '#empty' => t('You do not have saved credit card yet.'),
    );

    // Delete button.
    if (!empty($options)) {
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
      );
    }
  }
  else {
    // Show confirmation message.
    $form['confirmation'] = array(
      '#markup' => t('Are you sure want to delete the following credit card information ?'),
    );
    $lists = array();
    foreach ($tokens as $token) {
      if (in_array($token->token_id, $form_state['deleted'])) {
        $lists[] = commerce_veritrans_token_format($token, TRUE);
      }
    }
    $form['list'] = array(
      '#markup' => '<ul><li>' . implode('</li><li>', $lists) . '</li></ul>',
    );
    $form['confirm'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
    );
    $form['or'] = array(
      '#markup' => ' ' . t('or') . ' ',
    );
    $form['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    );
  }
  // Return array form.
  return $form;
}

/**
 * Function commerce_veritrans_user_token_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function commerce_veritrans_user_token_form_validate($form, &$form_state) {
  // Check form state deleted: only for empty state.
  if (empty($form_state['deleted'])) {
    // Define deleted token.
    $deleted_token = $form_state['values']['tokens'];
    $deleted_token = array_filter($deleted_token);

    // Check whether user select empty token.
    if (empty($deleted_token)) {
      form_set_error('tokens', t('You need at least select one before continue.'));
    }
  }
}

/**
 * Function commerce_veritrans_user_token_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function commerce_veritrans_user_token_form_submit($form, &$form_state) {
  // Check form state: deleted.
  if (empty($form_state['deleted'])) {
    // Get deleted token.
    $form_state['deleted'] = array_filter($form_state['values']['tokens']);
    $form_state['deleted'] = array_keys($form_state['deleted']);
    // Rebuild form.
    $form_state['rebuild'] = TRUE;
  }
  else {
    // Get form operation.
    if ($form_state['values']['op'] == $form['cancel']['#value']) {
      drupal_set_message(t('Canceling last delete request.'));
    }
    else {
      $deleted_counter = 0;
      foreach ($form_state['deleted'] as $key) {
        $deleted = db_delete('commerce_veritrans_token')
          ->condition('token_id', $key)
          ->execute();
        $deleted_counter += $deleted;
      }
      // Show notification to user.
      drupal_set_message(t('Success deleting @counter credit card information.', array(
        '@counter' => $deleted_counter,
      )));
    }
  }
}

function commerce_veritrans_notification_callback() {
  if ($_SERVER['REQUEST_METHOD'] != 'POST') {
    drupal_add_http_header('status', '405 Method Not Allowed');
    return;
  }

  $headers = getallheaders();

  if ($headers['User-Agent'] != 'Veritrans') {
    drupal_add_http_header('status', '417 Expectation Failed');
    return;
  }

  $raw_body = empty($_POST) ? file_get_contents('php://input') : $_POST;

  if (!empty($raw_body)) {
    $body = drupal_json_decode($raw_body);
    extract($body);

    if (!empty($status_code) && $status_code == 200 && !empty($transaction_status) && $transaction_status != 'cancel' && !empty($order_id)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'commerce_payment_transaction');
      $query->propertyCondition('remote_id', $order_id);
      $query->propertyCondition('status', COMMERCE_PAYMENT_STATUS_PENDING);
      $query->propertyOrderBy('revision_id', 'DESC');
      $query->range(0, 1);

      $result = $query->execute();

      if (!empty($result)) {
        $transaction = commerce_payment_transaction_load(key($result['commerce_payment_transaction']));
        drupal_json_output($balance);
        if (!empty($transaction_id) && !empty($transaction->data['transaction_id']) && $transaction_id == $transaction->data['transaction_id']) {
          $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
          commerce_payment_transaction_save($transaction);
          drupal_json_output($transaction);
        }
      }
    }
  }

  return;
}
