<?php

/**
 * @file
 * Referral Sepulsa Rules code: conditions.
 */

/**
 * Implements hook_rules_condition_info().
 */
function referral_sepulsa_rules_condition_info() {
  $conditions = array(
    'user_first_paid_in_full_order' => array(
      'label' => t('User First Order'),
      'group' => t('Sepulsa'),
      'parameter' => array(
        'order' => array(
          'type' => 'commerce_order',
          'label' => t('Commerce Order'),
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('User who referred to sepulsa'),
        ),
      ),
    ),
  );

  return $conditions;
}

/**
 * @file
 * Referral Sepulsa Rules code: actions.
 */

/**
 * Implements hook_rules_action_info().
 *
 * Declares any meta-data about actions for Rules in a big, associative, nested
 * array. See also hook_rules_action_info in the rules.api.php file, and the
 * online documentation at http://drupal.org/node/878928.
 */
function referral_sepulsa_rules_action_info() {
  $actions = array(
    'assign_point_to_referrer_user' => array(
      'label' => t('Assign Point to Referrer User'),
      'group' => t('Sepulsa'),
      'parameter' => array(
        'order' => array(
          'type' => 'commerce_order',
          'label' => t('Commerce Order'),
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('User who referred to sepulsa'),
        ),
				'point_type_referrer' => array(
          'type' => 'text',
          'label' => t('Point Percentage for Referrer'),
          'description' => t('Choose one of point type below'),
					'options list' => 'referral_sepulsa_point_type_options',
          'restriction' => 'input',
        ),
        'point_percentage_referrer' => array(
          'type' => 'integer',
          'label' => t('Point Percentage for Referrer'),
          'description' => t('Percentage of commerce order total convert into points (eg. 10)'),
        ),
				'point_type_refer' => array(
          'type' => 'text',
          'label' => t('Point Percentage for User Refer'),
          'description' => t('Choose one of point type below'),
					'options list' => 'referral_sepulsa_point_type_options',
          'restriction' => 'input',
        ),
        'point_percentage_refer' => array(
          'type' => 'integer',
          'label' => t('Point Percentage for User Refer'),
          'description' => t('Percentage of commerce order total convert into points (eg. 10)'),
        ),
        'category' => array(
          'type' => 'text',
          'label' => t('Userpoint Category'),
          'description' => t('Select Userpoint Category'),
          'options list' => 'referral_sepulsa_userpoint_category_options',
          'restriction' => 'input',
        ),
      ),
    ),
  );

  return $actions;
}

/**
 * The condition function for 'user_first_paid_in_full_order'.
 */
function user_first_paid_in_full_order($order, $user) {
    $order_id_prev = db_query_range("SELECT order_id FROM {commerce_order} WHERE (status = 'invoiced' OR status = 'completed' OR status = 'processing') AND uid = :uid AND order_id != :order_id", 0, 1, array('uid' => $user->uid, 'order_id' => $order->order_number))->fetchField();
		//drupal_set_message("<pre>order id prev: ".print_r($order_id_prev, true)."</pre>");
    // if order id prev exists, it means this is not user first order
    if ($order_id_prev) return FALSE;
    return TRUE;
}

/**
 * The action function for 'assign_point_to_referrer_user'.
 */
function assign_point_to_referrer_user($order, $user, $point_type_referrer, $point_percentage_referrer, $point_type_refer, $point_percentage_refer, $category) {
	// select table user_referral, get user referrer from current user
	//$referrer_uid = db_query_range("SELECT referral_uid FROM {referral} WHERE uid = :uid", 0, 1, array('uid' => $user->uid))->fetchField();
	if (isset($user->uid)) {
		$query = 'SELECT referral_uid, a.mail FROM users a INNER JOIN referral b ON a.uid = b.referral_uid WHERE b.uid = '.$user->uid;
		$referrer = db_query_range($query, 0, 1);
		foreach ($referrer as $row) {
			$referrer_uid = $row->referral_uid;
			$referrer_mail = $row->mail;
		}

		//drupal_set_message("<pre>referer uid: ".print_r($referrer_uid, true)."</pre>");
		if ($referrer_uid) {
			// assign point untuk yang merefer
			$order_total = $order->commerce_order_total["und"][0]["amount"];
			if ($point_percentage_referrer > 0) {
				if ($point_type_referrer == 'percentage') $points_referrer = ceil($order_total/100 * $point_percentage_referrer);
				else $points_referrer = $point_percentage_referrer;
				$params_referrer = array (
					'uid' => $referrer_uid,
					'points' => $points_referrer,
					'tid' => $category,
					'moderate' => TRUE,
					'description' => 'Refer '.$user->mail
				);
				userpoints_userpointsapi($params_referrer);
				_send_referral_credit_notification($user->mail, $referrer_mail, $points_referrer, true);
			}

			// assign point untuk yang di refer
			if ($point_percentage_refer > 0) {
				if ($point_type_refer == 'percentage') $points_refer = ceil($order_total/100 * $point_percentage_refer);
				else $points_refer = $point_percentage_refer;
				$params_refer = array (
					'uid' => $user->uid,
					'points' => $points_refer,
					'tid' => $category,
					'moderate' => TRUE,
					'description' => 'Referred by '.$referrer_mail
				);
				userpoints_userpointsapi($params_refer);
				_send_referral_credit_notification($user->mail, $referrer_mail, $points_refer);
			}
		}
	}
}

function _send_referral_credit_notification($email, $email_referrer, $point, $referrer = false){
	$point = number_format($point,0,",",".");
  $from = "info@sepulsa.com";
  $subject = "Anda Mendapatkan Sepulsa Kredit Sebesar Rp. ".$point;
	if ($referrer == true) {
		$to = $email_referrer;
		$message = "Anda mendapatkan Sepulsa Kredit sebesar Rp. ".$point." karena user yang anda undang ".$email." melakukan order pertama. Sepulsa Kredit akan masuk ke akun anda dalam waktu 1 x 24 jam. Syarat dan Ketentuan berlaku (https://www.sepulsa.com/promo-referral).";
	} else {
		$to = $email;
		$message = "Anda mendapatkan Kredit Sepulsa sebesar Rp. ". $point." karena diundang ".$email_referrer." dan telah melakukan order pertama kali. Sepulsa Kredit akan masuk ke akun anda dalam waktu 1 x 24 jam. Syarat dan Ketentuan berlaku (https://www.sepulsa.com/promo-referral).";
	}

  //params is the array passed to hook_mail function
  $params = array(
	  'subject' => $subject,
	  'body' => $message,
	  );

  drupal_mail('referral_sepulsa', 'referral_credit_notification', $to, language_default(), $params, $from);
}

function referral_sepulsa_userpoint_category_options() {
    $list = array();
    $vocabulary = taxonomy_vocabulary_machine_name_load('userpoints');
    $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
    foreach ($terms as $category) {
        $list[$category->tid] = $category->name;
    }
    //drupal_set_message("<pre>".print_r($list, true)."</pre>");

    return $list;
}

function referral_sepulsa_point_type_options() {
	$list = array('cash' => t('Cash'), 'percentage' => t('Percentage'));
	return $list;
}
