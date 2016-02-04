<?php

function apps_services_v2_init_request_setting_form($node, $form_state) {
    
    $form['apps_services_v2_init_request_cache_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Cache Id'),
        '#default_value' => variable_get("apps_services_v2_init_request_cache_id", "XcF1d"),
        '#required' => TRUE,
        '#description' => 'Current active Cache Id',
    );
    
    $form['android'] = array(
        '#type' => 'fieldset',
        '#title' => t('Android'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    
    $form['android']['apps_services_v2_init_request_android_current_version'] = array(
        '#type' => 'textfield',
        '#title' => t('Android Current Version'),
        '#default_value' => variable_get("apps_services_v2_init_request_android_current_version", ""),
        '#required' => FALSE,
        '#description' => 'Current active Android Version',
    );
    
    $form['android']['apps_services_v2_init_request_android_google_play_update_link'] = array(
        '#type' => 'textfield',
        '#title' => t('Android Google Play Update Link'),
        '#default_value' => variable_get("apps_services_v2_init_request_android_google_play_update_link", "market://details?id=com.sepulsa.android"),
        '#required' => FALSE,
        '#description' => 'Android Google Play Update Link',
    );
    
    $form['android']['apps_services_v2_init_request_android_non_google_play_update_link'] = array(
        '#type' => 'textfield',
        '#title' => t('Android Non Google Play Update Link'),
        '#default_value' => variable_get("apps_services_v2_init_request_android_non_google_play_update_link", "https://play.google.com/store/apps/details?id=com.sepulsa.android"),
        '#required' => FALSE,
        '#description' => 'Android Google Play Update Link',
    );
    
    $form['android']['apps_services_v2_init_request_android_force_update'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable/Disable Force Update on Android'),
        '#default_value' => variable_get('apps_services_v2_init_request_android_force_update', 0),
    );
    
    $form['ios'] = array(
        '#type' => 'fieldset',
        '#title' => t('Ios'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    
    $form['ios']['apps_services_v2_init_request_ios_current_version'] = array(
        '#type' => 'textfield',
        '#title' => t('Ios Current Version'),
        '#default_value' => variable_get("apps_services_v2_init_request_ios_current_version", ""),
        '#required' => FALSE,
        '#description' => 'Current active Ios Version',
    );
    
    $form['ios']['apps_services_v2_init_request_ios_update_link'] = array(
        '#type' => 'textfield',
        '#title' => t('Ios Update Link'),
        '#default_value' => variable_get("apps_services_v2_init_request_ios_update_link", "https://itunes.apple.com/id/app/sepulsa/id991045758?mt=8"),
        '#required' => FALSE,
        '#description' => 'Ios Update Link',
    );
    
    $form['ios']['apps_services_v2_init_request_ios_force_update'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable/Disable Force Update on Ios'),
        '#default_value' => variable_get('apps_services_v2_init_request_ios_force_update', 0),
    );
    
    $form['maintenance'] = array(
        '#type' => 'fieldset',
        '#title' => t('Maintenance'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    
    $form['maintenance']['apps_services_v2_init_request_maintenance'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable/Disable Maintenance Mode on Apps'),
        '#default_value' => variable_get('apps_services_v2_init_request_maintenance', 0),
    );
    
    // Return form array.
    return system_settings_form($form);
}

