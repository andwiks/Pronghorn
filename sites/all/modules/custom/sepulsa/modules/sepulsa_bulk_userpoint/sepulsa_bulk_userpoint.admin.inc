<?php
/**
 * File sepulsa_bulk_userpoint.admin.inc
 * Created By : Antonius Ananto
 * File Usage : Form Init
 */
function userpoint_bulk_form($form = array(), &$form_state = array()){
  $form['fid'] = array(
    '#type' => 'managed_file',
    '#title' => t('File to Bulk Transaction'),
    '#description' => '"username" , "points(+/- without . or ,)"',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('csv'),
    ),
    '#required' => TRUE,
  );
  
  if (module_exists('taxonomy')) {
    $options = userpoints_get_categories();
    $form['tid'] = array(
      '#type' => 'select',
      '#title' => t('Category'),
      '#default_value' => isset($txn->tid) ? $txn->tid : userpoints_get_default_tid(),
      '#options' => $options,
      '#description' => t('The !points category that should apply to this bulk transaction.', userpoints_translation()),
      '#weight' => 0,
      '#access' => count($options) > 1,
      '#required' => TRUE,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function userpoint_bulk_form_submit($form, &$form_state){
  $uri =  db_query("SELECT uri FROM {file_managed} WHERE fid = :fid", array(
            ':fid' => $form_state['values']['fid'],
          ))->fetchField();
  if(!empty($uri) ){
    if(file_exists(drupal_realpath($uri))) {
        $operations = array();
        $handle = fopen(drupal_realpath($uri), "r");
        while (($data = fgetcsv($handle, 0, ',', '"')) !== FALSE) {
          $item = array (
            'uid' => get_uid_from_username($data[0]),
            'points' => $data[1],
            'tid' => $form_state['values']['tid'],
            'description' => '[BULK] ' . isset($data[2]) ? $data[2] : 'No Reason',
          ); 
          $operations[] = array(
                  'bulk_userpoint',  
                  array($item),  
                );
        }
        $batch = array(
              'title' => t('Bulk UserPoint ...'),
              'operations' => $operations,  
              'finished' => 'bulk_userpoint_finish', 
              'error_message' => t('Something Went Wrong When Appending User Point Data.'),
              'progress_message' => t('Adding @current of @total User Point Data.'),
            );
            batch_set($batch);
            fclose($handle); 
    }  
  } else {
    drupal_set_message(t('File Not Exsist')); 
  }
}

function get_uid_from_username($username){
  $query = new EntityFieldQuery;
  $result = $query
    ->entityCondition('entity_type', 'user')
    ->propertyCondition('name', trim($username))
    ->execute();
  $ufindser = array_keys($result['user']);
  return $ufindser[0];  
}

function bulk_userpoint_finish(){
  drupal_set_message(t('Bulk User Point Success'));  
}

function bulk_userpoint($item) {
  $params = array (
    'uid' => $item['uid'],
    'points' => $item['points'],
    'tid' => $item['tid'],
    'description' => $item['description'],
  );
  userpoints_userpointsapi($params);
}

