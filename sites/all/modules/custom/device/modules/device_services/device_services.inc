<?php
/**
 * @file
 * devices_servies.inc
 *
 * @author andreas@sepulsa.com
 *
 * @todo: logout session.
 */

/**
 * Callback Functions.
 */

/**
 * Function device_services_resource_request().
 *
 * @param string $imei
 *   Device IMEI.
 * @param string $phone
 *   Device Phone number.
 *
 * @return mixed
 *   Access and refresh token or error message.
 */
function device_services_resource_request($imei = '', $phone = '', $agent = '') {
  // Check input imei and phone.
  if (!empty($imei) && !empty($phone) && !empty($agent)) {
    // 1. Validate phone number.
    // 1a. Available and not empty.
    // 1b. Has pattern ^08digits.
    // 1c. At least has 8 chars (magic number?).
    // 1d. Maximum has 16 chars (magic number?).
    // 2. Validate imei
    // 2a. Minimum 14 digits.
    // 2b. Maximum 16 digits.
    // 2c. Email address (iOS version).
    if (preg_match(DEVICE_PHONE_PATTERN, $phone)
      && (preg_match(DEVICE_IMEI_PATTERN, $imei) || valid_email_address($imei))
    ) {
      // Get default device session timeout.
      $device_session_timeout = variable_get('device_session_timeout', array(
        'refresh' => '+30 days',
        'access' => '+1 hour',
      ));
      // Check imei and phone on device entity.
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'device')
        ->propertyCondition('imei', $imei)
        ->propertyCondition('phone', $phone);
      $result = $query->execute();
      // Check if empty result.
      if (empty($result)) {
        // Device is new: create new device entity.
        $entity = entity_get_controller('device')->create(array(
          'imei' => $imei,
          'phone' => $phone,
          'label' => $phone,
          'agent' => $agent,
          'created' => REQUEST_TIME,
          'access' => 0,
          'status' => 1,
          'uid' => 0,
          'data' => serialize(array()),
        ));
        entity_save('device', $entity);

        // Get device verification hash variable.
        $hash = device_generate_hash();
        // Create new device verification.
        $entity_verification = entity_get_controller('device_verification')->create(array(
          'did' => $entity->did,
          'phone' => $phone,
          'created' => REQUEST_TIME,
          'timeout' => strtotime(variable_get('device_verification_timeout', '+30 minutes'), REQUEST_TIME),
          'status' => 0,
          'hash' => $hash,
        ));
        entity_save('device_verification', $entity_verification);

        // Set new device session.
        drupal_session_initialize();
        // Insert new device session.
        db_insert('device_sessions')->fields(array(
          'did' => $entity->did,
          'asid' => session_id(),
          'rsid' => '',
          'atimestamp' => REQUEST_TIME,
          'rtimestamp' => 0,
        ))->execute();

        // Define token container.
        $token = array();
        // Generate access token: place it here, if something wrong, no insert.
        $token['access'] = device_services_generate_token('access', array(
          'scope' => array(
            'device' => 'verify',
          ),
          'jti' => session_id(),
          'did' => $entity->did,
          'aud' => $imei,
        ));

        // Send verification message.
        device_send_verification_message($phone, $hash);

        // Log this event.
        watchdog('device_services', 'Request service: device new. Debug: <pre>@imei<br/>@phone<br/>@entity<br/>@verification<br/>@token</pre>', array(
          '@imei' => $imei,
          '@phone' => $phone,
          '@entity' => print_r($entity, TRUE),
          '@verification' => print_r($entity_verification, TRUE),
          '@token' => print_r($token, TRUE),
        ), WATCHDOG_DEBUG);

        // Return token.
        return $token;
      }
      else {
        // Get device id.
        $did = key($result['device']);
        // Define device check variable.
        $check = array();
        // Record the request time.
        $check['request'] = REQUEST_TIME . ' (' . format_date(REQUEST_TIME, 'short') . ')';

        // Handled device has been available before.
        // 1. Check device status: blocked or not blocked.
        $device_wrapper = entity_metadata_wrapper('device', $did);
        $check['device_status'] = $device_wrapper->status->value();
        $check['device_uid'] = $device_wrapper->uid->value();
        // 2. Check verification status: unconfirmed or new.
        $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', 'device_verification')
          ->propertyCondition('did', $did);
        $result = $query->execute();
        if (!empty($result)) {
          $check['dv_id'] = key($result['device_verification']);
          $dv_wrapper = entity_metadata_wrapper('device_verification', $check['dv_id']);
          $check['dv_hash'] = $dv_wrapper->hash->value();
          $check['dv_status'] = $dv_wrapper->status->value();
          $check['dv_timeout'] = $dv_wrapper->timeout->value();
        }
        // 3. Get device session: token status timeout.
        $sessions = db_select('device_sessions', 'ds')
          ->fields('ds')->condition('did', $did)->execute()->fetchAll();
        // 4. Get available claims.
        $check['claims'] = jwt_auth_claims();
        // 5. Define flag: valid refresh token.
        $check['refresh'] = FALSE;
        // Device is confirmed: check refresh token.
        if (isset($check['claims']['jti'])) {
          $rsid = $check['claims']['jti']->getValue();
          // Check jti / rsid with database device_sessions.
          if (!empty($sessions)) {
            foreach ($sessions as $session) {
              // Check refresh timestamp: valid refresh token.
              if (isset($session->rtimestamp)
                && strtotime($device_session_timeout['refresh'], $session->rtimestamp) > REQUEST_TIME
                && isset($session->rsid) && $session->rsid == $rsid
              ) {
                $check['refresh'] = $rsid;
                $check['access'] = $session->asid;
                // Breaking the loop.
                break;
              }
            }
          }
        }

        // Log this event.
        watchdog('device_services', 'Request service: device found. Debug: <pre>Imei: @imei<br/>Phone: @phone<br/>Device ID: @did<br />Check: @check<br />Sessions: @sess</pre>', array(
          '@imei' => $imei,
          '@phone' => $phone,
          '@did' => $did,
          '@check' => print_r($check, TRUE),
          '@sess' => print_r($sessions, TRUE),
        ), WATCHDOG_DEBUG);

        // Check device status: blocked.
        if (empty($check['device_status'])) {
          return services_error(t('Device has been blocked.'), 401);
        }

        // Check device verication: unconfirmed or new.
        // Confirmed but no / expired refresh token.
        if ($check['dv_status'] == 2 || $check['dv_status'] == 0
          || ($check['dv_status'] == 1 && empty($check['refresh']))
        ) {
          // Check if:
          // 1. unconfirmed: need to generate new hash.
          // 2. confirmed: only if more than timeout.
          if ($check['dv_status'] == 2
            || ($check['dv_status'] == 1 &&  REQUEST_TIME > $check['dv_timeout'])
          ) {
            // Regenerate hash: set as new.
            $check['dv_hash'] = device_generate_hash();
            // Update device verification entity.
            if ($check['dv_status'] == 2) {
              $dv_wrapper->status->set(0);
            }
            $dv_wrapper->created->set(REQUEST_TIME);
            $dv_wrapper->timeout->set(strtotime(variable_get('device_verification_timeout', '+30 minutes'), REQUEST_TIME));
            $dv_wrapper->hash->set($check['dv_hash']);
            $dv_wrapper->save();
          }
          // Check device session: access session.
          if (!empty($sessions)) {
            // Delete it for regenerate after it.
            db_delete('device_sessions')->condition('did', $did)->execute();
          }
          // Set new device session.
          drupal_session_initialize();
          // Insert new device session.
          db_insert('device_sessions')->fields(array(
            'did' => $did,
            'asid' => session_id(),
            'rsid' => '',
            'atimestamp' => REQUEST_TIME,
            'rtimestamp' => 0,
          ))->execute();
          // Define token container.
          $token = array();
          // Generate access token.
          $token['access'] = device_services_generate_token('access', array(
            'scope' => array(
              'device' => 'verify',
            ),
            'jti' => session_id(),
            'did' => $did,
            'aud' => $imei,
          ));
          // Send verification message.
          device_send_verification_message($phone, $check['dv_hash']);
          // Return token.
          return $token;
        }
        elseif ($check['dv_status'] == 1 && !empty($check['refresh'])) {
          // Confirmed device with valid refresh token.
          global $is_https;
          // Delete drupal session also.
          db_delete('sessions')->condition($is_https ? 'ssid' : 'sid', $check['access'])->execute();
          // Start drupal session.
          drupal_session_start();
          // Give access token with sliding expiration for refresh token.
          // Update device session: new asid and updated timestamp.
          db_merge('device_sessions')
            ->key(array(
              'did' => $did,
              'rsid' => $check['refresh'],
            ))
            ->fields(array(
              'asid' => session_id(),
              'atimestamp' => REQUEST_TIME,
              'rtimestamp' => REQUEST_TIME,
            ))
            ->execute();
          // Set fake cookie name.
          $_COOKIE[session_name()] = TRUE;
          // Save this session.
          drupal_save_session(TRUE);
          // Check user to save session.
          global $user;
          if ($check['device_uid'] > 0 && $user->uid == 0) {
            // Load user and store it on global user object.
            $user = user_load($check['device_uid']);
          }
          // Commit the session.
          drupal_session_commit();

          // Define token container.
          $token = array();
          // Define scope claims.
          $scope = device_services_get_client_scope();
          $scope += array(
            'device' => array('mail', 'retrieve', 'index', 'logout'),
          );
          // Generate access token: place it here.
          $token['access'] = device_services_generate_token('access', array(
            'scope' => $scope,
            'jti' => session_id(),
            'did' => $did,
            'aud' => $user->mail,
          ));
          // Generate refresh token.
          $token['refresh'] = device_services_generate_token('refresh', array(
            'jti' => $check['refresh'],
            'did' => $did,
            'aud' => $device_wrapper->imei->value(),
          ));
          // Return new token.
          return $token;
        }
      }
    }
  }

  // Other condition.
  return services_error(t('Invalid request.'), 406);
}

