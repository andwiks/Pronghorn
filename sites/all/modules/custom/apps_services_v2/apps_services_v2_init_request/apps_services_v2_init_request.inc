<?php

function apps_services_v2_init_request_validation_and_get_init_services($phone_number, $session, $source){
    $return = array();
    
    //set validation of phone number and session here
    //currently return 0
    $return['validation'] = 0;

    //cache id
    $return["cache_id"] = variable_get("apps_services_v2_init_request_cache_id", "XcF1d");
    
    //android
    $return["android_current_version"] = variable_get("apps_services_v2_init_request_android_current_version", "");
    $return["android_google_play_link"] = variable_get("apps_services_v2_init_request_android_google_play_update_link", "market://details?id=com.sepulsa.android");
    $return["android_non_google_play_link"] = variable_get("apps_services_v2_init_request_android_non_google_play_update_link", "https://play.google.com/store/apps/details?id=com.sepulsa.android");
    $return["android_force_update"] = variable_get('apps_services_v2_init_request_android_force_update', 0);
    
    //ios
    $return["ios_current_version"] = variable_get("apps_services_v2_init_request_ios_current_version", "");
    $return["ios_link"] = variable_get("apps_services_v2_init_request_ios_update_link", "https://itunes.apple.com/id/app/sepulsa/id991045758?mt=8");
    $return["ios_force_update"] = variable_get('apps_services_v2_init_request_ios_force_update', 0);
    
    //maintenance
    $return["maintenance"] = variable_get('apps_services_v2_init_request_maintenance', 0);
    
    return $return;
}