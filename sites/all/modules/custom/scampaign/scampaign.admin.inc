<?php
/**
 * @file
 * scampaign.admin.inc
 *
 * @author roy
 */

/**
 * Main form function to create the link.
 */
function scampaign_add_form($form, &$form_state) {
  $form = array();
  $form['campaign'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Campaign Name'),
    '#required' => TRUE,
  );

  // Get rules compontents name and label.
  $rules = rules_get_components(TRUE, 'action');
  $form['rules_name'] = array(
    '#type' => 'select',
    '#title' => t('Rules Action'),
    '#description' => t('Rules Action to fire'),
    '#options' => $rules,
  );

  $form['max_use'] = array(
    '#type' => 'textfield',
    '#title' => t('Link Count'),
    '#description' => t('How many link you want to create or Maximum use for non unique link. Max 820812'),
    '#default_value' => 1,
    '#required' => TRUE,
  );

  $form['unique'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unique'),
    '#description' => t('If checked, link will be created with 1 max use for each count.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * Form validate to create links.
 */
function scampaign_add_form_validate($form, &$form_state) {
  $result = db_query('SELECT rules_name FROM {scampaign} WHERE campaign = :campaign', array(
    ':campaign' => $form_state['values']['campaign'],
  ));

  if ($result->rowCount() > 0) {
    // Campaign name already exist.
    if ($rule->rules_name != $form_state['values']['rules_name']) {
      form_set_error('campaign', 'Campaign name already exist with different rule');
    }
  }
  if (!preg_match('#^[a-zA-Z0-9]+$#', $form_state['values']['campaign'])) {
    form_set_error('campaign', 'Campaign name must contain only alphanumeric.');
  }
  if ($form_state['values']['max_use'] > 820812) {
    form_set_error('max_use', 'Cannot exceed 820812.');
  }
}

/**
 * Form submit to create links.
 */
function scampaign_add_form_submit($form, &$form_state) {
  if ($form_state['values']['unique']) {
    // If unique is ticked, create unique link for each counter.
    for ($i = 0; $i < $form_state['values']['max_use']; $i++) {
      db_insert('scampaign')
        ->fields(array(
          'campaign' => $form_state['values']['campaign'],
          'rules_name' => $form_state['values']['rules_name'],
        ))
        ->execute();
    }
  }
  else {
    db_insert('scampaign')
      ->fields(array(
        'campaign' => $form_state['values']['campaign'],
        'rules_name' => $form_state['values']['rules_name'],
        'max_use' => $form_state['values']['max_use'],
        'email' => serialize(array()),
      ))
      ->execute();
  }
  drupal_set_message(t('Sepulsa campaign has been created.'));
  // Redirect to main page.
  $form_state['redirect'] = 'admin/people/scampaign/' . $form_state['values']['campaign'] . '/detail';
}

/**
 * Campaign Link Overview.
 */
function scampaign_main() {
  // Select all campaign.
  $result = db_query('SELECT * FROM {scampaign}');
  $arr = array();
  foreach ($result as $key => $value) {
    // Group into array with campaign name as key.
    $arr[$value->campaign]['rules_name'] = $value->rules_name;
    isset($arr[$value->campaign]['max_use']) ? $arr[$value->campaign]['max_use'] += $value->max_use : $arr[$value->campaign]['max_use'] = $value->max_use;
    isset($arr[$value->campaign]['used']) ? $arr[$value->campaign]['used'] += $value->used : $arr[$value->campaign]['used'] = $value->used;
  }

  $headers = array(
    t('Campaign'), t('Rules Component'), t('Link Count'), t('Link Used'),
    t('Operation'),
  );
  $rows = array();
  foreach ($arr as $key => $value) {
    // Define operation link.
    $links = array(
      array(
        'title' => t('Detail'),
        'href' => 'admin/people/scampaign/' . $key . '/detail',
      ),
      array(
        'title' => t('Export'),
        'href' => 'admin/people/scampaign/' . $key . '/export',
      ),
      array(
        'title' => t('Delete'),
        'href' => 'admin/people/scampaign/' . $key . '/delete',
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
    // Add table row.
    $rows[] = array(
      $key,
      $value['rules_name'],
      $value['max_use'],
      $value['used'],
      $operations,
    );
  }
  $output = theme('table', array('header' => $headers, 'rows' => $rows));
  return $output;
}

/**
 * Detail page filter form.
 */
function scampaign_detail_filter_form($form, &$form_state) {
  $form['#attributes'] = array('class' => array('search-form'));
  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic Filter'),
    '#attributes' => array('class' => array('container-inline')),
  );

  $keys = isset($_SESSION['scampaign_detail_filter']) ? $_SESSION['scampaign_detail_filter'] : array();

  if (empty($keys)) {
    // Provide default value.
    $keys['used'] = 0;
  }
  else {
    $form['filter']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#submit' => array('scampaign_detail_filter_form_submit_reset'),
      '#weight' => 100,
    );
  }

  $form['filter']['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Code'),
    '#default_value' => isset($keys['code']) ? $keys['code'] : '',
    '#size' => 10,
    '#maxlength' => 20,
  );

  $form['filter']['used'] = array(
    '#type' => 'checkbox',
    '#title' => 'Only used link',
    '#default_value' => isset($keys['used']) ? $keys['used'] : FALSE,
  );


  $form['filter']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('scampaign_detail_filter_form_submit_filter'),
  );
  return $form;
}

/**
 * Function scampaign_detail_filter_form_submit_filter().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function scampaign_detail_filter_form_submit_filter($form, &$form_state) {
  $_SESSION['scampaign_detail_filter'] = $form_state['values'];
  $form_state['redirect'] = 'admin/people/scampaign/' . $_SESSION['scampaign_detail_campaign'] . '/detail';
}

/**
 * Function scampaign_detail_filter_form_submit_reset().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function scampaign_detail_filter_form_submit_reset($form, &$form_state) {
  $_SESSION['scampaign_detail_filter'] = array();
  $form_state['redirect'] = 'admin/people/scampaign/' . $_SESSION['scampaign_detail_campaign'] . '/detail';
}

/**
 * Function scampaign_detail().
 *
 * Campaign Link Detail.
 *
 * @param string $campaign
 *   Campaign name.
 */
function scampaign_detail($campaign) {
  $_SESSION['scampaign_detail_campaign'] = $campaign;
  $header = array();
  $header[] = array('data' => t('ID'), 'field' => 'id', 'sort' => 'asc');
  $header[] = array('data' => t('Link'));
  $header[] = array('data' => t('Link Count'), 'field' => 'max_use');
  $header[] = array('data' => t('Used'), 'field' => 'used');
  $header[] = array('data' => t('Email'));

  $query = db_select('scampaign')->extend('PagerDefault')->extend('TableSort');

  $query->condition('campaign', $campaign, '=');

  $keys = isset($_SESSION['scampaign_detail_filter']) ? $_SESSION['scampaign_detail_filter'] : array();
  if ($keys) {
    if (isset($keys['used']) && !empty($keys['used'])) {
      $query->condition('used', 0, '>');
    }
  }

  $result = $query
  ->fields('scampaign')
    ->orderByHeader($header)
    ->limit(50)
    ->execute();

  $arr = array();
  // Hardcode the pin for cosistency, must be bigger than 100k
  $rand = '820812';
  foreach ($result as $key => $value) {
    $crypt = $value->id . $rand . $value->id;
    $crypt = alphaID($crypt, FALSE, 10);
    if (!isset($keys['code'])
      || (isset($keys['code']) && empty($keys['code']))
      || (isset($keys['code']) && !empty($keys['code']) && $keys['code'] == $crypt)
    ) {
      $row = array(
        'id' => $value->id,
        'link' => $crypt,
        'max_use' => $value->max_use,
        'used' => $value->used,
      );
      $emails = unserialize($value->email);
      empty($emails) ? $row['email'] = '' : $row['email'] = implode(', ', $emails);
      $arr[] = $row;
    }
  }
  $output['scampaign_detail_filter_form'] = drupal_get_form('scampaign_detail_filter_form');
  $output['scampaign_detail_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $arr,
  );

  if (!(isset($keys['code']) && !empty($keys['code']))) {
    $output['scampaign_detail_pager'] = array('#theme' => 'pager');
  }

  $output['scampaign_export'] = array(
    '#theme' => 'link',
    '#text' => 'Export ALL Link to CSV',
    '#path' => 'admin/people/scampaign/' . $campaign . '/export',
    '#options' => array(
      'attributes' => array('target' => '_blank'),
      // REQUIRED.
      'html' => FALSE,
    ),
  );
  return $output;
}

/**
 * Function scampaign_detail_export().
 *
 * Campaign Link Detail Export Callback.
 *
 * @param string $campaign
 *   Campaign name.
 */
function scampaign_detail_export($campaign) {
  $result = db_query('SELECT * FROM {scampaign} WHERE campaign = :campaign', array(':campaign' => $campaign));
  $arr = array();
  $rand = '820812';
  foreach ($result as $key => $value) {
    $crypt = $value->id . $rand . $value->id;
    $link = alphaID($crypt, FALSE, 10);
    $row = array(
      'link' => $link,
    );
    if ($value->max_use != 1) {
      $row['max_use'] = $value->max_use;
      $row['used'] = $value->used;
    }
    $emails = unserialize($value->email);
    $row['email'] = empty($emails) ? '' : implode(', ', $emails);
    $arr[] = $row;
  }

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=campaign-' . $value->rules_name . '.csv');

  $output = '';
  foreach ($arr as $link) {
    $output .= implode(',', $link) . "\n";
  }
  ob_clean();
  print $output;
  exit;
}

/**
 * Function scampaign_delete_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param string $campaign
 *   Campaign name.
 *
 * @return array
 *   Confirm form array.
 */
function scampaign_delete_form($form = array(), $form_state = array(), $campaign = NULL) {
  $form['campaign'] = array(
    '#type' => 'value',
    '#value' => $campaign,
  );

  $message = t('Are you sure you want to delete the sepulsa campagin configuration %label?', array('%label' => $campaign));
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, filter_xss_admin($message), 'admin/people/scampaign/main', filter_xss_admin($caption), t('Delete'));
}

/**
 * Function scampaign_delete_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function scampaign_delete_form_submit($form, &$form_state) {
  // Set log and message arguments.
  $t_args = array('%label' => $form_state['values']['campaign']);
  // Check the campaign.
  $result = db_query('SELECT rules_name FROM {scampaign} WHERE campaign = :campaign', array(
    ':campaign' => $form_state['values']['campaign'],
  ));
  if ($result->rowCount() > 0) {
    // Delete the campaign.
    db_delete('scampaign')->condition('campaign', $form_state['values']['campaign'])->execute();
    // Notify the admin.
    drupal_set_message(t('Sepulsa campaign configuration %label has been deleted.', $t_args));
    watchdog('scampaign', 'Deleted sepulsa campaign configuration %label.', $t_args, WATCHDOG_NOTICE);
  }
  else {
    drupal_set_message(t('Sepulsa campaign configuration %label not found.', $t_args), 'error');
  }

  // Redirect to main page.
  $form_state['redirect'] = 'admin/people/scampaign/main';
}