/**
 * Function device_services_resource_verify().
 *
 * @param string $code
 *   Verification code.
 *
 * @return mixed
 *   Access and refresh token or error message.
 */
function device_services_resource_verify($code = '') {
  // Check input code: can not empty.
  if (!empty($code)) {
    // Validate device session.
    $session = device_services_default_session_auth('verify');
    if ($session !== FALSE) {
      // Query to get device verification id.
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'device_verification')
        ->propertyCondition('did', $session['device']->did);
      $result = $query->execute();
      // Check the query result.
      if (!empty($result)) {
        // Get wrapper for device verification.
        $verification_wrapper = entity_metadata_wrapper('device_verification', key($result['device_verification']));
        // Validate hash code, timeout and status.
        if ($verification_wrapper->hash->value() == $code
          && REQUEST_TIME < $verification_wrapper->timeout->value()
        ) {
          // Only if status if not confirmed.
          if ($verification_wrapper->status->value() != 1) {
            // Change device verification to confirmed.
            $verification_wrapper->status->set(1);
          }
          // Set timeout as 0: next request hash has already expired.
          $verification_wrapper->timeout->set(0);
          $verification_wrapper->save();

          // Get global https variable.
          global $is_https;

          // Remove old device session.
          db_delete('device_sessions')->condition('did', $session['device']->did)->execute();
          // Delete drupal session also.
          db_delete('sessions')->condition($is_https ? 'ssid' : 'sid', $session['asid'])->execute();

          // Search for old user.
          $account = db_select('users', 'u')->fields('u')
            ->condition('init', $session['device']->phone)
            ->execute()->fetch();

          // Check if user is empty.
          if (empty($account)) {
            // Create user dummy: set up the user fields.
            $account = new stdClass();
            $account->is_new = TRUE;
            $account->name = $session['device']->phone;
            $account->mail = $session['device']->imei . '@' . $session['device']->phone . '.dummy';
            $account->pass = user_password();
            $account->status = 1;
            $account->init = $session['device']->phone;
            $account->roles = array(
              DRUPAL_AUTHENTICATED_RID => 'authenticated user',
            );
            $account = user_save($account);
          }
          else {
            // User may have another device: remove old sessions.
            $list = db_select('device', 'dev')
              ->fields('dev', array('did'))
              ->condition('uid', $account->uid)
              ->execute()->fetchAll();
            if (!empty($list)) {
              foreach ($list as $list_device) {
                db_delete('device_sessions')->condition('did', $list_device->did)->execute();
              }
            }
          }

          // Update device uid.
          $device_wrapper = entity_metadata_wrapper('device', $session['device']->did);
          $device_wrapper->uid->set($account->uid);
          $device_wrapper->save();

          // Regenerate session: session already started.
          drupal_session_regenerate();
          // Create refresh token.
          $rsid = drupal_random_key(64);
          // Insert new device session.
          db_insert('device_sessions')->fields(array(
            'did' => $session['device']->did,
            'asid' => session_id(),
            'rsid' => $rsid,
            'atimestamp' => REQUEST_TIME,
            'rtimestamp' => REQUEST_TIME,
          ))->execute();

          // Get global user object.
          global $user;
          // Check if global user is still anonymous.
          if ($account->uid > 0 && $user->uid == 0) {
            // Load user and store it on global user object.
            $user = user_load($account->uid);
          }
          drupal_session_commit();

          // Define token container.
          $token = array();
          // Get device client scope.
          $scope = device_services_get_client_scope();
          $scope += array(
            'device' => array('mail', 'retrieve', 'index', 'logout'),
          );
          // Generate access token.
          $token['access'] = device_services_generate_token('access', array(
            'scope' => $scope,
            'jti' => session_id(),
            'did' => $session['device']->did,
            'aud' => $user->mail,
          ));
          // Generate refresh token.
          $token['refresh'] = device_services_generate_token('refresh', array(
            'jti' => $rsid,
            'did' => $session['device']->did,
            'aud' => $device_wrapper->imei->value(),
          ));
          // Return new token.
          return $token;
        }
      }
    }
  }

  // Other condition.
  return services_error(t('Invalid verification.'), 406);
}

