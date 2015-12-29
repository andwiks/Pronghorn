<?php
// admin page.
function auto_bank_transfer_admin_form($form, &$form_state) {
  // load order by total order

  $form['bca_csv_file_folder'] = array(
    '#type' => 'textfield',
    '#title' => t('BCA CSV file folder'),
    '#required' => true,
    '#default_value' => variable_get('bca_csv_file_folder', 'sites/default/files/bank/bca')
  );
  
  $form['activate_bca_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate BCA Cron'),
    '#default_value' => variable_get('activate_bca_cron', 1)
  );
  
  $form['mandiri_csv_file_folder'] = array(
    '#type' => 'textfield',
    '#title' => t('Mandiri CSV file folder'),
    '#required' => true,
    '#default_value' => variable_get('mandiri_csv_file_folder', 'sites/default/files/bank/mandiri')
  );
  
  $form['activate_mandiri_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate Mandiri Cron'),
    '#default_value' => variable_get('activate_mandiri_cron', 0)
  );

  $form['auto_bank_transfer_CS'] = array(
    '#type' => 'textfield',
    '#title' => t('CS email to get notifications'),
    '#description' => t('List of email to get notified when there are more than 1 order to confirm.'),
    '#required' => true,
    '#default_value' => variable_get('auto_bank_transfer_CS', 'yeti@sepulsa.com')
  );
  $form['auto_bank_transfer_logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Logging'),
    '#default_value' => variable_get('auto_bank_transfer_logging', 0),
    '#description' => t('Enable database logging and raw CSV backup.')
  );
  
  return system_settings_form($form);
}

function auto_bank_transfer_filter_form($form, &$form_state) {
  $form['created'] = array('#type' => 'fieldset',
    '#title' => t('Created Date Filter'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $format = 'Y-m-d';

  $form['#attributes'] = array('class' => array('search-form'));
  $form['filter'] = array('#type' => 'fieldset',
    '#title' => t('Basic Filter'),
    '#attributes' => array('class' => array('container-inline')),
  );

  if($_SESSION['auto_bank_transfer_overview'] == 'bca'){
    $keys = isset($_SESSION['auto_bank_transfer_bca_filter']) ? $_SESSION['auto_bank_transfer_bca_filter'] : array();
  }else{
    $keys = isset($_SESSION['auto_bank_transfer_mandiri_filter']) ? $_SESSION['auto_bank_transfer_mandiri_filter'] : array();
  }
  if(empty($keys)){
    //provide default value
    $keys['created_from'] = '';
    $keys['created_to'] = '';
    $keys['status'] = 'all';
    $keys['amount_from'] = '';
    $keys['amount_to'] = '';
  }else{
    $form['filter']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#submit' => array('auto_bank_transfer_filter_form_submit_reset'),
      '#weight' => 100,
    );
  }

  $form['created']['created_from'] = array(
    '#type' => 'date_popup',
    '#default_value' => $keys['created_from'],
    '#date_format' => $format,
    '#date_label_position' => 'none',
    '#date_year_range' => '-1:+0',
    '#description' => NULL,
  );

  $form['created']['created_to'] = array(
    '#type' => 'date_popup',
    '#title' => 'Created To',
    '#title_display' => 'invisible',
    '#default_value' => $keys['created_to'],
    '#date_format' => $format,
    '#date_label_position' => 'none',
    '#description' => '',
  );  
  $form['filter']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $keys['status'],
    '#options' => array('all' => 'All', 'Success' => 'Success', 'Failed' => 'Failed')
  );
  $form['filter']['amount_from'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#default_value' => $keys['amount_from'],
    '#size' => 10,
  );
  $form['filter']['amount_to'] = array(
    '#type' => 'textfield',
    '#title' => t(' - '),
    '#default_value' => $keys['amount_to'],
    '#size' => 10,
  );  

  $form['filter']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('auto_bank_transfer_filter_form_submit_filter'),
  );
  return $form;
}

function auto_bank_transfer_filter_form_submit_filter($form, &$form_state) {
  if($_SESSION['auto_bank_transfer_overview'] == 'bca'){
    $_SESSION['auto_bank_transfer_bca_filter'] = $form_state['values'];
    $form_state['redirect'] = 'admin/commerce/config/abt/bca_overview/';
  }else{
    $_SESSION['auto_bank_transfer_mandiri_filter'] = $form_state['values'];
    $form_state['redirect'] = 'admin/commerce/config/abt/mandiri_overview/';
  }
}

