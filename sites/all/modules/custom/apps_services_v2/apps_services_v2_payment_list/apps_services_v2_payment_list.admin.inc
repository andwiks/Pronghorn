<?php

function apps_services_v2_payment_list_credit_setting_form($node, $form_state) {
    
    $form['apps_services_v2_payment_list_credit_option'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable/Disable Credit Use in Checkout Payment'),
        '#default_value' => variable_get('apps_services_v2_payment_list_credit_option', 1),
    );
  
    $form['apps_services_v2_payment_list_credit_title_text'] = array(
        '#type' => 'textfield',
        '#title' => 'Credit Title Choice Text',
        '#default_value' => variable_get("apps_services_v2_payment_list_credit_title_text", "Sepulsa Kredit Rp @credit"),
        '#required' => TRUE,
        '#description' => 'The Title Shown for Credit Payment in Checkout Payment, @credit is the holder for the credit total',
    );
    
    $form['apps_services_v2_payment_list_credit_subtitle_text'] = array(
        '#type' => 'textfield',
        '#title' => 'Credit Subtitle Choice Text',
        '#default_value' => variable_get("apps_services_v2_payment_list_credit_subtitle_text", "gunakan sepulsa kredit untuk membayar"),
        '#required' => TRUE,
        '#description' => 'The Subtitle Shown for Credit Payment in Checkout Payment',
    );
    
    // Return form array.
    return system_settings_form($form);
}