/**
 * Function device_services_resource_mail().
 *
 * @param string $mail
 *   Email address.
 *
 * @return mixed
 *   TRUE or error message.
 */
function device_services_resource_mail($mail = '') {
  // Validate mail.
  if (valid_email_address($mail)) {
    // Validate device session.
    $session = device_services_default_session_auth('mail');
    if ($session !== FALSE) {
      $send_verify = device_mail_verification_send_verification($mail, $session['device']->did);
      if ($send_verify === FALSE) {
        return services_error(t('Send Email Failed.'), 406);
      }
      else {
        return TRUE;
      }
    }
  }
  // Other condition.
  return services_error(t('Invalid mail request.'), 406);
}

/**
 * Function device_services_resource_retrieve().
 *
 * @param int $did
 *   Device ID.
 *
 * @return mixed
 *   Device object or error message.
 */
function device_services_resource_retrieve($did = 0) {
  // Validate did.
  if (!empty($did)) {
    $did = intval($did);
    // Validate device session.
    $session = device_services_default_session_auth('retrieve');
    if ($session !== FALSE) {
      // Check did must the same ass session device id.
      if ($did == $session['device']->did) {
        // Reformating device mail: stupid way.
        if (isset($session['device']->device_mail[LANGUAGE_NONE][0])
          && !empty($session['device']->device_mail[LANGUAGE_NONE][0])
        ) {
          $session['device']->device_mail = $session['device']->device_mail[LANGUAGE_NONE][0];
          if (isset($session['device']->device_mail['email']) && !empty($session['device']->device_mail['email'])) {
            $session['device']->device_mail = $session['device']->device_mail['email'];
          }
        }
        else {
          $session['device']->device_mail = NULL;
        }
        // Return device object.
        return $session;
      }
    }
  }
  // Other condition.
  return services_error(t('Invalid device information.'), 404);
}

