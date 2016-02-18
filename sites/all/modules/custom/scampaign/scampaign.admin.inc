<?php
/**
  * Main form function to create the link
  */
function scampaign_add_form($form, &$form_state){
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
    '#description' => t('How many link you want to create or Maximum use for non unique link. Max 820812'),
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
function scampaign_add_form_validate($form, &$form_state){
  $result = db_query('SELECT rules_name FROM {scampaign} WHERE campaign = :campaign',array(':campaign' => $form_state['values']['campaign']));
  
  if($result->rowCount() > 0){
    // Campaign name already exist
    if($rule->rules_name != $form_state['values']['rules_name']){
      form_set_error('campaign','Campaign name already exist with different rule');
    }
  }
  if($form_state['values']['max_use'] > 820812){
    form_set_error('max_use','Cannot exceed 820812.');
  }
}

/**
  * Form submit to create links
  */
function scampaign_add_form_submit($form, &$form_state){
  if($form_state['values']['unique']){
    // If unique is ticked, create unique link for each counter
    for ($i=0; $i < $form_state['values']['max_use']; $i++) { 
      db_insert('scampaign')
      ->fields(array(
        'campaign' => $form_state['values']['campaign'],
        'rules_name' => $form_state['values']['rules_name'],
      ))
      ->execute();
    }
  } else {
    db_insert('scampaign')
    ->fields(array(
      'campaign' => $form_state['values']['campaign'],
      'rules_name' => $form_state['values']['rules_name'],
      'max_use' => $form_state['values']['max_use'],
      'email' => serialize(array())
    ))
    ->execute();
  }
}

/**
  * Campaign Link Overview
  */
function scampaign_main(){
  // Select all campaign
  $result = db_query('SELECT * FROM {scampaign}');
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
      l('Detail','admin/people/scampaign/'.$key.'/detail',array('attributes' => array('target'=>'_blank')))
    );
  }
  $output = theme('table',array('header' => $headers, 'rows' => $rows));
  return $output;
}

/**
  * Detail page filter form
  */
function scampaign_detail_filter_form($form, &$form_state) {
  $form['#attributes'] = array('class' => array('search-form'));
  $form['filter'] = array('#type' => 'fieldset',
    '#title' => t('Basic Filter'),
    '#attributes' => array('class' => array('container-inline')),
  );
  
  $keys = isset($_SESSION['scampaign_detail_filter']) ? $_SESSION['scampaign_detail_filter'] : array();
  
  if(empty($keys)){
    //provide default value
    $keys['used'] = 0;
  }else{
    $form['filter']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#submit' => array('scampaign_detail_filter_form_submit_reset'),
      '#weight' => 100,
    );
  }

  $form['filter']['used'] = array(
    '#type' => 'checkbox',
    '#title' => 'Only used link',
    '#default_value' => $keys['used'],
  );

  $form['filter']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('scampaign_detail_filter_form_submit_filter'),
  );
  return $form;
}

function scampaign_detail_filter_form_submit_filter($form, &$form_state) {
  $_SESSION['scampaign_detail_filter'] = $form_state['values'];
  $form_state['redirect'] = 'admin/people/scampaign/'.$_SESSION['scampaign_detail_campaign'].'/detail';
}

function scampaign_detail_filter_form_submit_reset($form, &$form_state) {
  $_SESSION['scampaign_detail_filter'] = array();
  $form_state['redirect'] = 'admin/people/scampaign/'.$_SESSION['scampaign_detail_campaign'].'/detail';
}
/**
  * Campaign Link Detail
  * @param string $campaign name
  */
function scampaign_detail($campaign){
  $_SESSION['scampaign_detail_campaign'] = $campaign;
  $header = array();
  $header[] = array('data' => t('ID'), 'field' => 'id', 'sort' => 'asc');
  $header[] = array('data' => t('Link'));
  $header[] = array('data' => t('Link Count'), 'field' => 'max_use');
  $header[] = array('data' => t('Used'), 'field' => 'used');
  $header[] = array('data' => t('Email'));

  $query = db_select('scampaign')->extend('PagerDefault')->extend('TableSort');

  $query->condition('campaign',$campaign,'=');

  $keys = isset($_SESSION['scampaign_detail_filter']) ? $_SESSION['scampaign_detail_filter'] : array();
  if($keys){
    if(!empty($keys['used'])){
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
    $crypt = $value->id.$rand.$value->id;
    $row = array(
      'id' => $value->id,
      'link' => alphaID($crypt,false,10),
      'max_use' => $value->max_use,
      'used' => $value->used
    );
    $emails = unserialize($value->email);
    empty($emails) ? $row['email'] = '' : $row['email'] = implode(', ',$emails);
    $arr[] = $row;
  }
  $output['scampaign_detail_filter_form'] = drupal_get_form('scampaign_detail_filter_form');
  $output['scampaign_detail_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $arr,
  );
  $output['scampaign_detail_pager'] = array('#theme' => 'pager');

  $output['scampaign_export'] = array(
    '#theme' => 'link',
    '#text' => 'Export ALL Link to CSV',
    '#path' => 'admin/people/scampaign/'.$campaign.'/detail/export',
    '#options' => array(
      'attributes' => array('target' => '_blank'),
        //REQUIRED:
        'html' => FALSE,
     ),
  );
  return $output;
}

/**
  * Campaign Link Detail Export Callback
  * @param string $campaign name
  */
function scampaign_detail_export($campaign){
  $result = db_query('SELECT * FROM {scampaign} WHERE campaign = :campaign',array(':campaign' => $campaign));
  $arr = array();
  $rand = '820812';
  foreach ($result as $key => $value) {
    $crypt = $value->id.$rand.$value->id;
    $link = alphaID($crypt,false,10);
    $row = array(
      'link' => $link,
    );
    if($value->max_use != 1) {
      $row['max_use'] = $value->max_use;
      $row['used'] = $value->used;
    }
    $emails = unserialize($value->email);
    empty($emails) ? $row['email'] = '' : $row['email'] = implode(', ',$emails);
    $arr[] = $row;
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