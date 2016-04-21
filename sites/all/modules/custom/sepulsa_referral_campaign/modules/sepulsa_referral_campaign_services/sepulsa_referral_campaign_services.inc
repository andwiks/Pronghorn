<?php
/**
 * @file
 * sepulsa_referral_campaign_services.inc
 *
 * @author aizat@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function sepulsa_referral_campaign_services_resource_index().
 */
function sepulsa_referral_campaign_services_resource_index() {
  global $user;
  if ($user->uid > 0) {
    // List sepulsa campaign.
    $list = sepulsa_referral_campaign_getlist();
    if (!empty($list)) {
    	return $list;
    }
    $return = array(
      'msg' => 'There is no active campaign.',
    );
    return $return;
  }
  return services_error(t('Invalid User.'), 401);
}

/**
 * Function sepulsa_referral_campaign_services_resource_getcampaign().
 */
function sepulsa_referral_campaign_services_resource_getcampaign() {
  global $user;
  if ($user->uid > 0) {
    // Active sepulsa campaign.
    $list = sepulsa_referral_campaign_getlist(TRUE);
    if (!empty($list)) {
    	return $list;
    }
    $return = array(
      'msg' => 'There is no active campaign.',
    );
    return $return;
  }
  return services_error(t('Invalid User.'), 401);
}

/**
 * Function sepulsa_referral_campaign_services_resource_request().
 */
function sepulsa_referral_campaign_services_resource_request() {
  global $base_url;
  global $user;

  if ($user->uid > 0) {
    $list = sepulsa_referral_campaign_getlist(TRUE);
    if ($list) {
      $return = array(
        'code' => _referral_uid2ref($user->uid),
        'link_referral' => $base_url . '/' . $list[0]->path . '?code=' . _referral_uid2ref($user->uid),
      );
      return $return;
    }
    return services_error(t('There is no active campaign.'), 406);
  }
  return services_error(t('Invalid User.'), 401);
}

/**
 * Function sepulsa_referral_campaign_services_resource_register().
 */
function sepulsa_referral_campaign_services_resource_register($code = '') {
  global $user;

  if ($user->uid > 0) {
    if (!empty($code)) {
    	// Get active campaign.
    	$list = sepulsa_referral_campaign_getlist(TRUE);
    	if (!empty($list)) {
    	  $referral_uid = _referral_ref2uid($code);
    	  $referral_user = user_load($referral_uid);
    	  if (!empty($referral_user)) {
    	  	// Get UID in session.
      		$uid = $user->uid; // Misal.
      		// Check whether the user use referral code in the same campaign.
    	    $query = db_select('referral_campaign_details', 'rcd')
    	  			->fields('rcd')
    	  			->condition('uid', $uid)
    	  			->condition('src_id', $list[0]->src_id)
    	  			->execute();
      		if ($query->rowCount() == 0) {
      		  // Set data for referral_campaign_details. 
      		  $array_insert = array(
      		    'src_id' => $list[0]->src_id,
      		    'uid' => $uid,
      		    'referral_uid' => $referral_uid,
      		    'created' => REQUEST_TIME,
      		    'host' => ip_address(),
      		    'http_referer' => $_SERVER['HTTP_REFERER'],
      		  );
      		  // Insert data to referral_campaign_details.
      		  db_insert('referral_campaign_details')
    	  		  ->fields($array_insert)
    			  ->execute();
    	  	  return TRUE;
      		}
      		return services_error(t('Referral code has been used in this campaign.'), 406);
    	  }
    	  return services_error(t('Invalid Referral Code.'), 406);
    	}
    	return services_error(t('There is no active campaign.'), 406);
    }
  }
  return services_error(t('Invalid User.'), 401);
}