function auto_bank_transfer_filter_form_submit_reset($form, &$form_state) {
  if($_SESSION['auto_bank_transfer_overview'] == 'bca'){
    $_SESSION['auto_bank_transfer_bca_filter'] = array();
    $form_state['redirect'] = 'admin/commerce/config/abt/bca_overview';
  }else{
    $_SESSION['auto_bank_transfer_mandiri_filter'] = array();
    $form_state['redirect'] = 'admin/commerce/config/abt/mandiri_overview';
  }
}

//function to list BCA transaction table
function auto_bank_transfer_bca_overview(){
  $_SESSION['auto_bank_transfer_overview'] = 'bca';
  // Add the filter form above the overview table.
  $build['auto_bank_transfer_filter_form'] = drupal_get_form('auto_bank_transfer_filter_form');

  $header = array();
  $header[] = array('data' => t('ID'), 'field' => 'abbid', 'sort' => 'desc');
  $header[] = array('data' => t('Description'));
  $header[] = array('data' => t('Amount'), 'field' => 'amount');
  $header[] = array('data' => t('Created'), 'field' => 'created');
  $header[] = array('data' => t('Updated'), 'field' => 'updated');
  $header[] = array('data' => t('Order ID'), 'field' => 'order_id');
  $header[] = array('data' => t('Status'), 'field' => 'status');
  $header[] = array('data' => t('Note'));

  $query = db_select('auto_bank_bca')->extend('PagerDefault')->extend('TableSort');

  $keys = isset($_SESSION['auto_bank_transfer_bca_filter']) ? $_SESSION['auto_bank_transfer_bca_filter'] : array();
  if($keys){
    if(!empty($keys['created_from'])){
      $query->condition('created', strtotime($keys['created_from']), '>=');  
    }
    if(!empty($keys['created_to'])){
      $query->condition('created', strtotime($keys['created_to']), '<=');  
    }
    if(!empty($keys['amount_from'])){
      $query->condition('amount', $keys['amount_from'], '>=');  
    }
    if(!empty($keys['amount_to'])){
      $query->condition('amount', $keys['amount_to'], '<=');  
    }
    if(!empty($keys['status']) && $keys['status'] != 'all'){
      $query->condition('status', $keys['status'], '=');  
    }
  }

  $result = $query
  ->fields('auto_bank_bca')
    ->orderByHeader($header)
    ->limit(50)
    ->execute();

  $rows = array();
  
  foreach ($result as $data) {
    $row = array();
    $row['data']['abbid'] = $data->abbid;
    $row['data']['description'] = $data->description;
    $row['data']['amount'] = number_format($data->amount);
    $row['data']['created'] = date('F j, G:i',$data->created);
    $row['data']['updated'] = date('F j, G:i',$data->updated);
    $row['data']['order_id'] = l($data->order_id,'admin/commerce/orders/'.$data->order_id);
    $row['data']['status'] = $data->status;
    $row['data']['note'] = $data->note;
    
    // Highlight any non-success transactions
    if ($data->status != 'Success') {
      $row['class'] = array('warning');
    }

    if ($data->status == 'Failed') {
      $row['class'] = array('error');
    }

    $rows[] = $row;
  }

  $build['auto_bank_transfer_bca_overview_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No Transactions Logged Yet.'),
  );
  $build['auto_bank_transfer_bca_overview_pager'] = array('#theme' => 'pager');

  return $build;
}


