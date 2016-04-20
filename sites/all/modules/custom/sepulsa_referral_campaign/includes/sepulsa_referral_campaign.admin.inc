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

function sepulsa_referral_campaign_settings($form = array(), $form_state = array()) {
  // Device verification fieldset.
  $form['sepulsa_referral_campaign'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sepulsa Referral Campaign'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Device verification timeout.
  $form['sepulsa_referral_campaign']['test'] = array(
    '#type' => 'textfield',
    '#title' => t('test'),
    '#default_value' => '',
    '#description' => t('test'),
    '#required' => TRUE,
  );

  field_attach_form('lawmakers', $lawmakers, $form, $form_state);

  return system_settings_form($form);
}

/**
 * Implements hook_form().
 */
function sepulsa_referral_campaign_form($form, &$form_state, $referral = NULL) {
  $form = array();
  // echo "<pre>";
  // print_r(date('d-m-Y', $referral->start));die();

  if (isset($referral->src_id)) {
    $form['src_id'] = array(
      '#type' => 'value',
      '#value' => $referral->src_id
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

  $form['machine_name'] = array(
    '#title' => t('Mechine Name'),
    '#type' => 'textfield',
    '#default_value' => isset($referral->machine_name) ? $referral->machine_name : '',
    '#description' => t('Default Mechine Name : replace space with _ (underscore) in title.'),
    '#maxlength' => 254,
  );

  $form['active_time'] = array(
    '#type' => 'fieldset',
    '#title' => t('Active Time'),
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
  }

  if (isset($referral->end)) {
    $date_end = explode("-", date('d-m-Y', $referral->end));
    $default_end = array(
      'day' => $date_end[0],
      'month' => str_replace('0', '', $date_end[1]),
      'year' => $date_end[2],
    );
  }

  $form['active_time']['start'] = array(
    '#type' => 'date', 
    '#title' => t('Start Date Campaign'), 
    '#default_value' => (isset($default_start)) ? $default_start : array(), 
    '#description' => 'Date when the campaign will start.', 
    '#required' => TRUE,
  );

  $form['active_time']['end'] = array(
    '#type' => 'date',
    '#title' => t('Start End Campaign'),
    '#default_value' => (isset($default_end)) ? $default_end : array(),
    '#description' => 'End when the campaign will start.',
    '#required' => TRUE,
  );

  // Get rules compontents name and label.
  $rules = rules_get_components(TRUE, 'action');
  $form['rules_name'] = array(
    '#type' => 'select',
    '#title' => t('Rules Action'),
    '#description' => t('Rules Action to fire'),
    '#options' => $rules,
    '#default_value' => isset($referral->rules_name) ? $referral->rules_name : '',
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

  // Check start / end date.
  $start = strtotime($form_state['values']['start']['day'] . '-' .
                      $form_state['values']['start']['month'] . '-' .
                      $form_state['values']['start']['year'] . ' 00:00:00');
  $end = strtotime($form_state['values']['end']['day'] . '-' .
                      $form_state['values']['end']['month'] . '-' .
                      $form_state['values']['end']['year'] . ' 23:59:59');
  if ($end < $start) {
   form_set_error('end', t('End date lower than start date.'));
  }
}

/**
 * Implements hook_form_submit().
 */
function sepulsa_referral_campaign_form_submit($form, &$form_state) {
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
    $entity->machine_name = $form_state['values']['machine_name'];
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
      'machine_name' => $form_state['values']['machine_name'],
      'start' => $start,
      'end' => $end,
      'path' => $form_state['values']['path'],
      'rules_name' => $form_state['values']['rules_name'],
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
  $pager = CAMPAIGN_PER_PAGE;
  // Define table header.
  $header = array(
    array('data' => t('Ref ID'), 'field' => 'rcd.ref_id'),
    array('data' => t('Campaign Title'), 'field' => 'rcd.src_id'),
    array('data' => t('Refering User'), 'field' => 'rcd.referral_uid'),
    array('data' => t('# Referred'), 'field' => 'num_referrals'),
    array('data' => t('Operations'), 'field' => 'operations'),
  );
  // Query
  $query = db_select('referral_campaign_details', 'rcd')->extend('PagerDefault')->extend('TableSort'); 
  $query->addExpression('COUNT(*)', 'num_referrals');
  $query->innerJoin('sepulsa_referral_campaign', 'src', 'src.src_id = rcd.src_id');
  $query->innerJoin('users', 'u', 'u.uid = rcd.referral_uid');
  $query->fields('rcd', array('ref_id','src_id','referral_uid'));
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
        $row->mail,
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
 * String campaign id.
 * @param string $referral_uid
 * String Referring UID.
 */
function sepulsa_referral_campaign_details_list($campaign_id = '', $referral_uid = ''){
  $pager = CAMPAIGN_PER_PAGE;
  // Define table header.
  $header = array(
    array('data' => t('User'), 'field' => 'rcd.uid'),
    array('data' => t('Benefit'), 'field' => 'rcd.flag'),
    array('data' => t('Benefit Taken Time'), 'field' => 'rcd.flag_timestamp'),
    array('data' => t('IP'), 'field' => 'rcd.host'),
  );
  // Query
  $query = db_select('referral_campaign_details', 'rcd')->extend('PagerDefault')->extend('TableSort'); 
  $query->innerJoin('users', 'u', 'u.uid = rcd.uid');
  $query->fields('rcd', array('ref_id','src_id','referral_uid','uid','flag','flag_timestamp','host'));
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
        $row->mail,
        ($row->flag) ? 'Taken' : 'Not Yet',
        ($row->flag) ? date('d mm Y H:i:s', $row->flag_timestamp) : '-',
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