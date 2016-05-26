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
function sepulsa_referral_campaign_services_resource_index($active = NULL) {
  global $user;
  if ($user->uid > 0) {
    // List sepulsa campaign.
    $list = sepulsa_referral_campaign_getlist($active);
    if (!empty($list)) {
      if ($list != 'invalid') {
        return $list;
      }
      return services_error(t('Invalid argument.'), 406);
    }
    $msg_active = (is_null($active)) ? '' : $active;
    $return = array(
      'msg' => 'There is no ' . $msg_active . ' campaign.',
    );
    return $return;
  }
  return services_error(t('Invalid User.'), 401);
}

/**
 * Function sepulsa_referral_campaign_services_resource_retrieve().
 */
function sepulsa_referral_campaign_services_resource_retrieve($id) {
  global $user;
  if ($user->uid > 0) {
    // List sepulsa campaign.
    $details = sepulsa_referral_campaign_load($id);
    if (!empty($details)) {
      return $details;
    }
    return services_error(t('Campaign is not active.'), 406);
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
    $list = sepulsa_referral_campaign_getlist('active');
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
    $list = sepulsa_referral_campaign_getlist('active');
    if ($list) {
      $return = array(
        'code' => _referral_uid2ref($user->uid),
        'link_referral' => url($list[0]->path,
          array(
            'absolute' => TRUE,
            'query' => array(
              'code' => _referral_uid2ref($user->uid),
            ),
          )
        ),
        'src_id' => $list[0]->src_id,
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
      $list = sepulsa_referral_campaign_getlist('active');
      if (!empty($list)) {
        $referral_uid = _referral_ref2uid($code);
        $referral_user = user_load($referral_uid);
        if (!empty($referral_user)) {
          // Get UID in session.
          $uid = $user->uid;
          if ($uid != $referral_user) {
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
              watchdog('sepulsa_referral_campaign_services',
                'Insert Data to referral_campaign_details : <pre> @data </pre>',
                array('@data' => print_r($array_insert, TRUE)),
                WATCHDOG_NOTICE, 'info');
              return TRUE;
            }
            return services_error(t('Referral code has been used in this campaign.'), 406);
          }
          return services_error(t('Referral code can not be used by your own.'), 406);
        }
        return services_error(t('Invalid Referral Code.'), 406);
      }
      return services_error(t('There is no active campaign.'), 406);
    }
    return services_error(t('Invalid Referral Code.'), 406);
  }
  return services_error(t('Invalid User.'), 401);
}

/**
 * Function sepulsa_referral_campaign_services_resource_status().
 */
function sepulsa_referral_campaign_services_resource_status($id = NULL, $mode = NULL) {
  global $user;
  if ($user->uid > 0) {
    // Check id from body service.
    if (is_null($id)) {
      // Get active campaign.
      $details = sepulsa_referral_campaign_getlist('active');
    }
    else {
      // Get details campaign.
      $details = sepulsa_referral_campaign_load($id);
    }
    // Check whether detail is exist or not.
    if (!empty($details)) {
      if (is_null($id)) {
        $id = $details[0]->src_id;
      }
      // Get uplink / downlink from campaign. (default mode : downlink)
      if (!is_null($mode)) {
        $link = sepulsa_referral_campaign_getlink($user->uid, $id, $mode);
      }
      else {
        $link = sepulsa_referral_campaign_getlink($user->uid, $id);
      }
      if (!empty($link)) {
        if ($link != 'invalid_mode') {
          return $link;
        }
        return services_error(t('Invalid Mode link (uplink / downlink).'), 406);
      }
      $msg = array(
        'msg' => 'Empty result.',
      );
      return $msg;
    }
    return services_error(t('Invalid Campaign ID.'), 406);
  }
  return services_error(t('Invalid User.'), 401);
}

/**
 * Function sepulsa_referral_campaign_getlink().
 *
 * @param string $uid
 *   User ID.
 * @param string $src_id
 *   Campaign ID (src = sepulsa_referral_campaign [tablename]).
 * @param string $mode
 *   Mode for this function default : all , another mode uplink / downlink.
 */
function sepulsa_referral_campaign_getlink($uid, $src_id, $mode = 'downlink') {
  switch ($mode) {
    case 'uplink':
      // Query for uplink user.
      $query = db_select('referral_campaign_details', 'rcd');
      $query->join('users', 'u', 'rcd.referral_uid = u.uid');
      $query->join('sepulsa_referral_campaign', 'src', 'rcd.src_id = src.src_id');
      $query->join('device', 'd', 'd.uid = rcd.referral_uid');
      $query->fields('u', array('mail'));
      $query->fields('d', array('phone'));
      $query->fields('rcd', array('flag', 'flag_timestamp'));
      $query->fields('src', array('desc_benefit_referral'));
      $query->condition('rcd.referral_uid', $uid);
      $query->condition('rcd.src_id', $src_id);
      $result = $query->execute();
      foreach ($result as $key => $value) {
        $return['uplink'][] = $value;
      }
      break;

    case 'downlink':
      // Query for downlink user.
      $query = db_select('referral_campaign_details', 'rcd');
      $query->join('users', 'u', 'rcd.uid = u.uid');
      $query->join('sepulsa_referral_campaign', 'src', 'rcd.src_id = src.src_id');
      $query->join('device', 'd', 'd.uid = rcd.uid');
      $query->fields('u', array('mail'));
      $query->fields('d', array('phone'));
      $query->fields('rcd', array('flag', 'flag_timestamp'));
      $query->fields('src', array('desc_benefit'));
      $query->condition('rcd.uid', $uid);
      $query->condition('rcd.src_id', $src_id);
      $result = $query->execute();
      foreach ($result as $key => $value) {
        $return['downlink'][] = $value;
      }
      break;

    case 'all':
      // Query for uplink user.
      $query = db_select('referral_campaign_details', 'rcd');
      $query->join('users', 'u', 'rcd.referral_uid = u.uid');
      $query->join('sepulsa_referral_campaign', 'src', 'rcd.src_id = src.src_id');
      $query->join('device', 'd', 'd.uid = rcd.referral_uid');
      $query->fields('u', array('mail'));
      $query->fields('d', array('phone'));
      $query->fields('rcd', array('flag', 'flag_timestamp'));
      $query->fields('src', array('desc_benefit_referral'));
      $query->condition('rcd.referral_uid', $uid);
      $query->condition('rcd.src_id', $src_id);
      $result = $query->execute();
      foreach ($result as $key => $value) {
        $return['uplink'][] = $value;
      }

      // Query for downlink user.
      $query = db_select('referral_campaign_details', 'rcd');
      $query->join('users', 'u', 'rcd.uid = u.uid');
      $query->join('sepulsa_referral_campaign', 'src', 'rcd.src_id = src.src_id');
      $query->join('device', 'd', 'd.uid = rcd.uid');
      $query->fields('u', array('mail'));
      $query->fields('d', array('phone'));
      $query->fields('rcd', array('flag', 'flag_timestamp'));
      $query->fields('src', array('desc_benefit'));
      $query->condition('rcd.uid', $uid);
      $query->condition('rcd.src_id', $src_id);
      $result = $query->execute();
      foreach ($result as $key => $value) {
        $return['downlink'][] = $value;
      }
      break;

    default:
      $return = 'invalid_mode';
      break;
  }

  return $return;
}
