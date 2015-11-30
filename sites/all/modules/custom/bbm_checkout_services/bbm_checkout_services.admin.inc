<?php

function bbm_checkout_services_setting_form($node, $form_state) {
    
    $form['bbm_checkout_services_ip_block'] = array(
        '#type' => 'fieldset',
        '#title' => t('IP Filter'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    
    $form['bbm_checkout_services_ip_block']['bbm_checkout_services_whitelist'] = array(
        '#type' => 'textarea',
        '#title' => t('IP Whitelist'),
        '#default_value' => variable_get("bbm_checkout_services_whitelist", "127.0.0.1,::1"),
        '#required' => TRUE,
        '#description' => 'list of ip that can access BBM checkout api, separated with coma',
    );
    
    $form['bbm_checkout_services_ip_block']['bbm_checkout_services_whitelist_enable_watchdog'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable/Disable Ip Whitelist Watchdog'),
        '#default_value' => variable_get('bbm_checkout_services_whitelist_enable_watchdog', 1),
    );
    
    $form['bbm_checkout_services_signature'] = array(
        '#type' => 'fieldset',
        '#title' => t('Veritrans Signature'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    
    $form['bbm_checkout_services_signature']['bbm_checkout_services_server_key'] = array(
        '#type' => 'textfield',
        '#title' => t('Veritrans Server Key'),
        '#default_value' => variable_get("bbm_checkout_services_server_key", "VT-server-XS4LpMKEMvrl8NbKMx2hdWUe"),
        '#required' => TRUE,
        '#description' => 'Server key from veritrans to generate signature',
    );
    
    $form['bbm_checkout_services_signature']['bbm_checkout_services_check_signature'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable/Disable Check Signature Veritrans'),
        '#default_value' => variable_get('bbm_checkout_services_check_signature', 1),
    );

    // Return form array.
    return system_settings_form($form);
}

