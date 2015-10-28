<?php

function facebook_twitter_share_setting_form($node, $form_state) {
    
    $form['facebook_twitter_share_url'] = array(
        '#type' => 'textfield',
        '#title' => t('Facebook Twitter sharing url used'),
        '#default_value' => variable_get('facebook_twitter_share_url', 'http://www.sepulsa.com?o=<order_id>'),
        '#description' => t('The url shared for sharing, if empty facebook share will automatically disabled (use "<order_id>" to override the data with order_id value)'),  
        '#required' => FALSE,
    );
    
    $form['twitter_predefined_text'] = array(
        '#type' => 'fieldset',
        '#title' => t('Twitter Predefined Text'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    
    $form['twitter_predefined_text']['twitter_predefined_text_before_url'] = array(
        '#type' => 'textfield',
        '#title' => t('Twitter Predefined Text before url'),
        '#default_value' => variable_get('twitter_predefined_text_before_url', 'Saya membeli pulsa di sepulsa - '),
        '#description' => t('The predefined text before the url for Twitter'),  
        '#required' => FALSE,
    );
    
    $form['twitter_predefined_text']['twitter_predefined_text_after_url'] = array(
        '#type' => 'textfield',
        '#title' => t('Twitter Predefined Text after url'),
        '#default_value' => variable_get('twitter_predefined_text_after_url', ''),
        '#description' => t('The predefined text after the url for Twitter'),  
        '#required' => FALSE,
    );
    
    $form['enable_disable'] = array(
        '#type' => 'fieldset',
        '#title' => t('Enable/Disable Facebook Twitter share'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    
    $form['enable_disable']['enable_facebook_share'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable/Disable Facebook share'),
        '#default_value' => variable_get('enable_facebook_share', 1),
    );
    
    $form['enable_disable']['enable_twitter_share'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable/Disable Twitter share'),
        '#default_value' => variable_get('enable_twitter_share', 1),
    );
    
    // Return form array.
    return system_settings_form($form);
}