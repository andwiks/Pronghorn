<?php
/**
 * @file
 * template.php
 *
 * @author ananto@sepulsa.com
 * @since February 2nd 2015
 */

function sepulsa_form_alter(&$form, &$form_state, $form_id) {
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
  } else if ($commer_form_id == 'commerce_cart_add_to_cart' && arg(0) == 'coupon') {
    //drupal_set_message("<pre>".print_r($form, true)."</pre>");
    $form['submit']['#attributes'] = array('class' => array('btn', 'btn-sm', 'style3', 'post-read-more'));
  }
}