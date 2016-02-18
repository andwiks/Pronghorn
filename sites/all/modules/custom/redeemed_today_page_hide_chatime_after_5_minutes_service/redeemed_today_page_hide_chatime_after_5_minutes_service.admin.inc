<?php

function redeemed_today_page_hide_chatime_after_5_minutes_service_setting_form($node, $form_state) {
    
    $form['redeemed_today_page_hide_chatime_after_5_minutes_service_title_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Title Voucher'),
        '#default_value' => variable_get("redeemed_today_page_hide_chatime_after_5_minutes_service_title_name", "chatime"),
        '#required' => TRUE,
        '#description' => 'Containing text of Voucher Title that need to be hidden',
    );
    
    $form['redeemed_today_page_hide_chatime_after_5_minutes_service_time_minutes'] = array(
        '#type' => 'textfield',
        '#title' => t('Hidden After x minutes'),
        '#default_value' => variable_get("redeemed_today_page_hide_chatime_after_5_minutes_service_time_minutes", "5"),
        '#required' => TRUE,
        '#description' => 'How Long the Voucher will be shown in Redeemed Page (in minutes)',
    );

    // Return form array.
    return system_settings_form($form);
}

