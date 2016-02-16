<?php
/**
  * Main form function to create the link
  */
function campaign_link_add_form($form, &$form_state){
  $form = array();
  $form['campaign'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Campaign Name'),
    '#required' => true
  );

  // Get rules compontents name and label
  $rules = rules_get_components(true,'action');
  $form['rules_name'] = array(
    '#type' => 'select',
    '#title' => t('Rules Action'),
    '#description' => t('Rules Action to fire'),
    '#options' => $rules,
  );

  $form['max_use'] = array(
    '#type' => 'textfield',
    '#title' => t('Link Count'),
    '#description' => t('How many link you want to create or Maximum use for non unique link.'),
    '#default_value' => 1,
    '#required' => true
  );

  $form['unique'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unique'),
    '#description' => t('If checked, link will be created with 1 max use for each count.')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}
/**
  * Form validate to create links
  */
function campaign_link_add_form_validate($form, &$form_state){
  $result = db_query('SELECT rules_name FROM {campaign_link} WHERE campaign = :campaign',array(':campaign' => $form_state['values']['campaign']));
  
  if($result->rowCount() > 0){
    // Campaign name already exist
    if($rule->rules_name != $form_state['values']['rules_name']){
      form_set_error('campaign','Campaign name already exist with different rule');
    }
  }
}

/**
  * Form submit to create links
  */
function campaign_link_add_form_submit($form, &$form_state){
  if($form_state['values']['unique']){
    // If unique is ticked, create unique link for each counter
    for ($i=0; $i < $form_state['values']['max_use']; $i++) { 
      db_insert('campaign_link')
      ->fields(array(
        'campaign' => $form_state['values']['campaign'],
        'rules_name' => $form_state['values']['rules_name'],
      ))
      ->execute();
    }
  } else {
    db_insert('campaign_link')
    ->fields(array(
      'campaign' => $form_state['values']['campaign'],
      'rules_name' => $form_state['values']['rules_name'],
      'max_use' => $form_state['values']['max_use'],
    ))
    ->execute();
  }
}

/**
  * Campaign Link Overview
  */
function campaign_link_main(){
  // Select all campaign
  $result = db_query('SELECT * FROM {campaign_link}');
  $arr = array();
  foreach($result as $key => $value){
    // Group into array with campaign name as key
    $arr[$value->campaign]['rules_name'] = $value->rules_name;
    isset($arr[$value->campaign]['max_use']) ? $arr[$value->campaign]['max_use'] += $value->max_use : $arr[$value->campaign]['max_use'] = $value->max_use;
    isset($arr[$value->campaign]['used']) ? $arr[$value->campaign]['used'] += $value->used : $arr[$value->campaign]['used'] = $value->used;
  }

  $headers = array('Campaign', 'Rules Component', 'Link Count', 'Link Used', 'Detail');
  $rows = array();
  foreach($arr as $key => $value){
    $rows[] = array(
      $key, 
      $value['rules_name'], 
      $value['max_use'], 
      $value['used'], 
      l('Detail','admin/people/campaign_link/'.$key.'/detail',array('attributes' => array('target'=>'_blank')))
    );
  }
  $output = theme('table',array('header' => $headers, 'rows' => $rows));
  return $output;
}

/**
  * Campaign Link Detail
  * @param string $campaign name
  */
function campaign_link_detail($campaign){
  $result = db_query('SELECT * FROM {campaign_link} WHERE campaign = :campaign',array(':campaign' => $campaign));
  $arr = array();
  foreach ($result as $key => $value) {
    $arr[] = array(
      'link' => alphaID($value->id,false,5),
      'max_use' => $value->max_use,
      'used' => $value->used,
    );
  }
  $header = array('Unique Link', 'Link Count', 'Used');
  $output = theme('table',array('header' => $header, 'rows' => $arr));
  $output .= l('Export Link to CSV','admin/people/campaign_link/'.$campaign.'/detail/export');
  return $output;
}

/**
  * Campaign Link Detail Export Callback
  * @param string $campaign name
  */
function campaign_link_detail_export($campaign){
  $result = db_query('SELECT * FROM {campaign_link} WHERE campaign = :campaign',array(':campaign' => $campaign));
  $arr = array();
  foreach ($result as $key => $value) {
    $arr[] = array(
      'link' => alphaID($value->id,false,5),
    );
  }
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=campaign-link.csv');

  $output = '';

  foreach ($arr as $link) {
    $user = user_load($member->uid);
    $output .= implode(',',$link) . "\n";
  }
  ob_clean();
  print $output;
  exit;
}