/**
 * Function device_services_resource_index().
 */
function device_services_resource_index() {
  // Validate device session.
  $session = device_services_default_session_auth('index');
  if ($session !== FALSE && isset($session['device']->uid)) {
    $devices = array();
    // Query the device for this user.
    $query = db_select('device', 'dev');
    $query->fields('dev', array('did'));
    $query->condition('uid', $session['device']->uid);
    $list = $query->execute()->fetchAll();
    if (!empty($list)) {
      foreach ($list as $d) {
        $device = device_load($d->did);
        // Reformating device mail: stupid way.
        if (isset($device->device_mail[LANGUAGE_NONE][0])
          && !empty($device->device_mail[LANGUAGE_NONE][0])
        ) {
          $device->device_mail = $device->device_mail[LANGUAGE_NONE][0];
          if (isset($device->device_mail['email']) && !empty($device->device_mail['email'])) {
            $device->device_mail = $device->device_mail['email'];
          }
        }
        else {
          $device->device_mail = NULL;
        }
        $devices[] = $device;
      }
    }
    return $devices;
  }
  // Other condition.
  return services_error(t('Invalid device information.'), 404);
}

/**
 * Function device_services_resource_logout().
 *
 * @return mixed
 *   TRUE or error message.
 */
function device_services_resource_logout() {
  // Validate device session.
  $session = device_services_default_session_auth('logout');
  // Check session did.
  if ($session !== FALSE && isset($session['device']->did)) {
    global $is_https;
    // Remove device session.
    db_delete('device_sessions')->condition('did', $session['device']->did)->execute();
    // Delete drupal session also.
    db_delete('sessions')->condition($is_https ? 'ssid' : 'sid', $session['asid'])->execute();
  }

  // Any condition: return TRUE.
  return TRUE;
}

