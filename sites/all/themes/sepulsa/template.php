<?php
/**
 * @file
 * template.php
 *
 * @author ananto@sepulsa.com
 * @since February 2nd 2015
 */

function sepulsa_form_alter(&$form, &$form_state, $form_id) {
  //drupal_set_message($form_id);
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

  }
}