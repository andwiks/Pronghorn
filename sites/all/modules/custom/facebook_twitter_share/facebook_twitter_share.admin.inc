<?php

function facebook_twitter_share_setting_form($node, $form_state) {
    
    $form['facebook_twitter_share_url'] = array(
        '#type' => 'textfield',
        '#title' => t('Facebook Twitter sharing url used'),
        '#default_value' => variable_get('facebook_twitter_share_url', 'http://www.sepulsa.com?o=<order_id>'),
        '#description' => t('The url shared for sharing, if empty facebook share will automatically disabled (use "<order_id>" to override the data with order_id value)'),  
        '#required' => FALSE,
    );
    
    $form['facebook_twitter_share_info'] = array(
        '#type' => 'textarea',
        '#title' => t('Facebook Twitter info title to define to user their benefits of sharing'),
        '#default_value' => variable_get('facebook_twitter_share_info', 'Share transaksimu ke akun Media Sosial dan dapatkan kesempatan memenangkan tambahan sepulsa kredit'),
        '#description' => t('Description at the top of the sharing button, to explain the user their benefits if sharing'),  
        '#required' => TRUE,
    );
    
    $form['twitter_predefined_text'] = array(
        '#type' => 'fieldset',
        '#title' => t('Twitter Predefined Text'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
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
        '#collapsed' => FALSE,
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
    
    $form['facebook_twitter_share_endpoint'] = array(
        '#type' => 'fieldset',
        '#title' => t('Facebook and Twitter share endpoint'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    
    $form['facebook_twitter_share_endpoint']['facebook_share_endpoint'] = array(
        '#type' => 'textfield',
        '#title' => t('Facebook share endpoint'),
        '#default_value' => variable_get('facebook_share_endpoint', 'https://www.facebook.com/sharer/sharer.php?u='),
        '#description' => t('The endpoint for facebook share'),  
        '#required' => TRUE,
    );
    
    $form['facebook_twitter_share_endpoint']['twitter_share_endpoint'] = array(
        '#type' => 'textfield',
        '#title' => t('Twitter share endpoint'),
        '#default_value' => variable_get('twitter_share_endpoint', 'https://twitter.com/intent/tweet?text='),
        '#description' => t('The endpoint for twitter share'),  
        '#required' => TRUE,
    );
    
    // Return form array.
    return system_settings_form($form);
}