/**
 * Function device services_resource_session_index().
 */
function device_services_resource_session_index() {
  // Validate device session.
  $session = device_services_default_session_auth('index');
  if ($session !== FALSE && isset($session['device']->did)) {
    $sessions = array();
    // Query the device session.
    $query = db_select('device_sessions', 'ds');
    $query->fields('ds', array('did', 'asid', 'atimestamp'));
    $query->condition('did', $session['device']->did);
    $list = $query->execute()->fetchAll();
    if (!empty($list)) {
      foreach ($list as $sess) {
        $sessions[] = $sess;
      }
    }
    return $sessions;
  }
  // Other condition.
  return services_error(t('Invalid device information.'), 404);
}

/**
 * Function device_services_resource_session_retrieve().
 *
 * @param string $sid
 *   Access token session id.
 */
function device_services_resource_session_retrieve($sid) {
  if (!empty($sid)) {
    // Query the device session: using inputted sid.
    $query = db_select('device_sessions', 'ds');
    $query->fields('ds', array('did', 'asid', 'atimestamp'));
    $query->condition('asid', $sid);
    $list = $query->execute()->fetchAll();
    if (!empty($list)) {
      $sessions = array();
      foreach ($list as $sess) {
        $sessions[] = $sess;
      }
      return $sessions;
    }
  }
  // Other condition.
  return FALSE;
}

/**
 * Function device_services_resource_session_retrieve().
 *
 * @param string $sid
 *   Access token session id.
 */
function device_services_resource_session_delete($sid) {
  if (!empty($sid)) {
    // Query the device session: using inputted sid.
    $exists = db_select('device_sessions', 'ds')
      ->fields('ds', array('did', 'asid', 'atimestamp'))
      ->condition('asid', $sid)
      ->countQuery()
      ->execute()
      ->fetchField();
    if (!empty($exists)) {
      // Remove device session.
      db_delete('device_sessions')->condition('asid', $sid)->execute();
      // Delete drupal session also.
      db_delete('sessions')->condition($is_https ? 'ssid' : 'sid', $sid)->execute();
      // Return success flag.
      return TRUE;
    }
  }
  // Other condition.
  return FALSE;
}

/**
 * Private Functions.
 */

/**
 * Function device_services_generate_token().
 *
 * @param string $type
 *   Token type: access|refresh.
 * @param array $data
 *   Additional claim data.
 *
 * @return string
 *   JWT signed string.
 */
function device_services_generate_token($type, $data = array()) {
  // Get default device session timeout.
  $device_session_timeout = variable_get('device_session_timeout', array(
    'refresh' => '+30 days',
    'access' => '+1 hour',
  ));
  // Load enable / disable default claims variable.
  $enable = variable_get('device_services_enable_claims', array(
    'aud' => 0,
    'exp' => 'exp',
    'iat' => 'iat',
    'iss' => 'iss',
    'nbf' => 'nbf',
    'sub' => 0,
  ));
  $enable = array_filter($enable);
  // Get current service jwt configuration.
  $config = jwt_resources_get_config();
  // Get default claims container.
  $claims = device_services_get_default_claims();
  // Add claim exp: expired time.
  $claims['exp'] = strtotime($device_session_timeout[$type], REQUEST_TIME);
  // Add claim nbf: not before.
  $claims['nbf'] = REQUEST_TIME;
  // Add claim iat: Issued at.
  $claims['iat'] = REQUEST_TIME;
  // Filter with enable / disable default claims.
  $claims = array_intersect_key($claims, $enable);
  // Looping foreach additional data.
  foreach ($data as $key => $value) {
    $claims[$key] = $value;
  }
  // Generate access token: place it here.
  return jwt_sign($claims, $config);
}

