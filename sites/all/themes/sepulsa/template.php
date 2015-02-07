<?php
/**
 * @file
 * template.php
 *
 * @author ananto@sepulsa.com
 * @since February 2nd 2015
 */

function sepulsa_form_alter(&$form, &$form_state, $form_id) {
  //drupal_set_message("<pre>".print_r($form_id, true)."</pre>");
  $commer_form_id = substr($form_id, 0, 25);
  if ($form_id == "sepulsa_phone_form") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['phone']['#title'] = NULL;
    $form['phone']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Masukkan Nomor Handphone');

    $form['operator']['#title'] = NULL;
    $form['operator']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Operator');

    $form['card_type']['#title'] = NULL;
    $form['card_type']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Pilihan Kartu');

    $form['packet']['#title'] = NULL;
    $form['packet']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Pilihan Paket');
  } else if ($form_id == "user_login_block") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['name']['#title'] = NULL;
    $form['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    
    $form['pass']['#title'] = NULL;
    $form['pass']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Password');
    
    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));
    $form['links']['#markup'] = l(t('Request New Password'), 'user/password');
    
  } else if ($commer_form_id == 'commerce_cart_add_to_cart' && arg(0) == 'coupon') {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['submit']['#attributes'] = array('class' => array('btn', 'btn-sm', 'style3', 'post-read-more'));
    
  } else if ($form_id == "user_login") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['name']['#title'] = NULL;
    $form['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    
    $form['pass']['#title'] = NULL;
    $form['pass']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Password');
    
    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));
    $form['links']['#markup'] = l(t('Request New Password'), 'user/password');
    
  } else if ($form_id == "user_register_form") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['account']['name']['#title'] = NULL;
    $form['account']['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Username');
    
    $form['account']['mail']['#title'] = NULL;
    $form['account']['mail']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    
    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));
    
  } else if ($form_id == "user_pass") {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['name']['#title'] = NULL;
    $form['name']['#attributes'] = array('class' => array('input-text', 'full-width'), 'placeholder' => 'Alamat Email');
    
    $form['actions']['submit']['#attributes'] = array('class' => array('btn', 'style1'));
    
  }
}

function sepulsa_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}
