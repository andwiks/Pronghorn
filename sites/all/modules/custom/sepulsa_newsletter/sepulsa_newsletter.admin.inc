<?php
/**
 * @file
 * sepulsa_newsletter.admin.inc
 *
 * @author aizat@sepulsa.com
 */

/**
 * Function sepulsa_newsletter_form().
 */
function sepulsa_newsletter_form($form, &$form_state) {

  $form['sepulsa_newsletter_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailchimp API URL'),
    '#description' => 'Use https:// , example : https://us12.api.mailchimp.com/3.0/lists',
    '#size' => 200,
    '#maxlength' => 200,
    '#default_value' => variable_get('sepulsa_newsletter_api_url',
      'https://us12.api.mailchimp.com/3.0/lists'),
    '#required' => TRUE,
  );

  $form['sepulsa_newsletter_id_member'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailchimp ID Member'),
    '#size' => 200,
    '#maxlength' => 200,
    '#default_value' => variable_get('sepulsa_newsletter_id_member',
      'a38de08c0b'),
    '#required' => TRUE,
  );

  $form['sepulsa_newsletter_webhook_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Webhook Key'),
    '#size' => 200,
    '#maxlength' => 200,
    '#default_value' => variable_get('sepulsa_newsletter_webhook_key',
      'dev19isokenoproblemooo!'),
    '#required' => TRUE,
  );

  $form['auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic Auth Configuration'),
    '#weight' => 5,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
 );

  $form['auth']['sepulsa_newsletter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username Mailchimp'),
    '#size' => 200,
    '#maxlength' => 200,
    '#default_value' => variable_get('sepulsa_newsletter_username',
      'sepulsanews'),
    '#required' => TRUE,
  );

  $form['auth']['sepulsa_newsletter_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#size' => 200,
    '#maxlength' => 200,
    '#default_value' => variable_get('sepulsa_newsletter_apikey',
      '0cf0d5df56cc65702c6c5a7d404ac244'),
    '#required' => TRUE,
  );

  $form['auth']['sepulsa_newsletter_appkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Application Key'),
    '#description' => 'Application Key is the front of the url,
    example url https://us12.api.mailchimp.com/3.0/lists,
    application key : us12',
    '#size' => 200,
    '#maxlength' => 200,
    '#default_value' => variable_get('sepulsa_newsletter_appkey', 'us12'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Function sepulsa_newsletter_import_form().
 */
function sepulsa_newsletter_import_form() {
  $form['#attributes']['enctype'] = "multipart/form-data";
  $form['csv_upload'] = array(
    '#type' => 'file',
    '#title' => t('CSV File from Mailchimp. (Please split CSV to 1000 record)'),
    '#size' => 40,
    '#description' => t('Import Subscriber / Unsubscriber in Mailchimp.'),
  );
  $form['flag_subscriber'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mailchimp Subscriber ?'),
    '#description' => t('Check if you want to import subscribed user in mailchimp.'),
    '#default_value' => 0,
    '#return_value' => 1,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#title' => 'Submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * Function sepulsa_newsletter_import_form_validate().
 */
function sepulsa_newsletter_import_form_validate($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('csv'));
  $file = file_save_upload('csv_upload', $validators);
  // Check file uploaded OK.
  if (!$file) {
    form_set_error('csv_upload', t('A file must be uploaded'));
  }
  elseif ($file->filemime != 'text/csv') {
    form_set_error('csv_upload', t('The file must be of CSV type only.'));
  }
  else {
    // Set files to form_state, to process when form is submitted.
    $form_state['values']['csv_upload'] = $file;
  }
}

/**
 * Function sepulsa_newsletter_import_form_submit().
 */
function sepulsa_newsletter_import_form_submit($form, &$form_state) {
  // Get temp destination file.
  $filepath = $form_state['values']['csv_upload']->destination;
  // Open file.
  $handle = @fopen($filepath, "r");
  $report_watchdog = array();
  $i = 0;
  // Loop CSV file (1000 first record exclude header).
  while ($row = fgetcsv($handle, 1001, ',')) {
    if ($i != 0) {
      $result = db_select('users', 'u');
      $result->fields('u', array('uid'));
      $result->condition('u.mail', $row[0]);
      $result_fix = $result->execute()->fetchAssoc();

      if (isset($result_fix) && !empty($result_fix)) {
        $user = user_load($result_fix['uid']);
        $user->field_newsletter_subscribers[LANGUAGE_NONE][0]['value'] = $form_state['values']['flag_subscriber'];
        user_save($user);
        $report_watchdog[$i] = $row[0] . ' field_newsletter_subscribers change to ' . $form_state['values']['flag_subscriber'];
      }
      else {
        $report_watchdog[$i] = $row[0] . ' Not Found !';
      }
      $i++;
    }
    else {
      $i++;
    }
  }

  watchdog('sepulsa_newsletter',
    'Mailchimp Import : data : <pre> @report_watchdog </pre>',
    array('@report_watchdog' => print_r($report_watchdog, TRUE)));
}