/**
 * Function device_services_default_session_auth().
 *
 * @param string $resource
 *   Device resource name.
 *
 * @return mixed
 *   Array of device id and access session id or FALSE if failed.
 */
function device_services_default_session_auth($resource) {
  // Get JWT claims data.
  $claims = jwt_auth_claims();
  // Check claim data.
  if (isset($claims['scope']) && isset($claims['did']) && isset($claims['jti'])) {
    // Get claim did.
    $did = $claims['did']->getValue();
    // Get claim jti.
    $jti = $claims['jti']->getValue();
    // Get scope.
    $scope = $claims['scope']->getValue();
    // Validate scope: array or string.
    if (isset($scope->device) && !empty($scope->device)
      && ((is_array($scope->device) && in_array($resource, $scope->device))
      || (is_string($scope->device) && ($scope->device == $resource || $scope->device == '*'))
    )) {
      // Get default device session timeout.
      $device_session_timeout = variable_get('device_session_timeout', array(
        'refresh' => '+30 days',
        'access' => '+1 hour',
      ));
      // Get session based on did and jti.
      $session = db_select('device_sessions', 'ds')
        ->fields('ds', array('atimestamp'))
        ->condition('did', $did)
        ->condition('asid', $jti)
        ->execute()->fetchAssoc();
      // Check access token session.
      if (isset($session['atimestamp'])
        && REQUEST_TIME < strtotime($device_session_timeout['access'], $session['atimestamp'])
      ) {
        // Get device data.
        $device = device_load($did);
        // Check device status: active device only.
        if (isset($device->status) && $device->status == 1) {
          // Start drupal session.
          drupal_session_start();
          // Save this session.
          drupal_save_session(TRUE);
          // Set session id with current jti value.
          session_id($jti);
          // Set fake cookie name.
          $_COOKIE[session_name()] = TRUE;
          // Get global user object.
          global $user, $is_https;
          if ($device->uid > 0 && $user->uid == 0) {
            // Load user and store it on global user object.
            $user = user_load($device->uid);
            // Save the user to services original user.
            services_set_server_info('original_user', $user);
          }
          // Check current sessions.
          $current = db_select('sessions', 's')->fields('s')->condition($is_https ? 'ssid' : 'sid', $jti)->execute()->fetchAll();
          if (empty($current)
            || $current[0]->uid != $device->uid
          ) {
            drupal_session_commit();
          }
          // Reset: populate session again.
          session_reset();
          // Remove session messages.
          if (isset($_SESSION['messages'])) {
            // Log this condition.
            watchdog('device_services', 'Device @did (@uid) [@imei-@phone]  Session Messages: <pre>@message</pre>', array(
              '@did' => $did,
              '@uid' => $device->uid,
              '@imei' => $device->imei,
              '@phone' => $device->phone,
              '@message' => print_r($_SESSION['messages'], TRUE),
            ), WATCHDOG_DEBUG);
            // Unset messages.
            unset($_SESSION['messages']);
          }
          // Update access time.
          $device_wrapper = entity_metadata_wrapper('device', $did);
          $device_wrapper->access->set(REQUEST_TIME);
          $device_wrapper->save();
          // Return session value.
          return array(
            'user' => $user,
            'device' => $device,
            'asid' => $jti,
            'atimeout' => strtotime($device_session_timeout['access'], $session['atimestamp']),
          );
        }
      }
    }
  }
  return FALSE;
}

/**
 * Function device_services_get_client_scope().
 *
 * Get client scope defined by jwt_server.
 *
 * @return array
 *   Client name and resource access.
 */
function device_services_get_client_scope() {
  $return = array();
  foreach (jwt_server_get_active() as $server) {
    $return[$server->name] = '*';
  }
  return $return;
}