//function to list Mandiri transaction table
function auto_bank_transfer_mandiri_overview(){
  $_SESSION['auto_bank_transfer_overview'] = 'mandiri';
  // Add the filter form above the overview table.
  $build['auto_bank_transfer_filter_form'] = drupal_get_form('auto_bank_transfer_filter_form');

  $header = array();
  $header[] = array('data' => t('ID'), 'field' => 'abmid', 'sort' => 'desc');
  $header[] = array('data' => t('Description'));
  $header[] = array('data' => t('Amount'), 'field' => 'amount');
  $header[] = array('data' => t('Created'), 'field' => 'created');
  $header[] = array('data' => t('Updated'), 'field' => 'updated');
  $header[] = array('data' => t('Order ID'), 'field' => 'order_id');
  $header[] = array('data' => t('Status'), 'field' => 'status');
  $header[] = array('data' => t('Note'));

  $query = db_select('auto_bank_mandiri')->extend('PagerDefault')->extend('TableSort');

  $keys = isset($_SESSION['auto_bank_transfer_mandiri_filter']) ? $_SESSION['auto_bank_transfer_mandiri_filter'] : array();
  if($keys){
    if(!empty($keys['created_from'])){
      $query->condition('created', strtotime($keys['created_from']), '>=');  
    }
    if(!empty($keys['created_to'])){
      $query->condition('created', strtotime($keys['created_to']), '<=');  
    }
    if(!empty($keys['amount_from'])){
      $query->condition('amount', $keys['amount_from'], '>=');  
    }
    if(!empty($keys['amount_to'])){
      $query->condition('amount', $keys['amount_to'], '<=');  
    }
    if(!empty($keys['status']) && $keys['status'] != 'all'){
      $query->condition('status', $keys['status'], '=');  
    }
  }

  $result = $query
  ->fields('auto_bank_mandiri')
    ->orderByHeader($header)
    ->limit(50)
    ->execute();

  $rows = array();
  
  foreach ($result as $data) {
    $row = array();
    $row['data']['abmid'] = $data->abmid;
    $row['data']['description'] = $data->description;
    $row['data']['amount'] = number_format($data->amount);
    $row['data']['created'] = date('F j, G:i',$data->created);
    $row['data']['updated'] = date('F j, G:i',$data->updated);
    $row['data']['order_id'] = l($data->order_id,'admin/commerce/orders/'.$data->order_id);
    $row['data']['status'] = $data->status;
    $row['data']['note'] = $data->note;
    
    // Highlight any non-success transactions
    if ($data->status != 'Success') {
      $row['class'] = array('warning');
    }

    if ($data->status == 'Failed') {
      $row['class'] = array('error');
    }

    $rows[] = $row;
  }

  $build['auto_bank_transfer_mandiri_overview_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No Transactions Logged Yet.'),
  );
  $build['auto_bank_transfer_mandiri_overview_pager'] = array('#theme' => 'pager');

  return $build;
}


//function to get content of BCA CSV file
function bca_list_file() {
  $dir = file_default_scheme() . '://bank/summary_bca';
  $files = abt_getCSVFiles($dir);
  //only read latest
  $filepath = $files[0];

  $row = 1;
  if (($handle = fopen($filepath, "r")) !== FALSE) {
    $output = "<p><b>".$filepath."</b> "  . date ("F d Y H:i:s.", filemtime($filepath)) . "</p>\n";
    $output .= "<table>";
    while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
      $num = count($data);
      if ($num == 5) {
        $output .= "<tr>";
        $row++;
        for ($c=0; $c < $num; $c++) {
          $output .= "<td>";
          $output .= $data[$c];
          $output .= "</td>";
        }
        $output .= "</tr>";
      }

    }
    $output .= "</table>";
    fclose($handle);
  }
  
  drupal_add_css('.field-item.even table tr td {border: 1px solid #000;padding: 5px;}', array('type' => 'inline'));
  return $output;
}


//function to get content of Mandiri CSV file
function mandiri_list_file() {
  $dir = file_default_scheme() . '://bank/summary_mandiri';
  $files = abt_getCSVFiles($dir);
  //only read latest
  $filepath = $files[0];

  $row = 1;
  if (($handle = fopen($filepath, "r")) !== FALSE) {
    $output = "<p><b>".$filepath."</b> " . date ("F d Y H:i:s.", filemtime($filepath)) . "</p>\n";
    $output .= "<table>";
    while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
      $num = count($data);
      if ($num == 10) {
        $output .= "<tr>";
        $row++;
        for ($c=0; $c < $num; $c++) {
          $output .= "<td>";
          $output .= $data[$c];
          $output .= "</td>";
        }
        $output .= "</tr>";
      }

    }
    $output .= "</table>";
    fclose($handle);
  }
  
  drupal_add_css('.field-item.even table tr td {border: 1px solid #000;padding: 5px;}', array('type' => 'inline'));
  return $output;
}
