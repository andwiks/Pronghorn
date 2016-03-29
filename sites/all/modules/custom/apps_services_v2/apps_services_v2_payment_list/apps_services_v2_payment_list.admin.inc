<?php

function apps_services_v2_payment_list_credit_setting_form($node, $form_state) {
    
    $form['apps_services_v2_payment_list_credit_option'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable/Disable Credit Use in Checkout Payment'),
        '#default_value' => variable_get('apps_services_v2_payment_list_credit_option', 1),
    );
    
    // Return form array.
    return system_settings_form($form);
}

