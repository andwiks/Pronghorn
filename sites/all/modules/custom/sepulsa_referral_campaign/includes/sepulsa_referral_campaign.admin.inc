<?php
/**
 * @file
 * sepulsa_referral_campaign.admin.inc
 *
 * @author aizat@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function sepulsa_referral_campaign_settings().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   System settings form.
 */
function sepulsa_referral_campaign_settings($form = array(), $form_state = array()) {
  // Sepulsa Referral Campaign fieldset.
  $form['sepulsa_referral_campaign'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sepulsa Referral Campaign'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Sepulsa Referral Campaign timeout.
  $form['sepulsa_referral_campaign']['test'] = array(
    '#type' => 'textfield',
    '#title' => t('test'),
    '#default_value' => '',
    '#description' => t('test'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form().
 */
function sepulsa_referral_campaign_form($form, &$form_state, $referral = NULL) {
  $form = array();
  $prefix = '';

  if (isset($referral->src_id)) {
    $form['src_id'] = array(
      '#type' => 'value',
      '#value' => $referral->src_id,
    );
  }

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => isset($referral->title) ? $referral->title : '',
    '#description' => t('Title of referral Campaign.'),
    '#required' => TRUE,
    '#maxlength' => 254,
  );

  if (strpos(isset($referral->machine_name) ? $referral->machine_name : '',
    'campaign_') === FALSE) {
    $prefix = 'campaign_';
  }

  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($referral->machine_name) ? $referral->machine_name : '',
    '#disabled' => isset($referral->machine_name),
    '#maxlength' => 32,
    '#field_prefix' => '<span dir="ltr">' . $prefix,
    '#field_suffix' => '</span>&lrm;',
    '#machine_name' => array(
      'exists' => 'sepulsa_referral_campaign_load_by_name',
      'source' => array('title'),
    ),
    '#description' => t('A unique machine-readable name for the client configuration. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['active_time'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure Time of Campaign'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if (isset($referral->start)) {
    $date_start = explode("-", date('d-m-Y', $referral->start));
    $default_start = array(
      'day' => $date_start[0],
      'month' => str_replace('0', '', $date_start[1]),
      'year' => $date_start[2],
    );
    $time_start = date('H:i:s', $referral->start);
  }

  if (isset($referral->end)) {
    $date_end = explode("-", date('d-m-Y', $referral->end));
    $default_end = array(
      'day' => $date_end[0],
      'month' => str_replace('0', '', $date_end[1]),
      'year' => $date_end[2],
    );
    $time_end = date('H:i:s', $referral->end);
  }

  $form['active_time']['time_start'] = array(
    '#type' => 'fieldset',
    '#title' => t('Active Time'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['active_time']['time_start']['start'] = array(
    '#type' => 'date',
    '#title' => t('Start Date Campaign'),
    '#default_value' => (isset($default_start)) ? $default_start : array(),
    '#description' => 'Date when the campaign will start.',
    '#required' => TRUE,
  );

  $form['active_time']['time_start']['start_time'] = array(
    '#title' => t('Start Time Campaign'),
    '#type' => 'textfield',
    '#default_value' => isset($time_start) ? $time_start : '',
    '#description' => t('Time when the campaign will start. format : H:i:s, e.g. : 15:03:10'),
    '#required' => TRUE,
    '#maxlength' => 8,
  );

  $form['active_time']['time_end'] = array(
    '#type' => 'fieldset',
    '#title' => t('Deactive Time'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['active_time']['time_end']['end'] = array(
    '#type' => 'date',
    '#title' => t('Start End Campaign'),
    '#default_value' => (isset($default_end)) ? $default_end : array(),
    '#description' => 'Date when the campaign will End.',
    '#required' => TRUE,
  );

  $form['active_time']['time_end']['end_time'] = array(
    '#title' => t('End Time Campaign'),
    '#type' => 'textfield',
    '#default_value' => isset($time_end) ? $time_end : '',
    '#description' => t('Time when the campaign will start. format : H:i:s, e.g. : 08:59:60'),
    '#required' => TRUE,
    '#maxlength' => 8,
  );

  $active = array(0 => t('No'), 1 => t('Yes'));
  // Sepulsa Referral Campaign timeout.
  $form['flag_first_order'] = array(
    '#type' => 'radios',
    '#title' => t('Referrer User get benefit in first order ?'),
    '#default_value' => (isset($referral->flag_first_order)) ? $referral->flag_first_order : 0,
    '#options' => $active,
    '#description' => t('When it is no, benefit will send every transaction.'),
  );

  // Get rules compontents name and label.
  $rules = rules_get_components(TRUE, 'action');
  $form['rules_name'] = array(
    '#type' => 'select',
    '#title' => t('Rules Action For Rererred User'),
    '#description' => t('Rules Action to fire'),
    '#options' => $rules,
    '#default_value' => isset($referral->rules_name) ? $referral->rules_name : '',
  );

  $rules = rules_get_components(TRUE, 'action');
  $form['rules_name_referral'] = array(
    '#type' => 'select',
    '#title' => t('Rules Action For Referrer User'),
    '#description' => t('Rules Action to fire for the referer user'),
    '#options' => $rules,
    '#default_value' => isset($referral->rules_name_referral) ? $referral->rules_name_referral : '',
  );

  $form['path'] = array(
    '#title' => t('Path Static Page'),
    '#type' => 'textfield',
    '#default_value' => isset($referral->path) ? $referral->path : '',
    '#description' => t('Define path static page for campaign. e.g. : referral_campaign/mobile'),
    '#required' => TRUE,
    '#maxlength' => 254,
  );

  field_attach_form('sepulsa_referral_campaign', $referral, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => isset($referral->src_id) ? t('Update Referral Campaign') : t('Save Referral Campaign'),
    ),
  );

  return $form;
}

/**
 * Implements hook_form_validate().
 */
function sepulsa_referral_campaign_form_validate($form, &$form_state) {
  // Check path.
  if (!drupal_valid_path($form_state['values']['path'])) {
    form_set_error('path', t('Path doesnt exits.'));
  }
  $continue = TRUE;
  $start_time = explode(':', $form_state['values']['start_time']);
  if (count($start_time) == 3) {
    if (in_range((int) $start_time[0], 0, 24, TRUE) &&
      in_range((int) $start_time[1], 0, 59, TRUE) &&
      in_range((int) $start_time[2], 0, 59, TRUE)) {
      $continue = TRUE;
    }
    else {
      form_set_error('start_time', t('Invalid time format in Start Time Campaign'));
      $continue = FALSE;
    }
  }
  else {
    form_set_error('start_time', t('Invalid time format in Start Time Campaign'));
    $continue = FALSE;
  }
  $end_time = explode(':', $form_state['values']['end_time']);
  if (count($end_time) == 3) {
    if (in_range((int) $end_time[0], 0, 24, TRUE) &&
      in_range((int) $end_time[1], 0, 59, TRUE) &&
      in_range((int) $end_time[2], 0, 59, TRUE)) {
      $continue = TRUE;
    }
    else {
      form_set_error('end_time', t('Invalid time format in End Time Campaign'));
      $continue = FALSE;
    }
  }
  else {
    form_set_error('end_time', t('Invalid time format in End Time Campaign'));
    $continue = FALSE;
  }
  if ($continue) {
    // Check start / end date.
    $start = strtotime($form_state['values']['start']['day'] . '-' .
                        $form_state['values']['start']['month'] . '-' .
                        $form_state['values']['start']['year'] . ' ' .
                        $start_time[0] . ':' . $start_time[1] . ':' .
                        $start_time[2]);
    $end = strtotime($form_state['values']['end']['day'] . '-' .
                        $form_state['values']['end']['month'] . '-' .
                        $form_state['values']['end']['year'] . ' ' .
                        $end_time[0] . ':' . $end_time[1] . ':' .
                        $end_time[2]);
    if (empty($start)) {
      form_set_error('start_time', t('Invalid time format in Start Time Campaign'));
    }
    elseif (empty($end)) {
      form_set_error('end_time', t('Invalid time format in End Time Campaign'));
    }
    elseif ($end < $start) {
      form_set_error('end', t('End date lower than start date.'));
    }
  }
}

/**
 * Function in_range().
 *
 * For check time.
 */
function in_range($number, $min, $max, $inclusive = FALSE) {
  if (is_int($number) && is_int($min) && is_int($max)) {
    return $inclusive ? ($number >= $min && $number <= $max)
        : ($number > $min && $number < $max);
  }
  return FALSE;
}

/**
 * Implements hook_form_submit().
 */
function sepulsa_referral_campaign_form_submit($form, &$form_state) {
  $machine_name = $form_state['values']['machine_name'];
  if (strpos($form_state['values']['machine_name'], 'campaign_') === FALSE) {
    $machine_name = 'campaign_' . $form_state['values']['machine_name'];
  }

  if (isset($form_state['values']['src_id'])) {
    $start = strtotime($form_state['values']['start']['day'] . '-' .
                        $form_state['values']['start']['month'] . '-' .
                        $form_state['values']['start']['year'] . ' 00:00:00');
    $end = strtotime($form_state['values']['end']['day'] . '-' .
                        $form_state['values']['end']['month'] . '-' .
                        $form_state['values']['end']['year'] . ' 23:59:59');

    // Update sepulsa referral capaign: update entity.
    $entity = sepulsa_referral_campaign_load($form_state['values']['src_id']);
    $entity->title = $form_state['values']['title'];
    $entity->machine_name = $machine_name;
    $entity->start = $start;
    $entity->end = $end;
    $entity->path = $form_state['values']['path'];
    $entity->rules_name = $form_state['values']['rules_name'];
    $entity->changed = strtotime('now');
    $entity->save();
    drupal_set_message(t('Referral Campaign ID : @id has been saved.',
      array('@id' => $form_state['values']['src_id'])));
  }
  else {
    $start = strtotime($form_state['values']['start']['day'] . '-' .
                        $form_state['values']['start']['month'] . '-' .
                        $form_state['values']['start']['year'] . ' 00:00:00');
    $end = strtotime($form_state['values']['end']['day'] . '-' .
                        $form_state['values']['end']['month'] . '-' .
                        $form_state['values']['end']['year'] . ' 23:59:59');
    // Sepulsa referral capaign is new: create new entity.
    $entity = entity_get_controller('sepulsa_referral_campaign')->create(array(
      'title' => $form_state['values']['title'],
      'machine_name' => $machine_name,
      'start' => $start,
      'end' => $end,
      'path' => $form_state['values']['path'],
      'flag_first_order' => $form_state['values']['flag_first_order'],
      'rules_name' => $form_state['values']['rules_name'],
      'rules_name_referral' => $form_state['values']['rules_name_referral'],
      'created' => strtotime('now'),
      'changed' => strtotime('now'),
    ));
    entity_save('sepulsa_referral_campaign', $entity);
    drupal_set_message(t('New Referral Campaign has been saved.'));
    $form_state['redirect'] = 'admin/content/referral_campaign';
  }
}

/**
 * Function sepulsa_referral_campaign_summary_list().
 */
function sepulsa_referral_campaign_summary_list() {
  $pager = SEPULSA_REFERRAL_CAMPAIGN_PER_PAGE;
  // Define table header.
  $header = array(
    array('data' => t('Ref ID'), 'field' => 'rcd.ref_id'),
    array('data' => t('Campaign Title'), 'field' => 'rcd.src_id'),
    array('data' => t('Refering User'), 'field' => 'rcd.referral_uid'),
    array('data' => t('# Referred'), 'field' => 'num_referrals'),
    array('data' => t('Operations'), 'field' => 'operations'),
  );
  // Query.
  $query = db_select('referral_campaign_details', 'rcd')->extend('PagerDefault')->extend('TableSort');
  $query->addExpression('COUNT(*)', 'num_referrals');
  $query->innerJoin('sepulsa_referral_campaign', 'src', 'src.src_id = rcd.src_id');
  $query->innerJoin('users', 'u', 'u.uid = rcd.referral_uid');
  $query->fields('rcd', array('ref_id', 'src_id', 'referral_uid'));
  $query->fields('src', array('title'));
  $query->fields('u', array('mail'));
  $query->condition('u.status', 1);
  $query->groupBy('rcd.src_id');
  $query->groupBy('rcd.referral_uid');
  $result = $query->limit($pager)->orderByHeader($header)->execute()->fetchAll();
  // Define table rows.
  $rows = array();
  foreach ($result as $row) {
    // Define operation link.
    $links = array(
      array(
        'title' => t('Details'),
        'href' => 'admin/reports/sepulsa_referral_campaign/details/' . $row->src_id . '/' . $row->referral_uid,
      ),
    );
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
    // Append row to the rows container.
    $rows[] = array(
      'data' => array(
        $row->ref_id,
        $row->title,
        l($row->mail, "user/$row->referral_uid"),
        $row->num_referrals,
        $operations,
      ),
    );
  }
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
    '#empty' => t('There are no details.'),
  );

  // Attach the pager theme.
  $build['pager_pager'] = array('#theme' => 'pager');

  // Return build variables.
  return $build;
}

/**
 * Function sepulsa_referral_campaign_details_list().
 *
 * @param string $campaign_id
 *   String campaign id.
 * @param string $referral_uid
 *   String Referring UID.
 */
function sepulsa_referral_campaign_details_list($campaign_id = '', $referral_uid = '') {
  $pager = SEPULSA_REFERRAL_CAMPAIGN_PER_PAGE;
  // Define table header.
  $header = array(
    array('data' => t('User'), 'field' => 'rcd.uid'),
    array('data' => t('Benefit'), 'field' => 'rcd.flag'),
    array('data' => t('Benefit Taken Time'), 'field' => 'rcd.flag_timestamp'),
    array('data' => t('IP'), 'field' => 'rcd.host'),
  );
  // Query.
  $query = db_select('referral_campaign_details', 'rcd')->extend('PagerDefault')->extend('TableSort');
  $query->innerJoin('users', 'u', 'u.uid = rcd.uid');
  $query->fields('rcd', array('ref_id',
    'src_id',
    'referral_uid',
    'uid',
    'flag',
    'flag_timestamp',
    'host',
  ));
  $query->fields('u', array('mail'));
  $query->condition('rcd.src_id', $campaign_id);
  $query->condition('rcd.referral_uid', $referral_uid);
  $result = $query->limit($pager)->orderByHeader($header)->execute()->fetchAll();
  // Define table rows.
  $rows = array();
  foreach ($result as $row) {
    // Append row to the rows container.
    $rows[] = array(
      'data' => array(
        l($row->mail, "user/$row->uid"),
        ($row->flag) ? 'Taken' : 'Not Yet',
        ($row->flag) ? date('d mm Y H:i:s', $row->flag_timestamp) : '',
        $row->host,
      ),
    );
  }
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
    '#empty' => t('There are no details.'),
  );

  // Attach the pager theme.
  $build['pager_pager'] = array('#theme' => 'pager');

  // Return build variables.
  return $build;
}
