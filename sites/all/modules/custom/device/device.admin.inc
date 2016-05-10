<?php
/**
 * @file
 * device.admin.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function device_settings().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   System settings form.
 */
function device_settings($form = array(), $form_state = array()) {
  // Device verification fieldset.
  $form['verification'] = array(
    '#type' => 'fieldset',
    '#title' => t('Device Verification'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Device verification timeout.
  $form['verification']['device_verification_timeout'] = array(
    '#type' => 'select',
    '#title' => t('Verification Timeout'),
    '#options' => array(
      '+5 minutes' => t('5 Minutes'),
      '+15 minutes' => t('15 Minutes'),
      '+30 minutes' => t('30 Minutes'),
      '+1 hour' => t('1 Hour'),
      '+2 hours' => t('2 Hours'),
      '+3 hours' => t('3 Hours'),
      '+6 hours' => t('6 Hours'),
      '+12 hours' => t('12 Hours'),
      '+1 day' => t('1 Day'),
    ),
    '#default_value' => variable_get('device_verification_timeout', '+30 minutes'),
    '#description' => t('Set default device verification timeout.'),
    '#required' => TRUE,
  );
  // Device Verification hash fieldset.
  $form['verification']['device_verification_hash'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hash / Code'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#description' => t('Message pattern that will be send to customer for device verification.'),
  );
  // Get device verification hash config.
  $default_value = variable_get('device_verification_hash', array(
    'before' => '',
    'prefix' => '',
    'pattern' => 1,
    'length' => 6,
    'suffix' => '',
    'after' => '',
  ));
  // Device verification hash message before.
  $form['verification']['device_verification_hash']['before'] = array(
    '#type' => 'textfield',
    '#title' => t('Message before'),
    '#default_value' => $default_value['before'],
    '#size' => 40,
    '#maxlength' => 70,
    '#description' => t('If filled, system will add a space after this message.'),
  );
  // Device verification hash prefix.
  $form['verification']['device_verification_hash']['prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#default_value' => $default_value['prefix'],
    '#size' => 4,
    '#maxlength' => 4,
  );
  // Device verification hash pattern.
  $form['verification']['device_verification_hash']['pattern'] = array(
    '#type' => 'select',
    '#title' => t('Pattern'),
    '#options' => array(
      1 => t('Digit only'),
      2 => t('Alphanumeric lowercase.'),
      3 => t('Alphanumeric uppercase.'),
      4 => t('Alphanumeric randomcase.'),
    ),
    '#required' => TRUE,
    '#default_value' => $default_value['pattern'],
    '#description' => t('Set default device verification pattern.'),
  );
  // Device verification hash length.
  $form['verification']['device_verification_hash']['length'] = array(
    '#type' => 'select',
    '#title' => t('Length'),
    '#options' => array(
      6 => t('6 Chars'),
      7 => t('7 Chars'),
      8 => t('8 Chars'),
    ),
    '#required' => TRUE,
    '#default_value' => $default_value['length'],
    '#description' => t('Set default device verification pattern.'),
  );
  // Device verification hash suffix.
  $form['verification']['device_verification_hash']['suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Suffix'),
    '#default_value' => $default_value['suffix'],
    '#size' => 4,
    '#maxlength' => 4,
  );
  // Device verification hash message after.
  $form['verification']['device_verification_hash']['after'] = array(
    '#type' => 'textfield',
    '#title' => t('Message after'),
    '#default_value' => $default_value['after'],
    '#size' => 40,
    '#maxlength' => 70,
    '#description' => t('If filled, system will add a space before this message.'),
  );
  // Device verification hash result example.
  $form['verification']['device_verification_hash']['result'] = array(
    '#type' => 'item',
    '#title' => t('Result Example'),
    '#markup' => device_generate_hash_message(device_generate_hash()),
  );

  // Get device session default value.
  $default_value = variable_get('device_session_timeout', array(
    'refresh' => '+30 days',
    'access' => '+1 hour',
  ));
  // Session fieldset.
  $form['device_session_timeout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Device Session Timeout'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $form['device_session_timeout']['access'] = array(
    '#type' => 'select',
    '#title' => t('Access Session Timeout'),
    '#options' => array(
      '+30 minutes' => t('30 Minutes'),
      '+1 hour' => t('1 Hour'),
      '+6 hours' => t('6 Hours'),
      '+12 hours' => t('12 Hours'),
      '+1 day' => t('1 Day'),
      '+1 week' => t('1 Week'),
      '+2 week' => t('2 Weeks'),
      '+3 week' => t('3 Weeks'),
      '+4 week' => t('4 Weeks'),
      '+30 days' => t('30 Days'),
      '+1 month' => t('1 Month'),
    ),
    '#default_value' => $default_value['access'],
    '#description' => t('Set default timeout for device access session. Shorter than refresh session timeout.'),
    '#required' => TRUE,
  );
  $form['device_session_timeout']['refresh'] = array(
    '#type' => 'select',
    '#title' => t('Refresh Session Timeout'),
    '#options' => array(
      '0' => t('No timeout'),
      '+1 hour' => t('1 Hour'),
      '+6 hours' => t('6 Hours'),
      '+12 hours' => t('12 Hours'),
      '+1 day' => t('1 Day'),
      '+1 week' => t('1 Week'),
      '+2 week' => t('2 Weeks'),
      '+3 week' => t('3 Weeks'),
      '+4 week' => t('4 Weeks'),
      '+30 days' => t('30 Days'),
      '+1 month' => t('1 Month'),
      '+2 month' => t('2 Months'),
      '+3 month' => t('3 Months'),
      '+4 month' => t('4 Months'),
      '+5 month' => t('5 Months'),
      '+6 month' => t('6 Months'),
      '+1 year' => t('1 Year'),
    ),
    '#default_value' => $default_value['refresh'],
    '#description' => t('Set default timeout for device refresh session. Longer than access session timeout.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Function device_list().
 *
 * Admin can see all device and do something about it.
 */
function device_list() {
  // Define table header.
  $header = array(
    array('data' => t('ID'), 'field' => 'devir.did'),
    array('data' => t('IMEI'), 'field' => 'dev.imei'),
    array('data' => t('Phone'), 'field' => 'dev.phone'),
    // array('data' => t('Label'), 'field' => 'dev.label'),
    array('data' => t('Created'), 'field' => 'dev.created'),
    array('data' => t('Access'), 'field' => 'dev.access'),
    array('data' => t('Status'), 'field' => 'dev.status'),
    array('data' => t('User'), 'field' => 'dev.uid'),
    array('data' => t('User Mail')),
    array('data' => t('OTP Confirmation'), 'field' => 'devir.status'),
    array('data' => t('Mail Verification'), 'field' => 'mail_verification'),
    array('data' => t('Timeout<br/>(Hash)'), 'field' => 'devir.timeout'),
    array('data' => t('Operations')),
  );

  // Define pager.
  $pager = variable_get('device_list_pager', 20);
  // Define search parameter.
  $search = array();
  // Check session search imei.
  if (isset($_SESSION['device']['search']['imei']) && !empty($_SESSION['device']['search']['imei'])) {
    $search['imei'] = $_SESSION['device']['search']['imei'];
  }
  // Check session search phone.
  if (isset($_SESSION['device']['search']['phone']) && !empty($_SESSION['device']['search']['phone'])) {
    $search['phone'] = $_SESSION['device']['search']['phone'];
  }
  // Check session search mail.
  if (isset($_SESSION['device']['search']['mail']) && !empty($_SESSION['device']['search']['mail'])) {
    $search['mail'] = $_SESSION['device']['search']['mail'];
  }

  // Query to get total information.
  $total = db_select('device', 'dev')->fields('dev');
  if (isset($search['imei'])) {
    $total->condition('dev.imei', $search['imei']);
  }
  if (isset($search['phone'])) {
    $total->condition('dev.phone', $search['phone']);
  }
  if (isset($search['mail'])) {
    $total->join('users', 'u', 'u.uid = dev.uid');
    $total->condition('u.mail', $search['mail']);
  }
  $total = $total->countQuery()->execute()->fetchField();
  // Query to get promo data.
  $query = db_select('device', 'dev')->extend('PagerDefault')->extend('TableSort');
  $query->join('device_verification', 'devir', 'devir.did = dev.did');
  $query->leftJoin('device_mail_verification', 'dmv', 'dmv.did = dev.did');
  $query->fields('dev')->addField('devir', 'status', 'verification');
  $query->fields('devir')->addField('devir', 'did', 'devir_did');
  $query->fields('dmv')->addField('dmv', 'status', 'mail_verification');
  $query->addField('devir', 'hash');
  $query->addField('devir', 'timeout', 'timeout');
  if (isset($search['imei'])) {
    $query->condition('dev.imei', $search['imei']);
  }
  if (isset($search['phone'])) {
    $query->condition('dev.phone', $search['phone']);
  }
  if (isset($search['mail'])) {
    $query->join('users', 'u', 'u.uid = dev.uid');
    $query->condition('u.mail', $search['mail']);
  }
  $result = $query->limit($pager)->orderByHeader($header)->execute()->fetchAll();

  dpm($result);
  // Define table rows.
  $rows = array();
  foreach ($result as $row) {
    // Define operation link.
    $links = array(
      array(
        'title' => (empty($row->status)) ? t('Unblock') : t('Block'),
        'href' => 'admin/devices/' . (empty($row->status) ? 'unblock' : 'block') . '/' . $row->devir_did,
      ),
      array(
        'title' => t('Delete'),
        'href' => 'admin/devices/delete/' . $row->did,
      ),
    );

    // Check if device still need verification.
    if (!empty($row->timeout)) {
      $links[] = array(
        'title' => t('Resend OTP'),
        'href' => 'admin/devices/resend/' . $row->did,
      );
      $links[] = array(
        'title' => t('Reset OTP'),
        'href' => 'admin/devices/reset/' . $row->did,
      );
    }

    // kpr($result_dmv['mail_verification']);
    if (($row->mail_verification == 2 || $row->mail_verification == 0) && $row->verification==1) {
      $links[] = array(
        'title' => t('Resend Mail Verification'),
        'href' => 'admin/devices/resend_mail_verification/' . $row->devir_did,
      );
    }

    // Check ctools module.
    if (module_exists('ctools')) {
      // Include ctools dropdown.theme.
      ctools_include('dropbutton.theme');
      // Generate ctools dropdown.
      $operations = theme_links__ctools_dropbutton(array(
        'class' => FALSE,
        'image' => FALSE,
        'title' => FALSE,
        'attributes' => array(),
        'links' => $links,
      ));
    }
    else {
      $operations = theme_links(array(
        'links' => $links,
        'attributes' => array(),
        'heading' => array(),
      ));
    }
    // Define verification string.
    if ($row->verification == 2) {
      $verification = t('Unconfirmed');
    }
    elseif ($row->verification == 1) {
      $verification = t('Confirmed');
    }
    else {
      $verification = t('New');
    }

    // Define verification mail_verification string.
    if ($row->mail_verification == 2) {
      $mail_verification = t('Timeout');
    }
    elseif ($row->mail_verification == 1) {
      $mail_verification = t('Verified');
    }
    else {
      $mail_verification = t('Unverified');
    }

    // Load user object if necessary.
    $mail = '-';
    if ($row->uid > 0) {
      $user = user_load($row->uid);
      if (isset($user->mail)) {
        $mail = $user->mail;
      }
    }


    // Append row to the rows container.
    $rows[] = array(
      'data' => array(
        $row->devir_did, $row->imei, $row->phone, /*$row->label,*/
        format_date($row->created, 'short'),
        (!empty($row->access)) ? format_date($row->access, 'short') : '-',
        ($row->status > 0) ? t('Active') : '<strong>' . t('Blocked') . '</strong>',
        l($row->uid, 'user/' . $row->uid, array(
          'attributes' => array('target' => '_blank'),
        )), $mail,
        $verification,
        $mail_verification,
        (!empty($row->timeout) ? format_date($row->timeout, 'short') . '<br /><strong>' . $row->hash . '</strong>' : t('none')),
        $operations,
      ),
    );
  }

  // Get search form.
  $build['search'] = drupal_get_form('device_search_form');
  $build['search']['#attributes']['class'][] = 'container-inline';

  // Define start and end information.
  $start_info = (empty($total)) ? 0 : (($total > $pager) ? ((isset($_GET['page']) && !empty($_GET['page'])) ? (intval($_GET['page']) * $pager) + 1 : 1) : 1);
  $end_info = ($total > $pager) ? ((isset($_GET['page']) && !empty($_GET['page'])) ? (intval($_GET['page']) * $pager) + $pager : $pager) : $total;
  $end_info = ($end_info > $total) ? $total : $end_info;
  // Display page information.
  $build['page_info'] = array(
    '#markup' => t('Displaying @start - @end of @total (total page @page).<br />', array(
      '@start' => $start_info,
      '@end' => $end_info,
      '@total' => $total,
      '@page' => ceil($total / $pager),
    )),
  );

  // Attach the pager table.
  $build['pager_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no device registered.'),
  );

  // Attach the pager theme.
  $build['pager_pager'] = array('#theme' => 'pager');

  // Return build variables.
  return $build;
}

/**
 * Function device_search_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function device_search_form($form = array(), $form_state = array()) {
  $form['imei'] = array(
    '#type' => 'textfield',
    '#title' => t('IMEI'),
    '#default_value' => isset($_SESSION['device']['search']['imei']) ? $_SESSION['device']['search']['imei'] : '',
    '#size' => 40,
    '#maxlength' => 254,
  );
  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#default_value' => isset($_SESSION['device']['search']['phone']) ? $_SESSION['device']['search']['phone'] : '',
    '#size' => 16,
    '#maxlength' => 16,
  );
  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail'),
    '#default_value' => isset($_SESSION['device']['search']['mail']) ? $_SESSION['device']['search']['mail'] : '',
    '#size' => 40,
    '#maxlength' => 254,
  );
  $form['search'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('device_search_form_reset'),
  );
  if (isset($_SESSION['device']['search'])) {
    unset($_SESSION['device']['search']);
  }
  return $form;
}

/**
 * Function device_search_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function device_search_form_validate($form, &$form_state) {
  if (isset($form_state['values']['mail']) && !empty($form_state['values']['mail'])
    && !valid_email_address($form_state['values']['mail'])
  ) {
    form_set_error('mail', t('Invalid email address.'));
  }
  if (isset($form_state['values']['imei']) && !empty($form_state['values']['imei'])) {
    if (!(preg_match(DEVICE_IMEI_PATTERN, $form_state['values']['imei'])
      || valid_email_address($form_state['values']['imei'])
    )) {
      form_set_error('mail', t('Invalid IMEI.'));
    }
  }
  if (isset($form_state['values']['phone']) && !empty($form_state['values']['phone'])
    && !preg_match(DEVICE_PHONE_PATTERN, $form_state['values']['phone'])
  ) {
    form_set_error('mail', t('Invalid phone number.'));
  }
}

/**
 * Function device_search_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function device_search_form_submit($form, &$form_state) {
  $_SESSION['device']['search'] = array();
  if (isset($form_state['values']['mail']) && !empty($form_state['values']['mail'])) {
    $_SESSION['device']['search']['mail'] = $form_state['values']['mail'];
  }
  if (isset($form_state['values']['phone']) && !empty($form_state['values']['phone'])) {
    $_SESSION['device']['search']['phone'] = $form_state['values']['phone'];
  }
  if (isset($form_state['values']['imei']) && !empty($form_state['values']['imei'])) {
    $_SESSION['device']['search']['imei'] = $form_state['values']['imei'];
  }
}

/**
 * Function device_search_form_reset().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function device_search_form_reset($form, &$form_state) {
  if (isset($_SESSION['device']['search'])) {
    unset($_SESSION['device']['search']);
  }
}

/**
 * Function device_operate_confirm().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param string $operation
 *   Device operation from device_operation_load().
 * @param object $device
 *   Device object from device_load().
 *
 * @return array
 *   Confirm form array.
 */
function device_operate_confirm($form = array(), $form_state = array(), $operation = array(), $device = NULL) {
  // Check device operation.
  if (empty($operation)) {
    drupal_set_message(t('Undefined device operation.'));
    drupal_goto('admin/devices/list');
  }
  // Check if device is not found.
  if (empty($device)) {
    drupal_set_message(t('Can not %operation device: device not found.', array(
      '%operation' => $operation['title'],
    )));
    drupal_goto('admin/devices/list');
  }
  $form['did'] = array(
    '#type' => 'value',
    '#value' => $device->did,
  );
  $form['label'] = array(
    '#type' => 'value',
    '#value' => $device->label,
  );
  $form['imei'] = array(
    '#type' => 'value',
    '#value' => $device->imei,
  );
  $form['phone'] = array(
    '#type' => 'value',
    '#value' => $device->phone,
  );
  $form['operation'] = array(
    '#type' => 'value',
    '#value' => $operation['name'],
  );
  $form['operation_title'] = array(
    '#type' => 'value',
    '#value' => $operation['title'],
  );

  $message = t('You want to %operation device %label (@imei - @phone)', array(
    '%operation' => strtoupper($operation['title']),
    '%label' => $device->label,
    '@imei' => $device->imei,
    '@phone' => $device->phone,
  ));
  $caption = '<p>' . t('Are you sure?') . (($operation['name'] == 'delete') ? ' ' . t('This action cannot be undone.') : '') . '</p>';

  return confirm_form($form, filter_xss_admin($message), 'admin/devices/list', filter_xss_admin($caption), $operation['title']);
}

/**
 * Function device_operate_confirm_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function device_operate_confirm_submit($form, &$form_state) {
  switch ($form_state['values']['operation']) {
    case 'delete':
      device_delete($form_state['values']['did']);
      break;

    case 'block':
      device_block($form_state['values']['did']);
      break;

    case 'unblock':
      device_unblock($form_state['values']['did']);
      break;

    case 'resend':
      $dv_wrapper = entity_metadata_wrapper('device_verification', $form_state['values']['did']);
      device_send_verification_message($dv_wrapper->phone->value(), $dv_wrapper->hash->value());
      break;

    case 'reset':
      $dv_wrapper = entity_metadata_wrapper('device_verification', $form_state['values']['did']);
      // Regenerate hash: set as new.
      $hash = device_generate_hash();
      $dv_wrapper->created->set(REQUEST_TIME);
      $dv_wrapper->timeout->set(strtotime(variable_get('device_verification_timeout', '+30 minutes'), REQUEST_TIME));
      $dv_wrapper->hash->set($hash);
      $dv_wrapper->save();
      device_send_verification_message($dv_wrapper->phone->value(), $hash);
      break;

    case 'resend_mail_verification':
      device_resend_mail_verification($form_state['values']['did']);
      break;
  }
  // Notify the admin.
  $t_args = array(
    '%operation' => strtoupper($form_state['values']['operation_title']),
    '%label' => $form_state['values']['label'],
    '@imei' => $form_state['values']['imei'],
    '@phone' => $form_state['values']['phone'],
  );
  drupal_set_message(t('Operation %operation device %label (@imei - @phone) has been done.', $t_args));
  watchdog('device', 'Operation %operation device %label (@imei - @phone).', $t_args, WATCHDOG_NOTICE);
  // Redirect to list page.
  $form_state['redirect'] = 'admin/devices/list';
}
